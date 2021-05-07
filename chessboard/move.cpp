#include "move.h"

QString Move::reversible_algebraic(Field *origin,
                                   Field *destination) { // calculates reversible algebraic notation of move from origin to destination field
    std::pair<int, int> ori_pos = origin->getPosition();
    std::pair<int, int> dest_pos = destination->getPosition();
    QString rev_alg = Move::piece_to_letter()[origin->getPiece()] + Field::row_names()[ori_pos.second] +
                      QString::number(ori_pos.first + 1);
    if (destination->getPiece() != "") {
        rev_alg += " x ";
    } else {
        rev_alg += " - ";
    }
    rev_alg += Move::piece_to_letter()[destination->getPiece()] + Field::row_names()[dest_pos.second] +
               QString::number(dest_pos.first + 1);
    return rev_alg;
}

void Move::execute(Field *destination,
                   Board *b) { // executes move from last field clicked on board to destination field and fills in history table with correct move
    if (destination->isSelected()) {
        Field *origin = b->last_clicked;
        QString rev_alg = Move::reversible_algebraic(origin, destination);
        // move the piece here
        destination->changeIcon(origin->getPiece(), origin->getPieceColor(),
                                destination->isSelected());
        origin->changeIcon("", "", origin->isSelected());
        // selection status stays the same here, they all get deselected below
        // at this point the piece has moved, so actions where origin was used before are now performed on destination
        if (destination->getPiece() == "pawn") {
            std::vector<Field *> promoting_fields;
            int opponent_pawn, last_row;
            std::pair<int, int> two_forward;
            if (destination->getPieceColor() == "white") {
                opponent_pawn = -1;
                promoting_fields = b->white_promoting;
                last_row = 7;
                two_forward = {3, 1};
            } else {
                opponent_pawn = 1;
                promoting_fields = b->black_promoting;
                last_row = 0;
                two_forward = {4, 6};
            }
            std::pair<int, int> p1 = destination->getPosition();
            Field *epv = b->en_passant_vulnerable;
            if (epv == (*b)[std::make_pair(p1.first + opponent_pawn, p1.second)] && epv->getPieceColor() != "") {
                // take the en passant captured pawn off the board
                epv->changeIcon("", "", epv->isSelected());
                // change the notation from a simple move to a capture + notation e.p. for en passant
                QStringList l = rev_alg.split(" - ");
                rev_alg = l[0] + " x " + l[1] + " e.p.";
            } else if (p1.first == last_row) {
                // this else if for promoting might look a bit lost between the if and the else for en passant,
                // but since these moves cannot possibly occur at the same time, it is ok for them to be here together
                b->promoting = true;
                b->promoting_field = destination;
                for (auto f : promoting_fields) {
                    f->setVisible(true);
                }
            } else { // check if this pawn can be captured by en passant in the next turn
                std::pair<int, int> p2 = origin->getPosition();
                if (p1.first == two_forward.first &&
                    p2.first == two_forward.second) { // if pawn moved two fields forward
                    b->en_passant_possible = true;
                    b->en_passant_vulnerable = destination;
                }
            }
        } else if (destination->getPiece() == "king") {
            if (destination->getPieceColor() == "white") {
                b->white_king_position = destination->getPosition(); // we need to keep track of the king position for checking if it is in check
                b->white_king_moved = true; // we need to keep track of the king movement for the castling requirements
            } else {
                b->black_king_position = destination->getPosition(); // we need to keep track of the king position for checking if it is in check
                b->black_king_moved = true; // we need to keep track of the king movement for the castling requirements
            }
            std::pair<int, int> empos = destination->getPosition();
            if (std::abs(origin->getPosition().second - empos.second) == 2) {
                Field *rook_from, *rook_to;
                if (empos.second == 6) { // castling king-side
                    rook_from = (*b)[std::make_pair(empos.first, 7)];
                    rook_to = (*b)[std::make_pair(empos.first, 5)];
                    rev_alg = "O-O";
                } else { // castling queen-side
                    rook_from = (*b)[std::make_pair(empos.first, 0)];
                    rook_to = (*b)[std::make_pair(empos.first, 3)];
                    rev_alg = "O-O-O";
                }
                rook_to->changeIcon(rook_from->getPiece(), rook_from->getPieceColor(), rook_to->isSelected());
                rook_from->changeIcon("", "", rook_from->isSelected());
            }
        } else if (destination->getPiece() ==
                   "rook") { // we need to keep track of the rook movement for the castling requirements
            if (origin->getPosition() == std::make_pair(0, 0)) {
                b->white_rook_left_moved = true;
            } else if (origin->getPosition() == std::make_pair(0, 7)) {
                b->white_rook_right_moved = true;
            } else if (origin->getPosition() == std::make_pair(7, 0)) {
                b->black_rook_left_moved = true;
            } else if (origin->getPosition() == std::make_pair(7, 7)) {
                b->black_rook_right_moved = true;
            }
        }
        int col;
        std::pair<int, int> king_position;
        if (b->turn == "white") {
            col = 0;
            b->history->insertRow(b->turn_number);
            b->turn_number += 1;
            b->history->resizeRowsToContents();
            king_position = b->black_king_position;
        } else {
            col = 1;
            king_position = b->white_king_position;
        }
        if (!b->promoting) {
            b->switch_turn(); // turn is switched here, everything below this uses the new turn color
            if (b->result->isVisible() && b->result->text() != "Stalemate.") {
                rev_alg += "#";
            } else if (b->under_attack(king_position, b->turn)) {
                rev_alg += "+";
            }
        }
        auto *x = new QTableWidgetItem(rev_alg);
        x->setTextAlignment(Qt::AlignCenter);
        b->history->setItem(b->turn_number - 1, col, x);
        if (col == 0) {
            b->history->scrollToItem(x, QAbstractItemView::PositionAtBottom);
        }
    }
    for (auto i : b->selected) { // deselect everything
        i->changeSelection(); // keeping a list of selected fields is faster than going over all fields to see if they are selected or not
    }
    b->selected.clear();
    if (b->en_passant_possible && b->en_passant_vulnerable->getPieceColor() ==
                                  b->turn) { // only the second check is really necessary, but the first makes sure that this->en_passant_vulnerable exists
        b->en_passant_possible = false; // this can only be true for the duration of one turn, so after one move it is set to false
    }
}

void Move::revert(const QString &rev_alg) {
    QStringList l = rev_alg.split(" - ");
}

std::map<QString, QString> Move::piece_to_letter() {
    return {{"queen",  "Q"},
            {"king",   "K"},
            {"rook",   "R"},
            {"knight", "N"},
            {"bishop", "B"},
            {"pawn",   " "}};
}
