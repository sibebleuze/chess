#include "game.h"

Game::Game(QWidget *mainwidget, int x_offset, int y_offset) {
    this->b = new Board(mainwidget, x_offset, y_offset);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            QObject::connect((*this->b)[std::make_pair(i, j)], &QPushButton::clicked, this, &Game::field_clicked);
        }
    }
    for (auto color : {"white", "black"}) {
        for (auto f : this->b->get_promoting(color)) {
            QObject::connect(f, &QPushButton::clicked, this, &Game::promote);
        }
    }
    QFont fnt;
    this->game_end = new QLabel(mainwidget);
    this->game_end->setObjectName(QString::fromUtf8("result"));
    this->game_end->setGeometry(x_offset, y_offset + int(3 / 5.0 * Field::side), 8 * Field::side,
                                int(3 / 5.0 * Field::side));
    this->game_end->setText("Game over. Close the program and restart it to start a new game.");
    this->game_end->hide();
    this->history = new QTableWidget(1, 2, mainwidget);
    this->history->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->history->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->history->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fnt = this->history->font();
    fnt.setPixelSize(Field::side / 3);
    this->history->setFont(fnt);
    this->history->setHorizontalHeaderLabels({"white", "black"});
    for (int i = 0; i < 2; i++) {
        QTableWidgetItem *hdr = this->history->horizontalHeaderItem(i);
        fnt = hdr->font();
        fnt.setPixelSize(Field::side / 3);
        hdr->setFont(fnt);
        this->history->setHorizontalHeaderItem(i, hdr);
        this->history->setColumnWidth(i, int(2.5 * Field::side));
    }
    this->history->setGeometry(x_offset + 10 * Field::side, // next to the board at the right side
                               y_offset - 8 * Field::side, // on the same height as the board
                               23 + this->history->verticalHeader()->width() + 5 * Field::side,
            // two columns, a vertical header, a vertical scrollbar (21px) and 2 extra px
                               8 * Field::side); // same height as the board
    // 1 row needed for this->history->verticalHeader() to have the correct width, but game should start with zero rows
    this->history->setRowCount(0);
    this->history->show();
}

Game::~Game() {
    delete this->b;
    delete this->game_end;
    delete this->history;
}

void Game::field_clicked() {
    // the sender will always be a Field, and since we need to apply Field methods to it, it needs to be cast to a Field here
    auto *emitting = (Field *) (QObject::sender());
    if (this->promoting) {
        return; // while promoting, no other field can be interacted with
    } else if (!this->selected.empty()) {
        if (emitting->isSelected() &&
            std::find(this->selected.begin(), this->selected.end(), emitting) != this->selected.end()) {
            this->execute(emitting);
        }
        for (auto i : this->selected) { // deselect everything
            i->changeSelection(); // keeping a list of selected fields is faster than going over all fields to see if they are selected or not
        }
        this->selected.clear();
    } else if (emitting->getPieceColor() == this->turn) { // select all possible moves for this field
        std::vector<Field *> all_moves = this->get_field_moves(emitting);
        std::vector<Field *> possible_moves;
        for (auto i : all_moves) {
            std::vector<Field *> mv;
            mv.push_back(emitting);
            mv.push_back(i);
            std::pair<int, int> king_position;
            QString pc = emitting->getPieceColor();
            if (emitting->getPiece() == "king") {
                king_position = {-1, -1};
            } else {
                if (pc == "white") {
                    king_position = this->white_king_position;
                } else if (pc == "black") {
                    king_position = this->black_king_position;
                } else { // this would mean an empty field has possible moves, this should not ever be possible
                    exit(EMPTY_FIELD_MOVE); // if it does happen the program should crash instead of exhibiting who knows what unpredictable behaviour
                }
            }
            if (!this->under_attack(king_position, pc, mv)) {
                if (emitting->getPiece() == "king" &&
                    std::abs(emitting->getPosition().second - i->getPosition().second) ==
                    2) { // we need to check the remaining castling requirements here
                    std::pair<int, int> king_pos;
                    bool king_moved;
                    int row;
                    if (this->turn == "white") {
                        king_pos = this->white_king_position;
                        king_moved = this->white_king_moved;
                        row = 0;
                    } else if (this->turn == "black") {
                        king_pos = this->black_king_position;
                        king_moved = this->black_king_moved;
                        row = 7;
                    } else {
                        exit(EMPTY_TURN);
                    }
                    if (!this->under_attack(king_pos, this->turn)) {
                        if (!king_moved) {
                            std::pair<int, int> pos1 = {row, 3};
                            std::pair<int, int> pos2 = {row, 2};
                            std::pair<int, int> pos3 = {row, 5};
                            std::pair<int, int> pos4 = {row, 6};
                            if ((i->getPosition().second == pos2.second &&
                                 !(this->under_attack(pos1, this->turn) || this->under_attack(pos2, this->turn)))
                                // if the chosen move is queen side castling and the appropriate fields are not under attack
                                || // or
                                // if the chosen move is king side castling and the appropriate fields are not under attack
                                (i->getPosition().second == pos4.second &&
                                 !(this->under_attack(pos3, this->turn) || this->under_attack(pos4, this->turn)))) {
                                possible_moves.push_back(i);
                            }
                        }
                    }
                } else { // if the king is not castling (or another piece entirely is moving), the checks that happened before this suffice
                    possible_moves.push_back(i);
                }
            }
        }
        for (auto i : possible_moves) {
            i->changeSelection();
            this->selected.push_back(i);
        }
        this->last_clicked = emitting;
    }
}

void Game::promote() {
    // the sender will always be a Field, and since we need to apply Field methods to it, it needs to be cast to a Field here
    auto *emitting = (Field *) (QObject::sender());
    std::vector<Field *> promoting_fields;
    int col;
    std::pair<int, int> king_position;
    if (this->turn == "white") {
        promoting_fields = this->b->get_promoting("white");
        col = 0;
        king_position = this->black_king_position;
    } else if (this->turn == "black") {
        promoting_fields = this->b->get_promoting("black");
        col = 1;
        king_position = this->white_king_position;
    } else {
        exit(EMPTY_TURN);
    }
    if (std::find(promoting_fields.begin(), promoting_fields.end(), emitting) != promoting_fields.end()) {
        this->promoting_field->changeIcon(emitting->getPiece(), emitting->getPieceColor(),
                                          this->promoting_field->isSelected());
        this->promoting = false;
        for (auto f : promoting_fields) {
            f->setVisible(false);
        }
        this->switch_turn(); // turn was explicitly not switched on move, but only here on piece selection
        QTableWidgetItem *x = this->history->item(this->history->rowCount() - 1, col);
        x->setText(x->text() + Game::piece_to_letter()[emitting->getPiece()]);
        if (this->game_end->isVisible()) {
            x->setText(x->text() + "#");
        } else if (this->under_attack(king_position, this->turn)) {
            x->setText(x->text() + "+");
        }
    }
}

std::vector<Field *> Game::getStraightMoves(Field *invoking, std::pair<int, int> position,
                                            const std::vector<std::pair<int, int>> &directions) {
    std::vector<Field *> possible_moves;
    for (auto i : directions) {
        int j = 1;
        Field *f;
        if (Board::on_board(position, i)) {
            do {
                f = (*this->b)[std::make_pair(position.first + j * i.first, position.second + j * i.second)];
                if (f->getPieceColor() != invoking->getPieceColor()) { // can't 'capture' own pieces
                    possible_moves.push_back(f);
                }
                j += 1;
            } while (f->getPiece() == "" && Board::on_board(position, std::make_pair(j * i.first, j * i.second)));
        }
    }
    return possible_moves;
}

std::vector<Field *> Game::getKnightMoves(Field *invoking, std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    std::vector<std::pair<int, int>> knight_moves = {{1,  2},
                                                     {2,  1},
                                                     {-1, 2},
                                                     {-2, 1},
                                                     {1,  -2},
                                                     {2,  -1},
                                                     {-1, -2},
                                                     {-2, -1}}; // all moves a knight could make
    for (auto i : knight_moves) {
        if (Board::on_board(position, i)) {
            Field *f = (*this->b)[std::make_pair(position.first + i.first, position.second + i.second)];
            if (f->getPieceColor() != invoking->getPieceColor()) { // can't 'capture' own pieces
                possible_moves.push_back(f);
            }
        }
    }
    return possible_moves;
}

std::vector<Field *> Game::getKingMoves(Field *invoking, std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    std::vector<std::pair<int, int>> king_moves = {{1,  1},
                                                   {1,  0},
                                                   {1,  -1},
                                                   {0,  1},
                                                   {0,  -1},
                                                   {-1, 1},
                                                   {-1, 0},
                                                   {-1, -1}}; // all moves a king could make (except castling)
    for (auto i : king_moves) {
        if (Board::on_board(position, i)) {
            Field *f = (*this->b)[std::make_pair(position.first + i.first, position.second + i.second)];
            if (f->getPieceColor() != invoking->getPieceColor()) { // can't 'capture' own pieces
                possible_moves.push_back(f);
            }
        }
    }
    bool king_moved, rook_left_moved, rook_right_moved;
    int row;
    if (invoking->getPieceColor() == "white") {
        king_moved = this->white_king_moved;
        rook_left_moved = this->white_rook_left_moved;
        rook_right_moved = this->white_rook_right_moved;
        row = 0;
    } else {
        king_moved = this->black_king_moved;
        rook_left_moved = this->black_rook_left_moved;
        rook_right_moved = this->black_rook_right_moved;
        row = 7;
    }
    if (!king_moved) {
        std::pair<int, int> pos1 = {row, 3};
        std::pair<int, int> pos2 = {row, 2};
        std::pair<int, int> pos3 = {row, 5};
        std::pair<int, int> pos4 = {row, 6};
        if (!rook_left_moved && (*this->b)[pos1]->getPiece() == "" && (*this->b)[pos2]->getPiece() == "") {
            Field *f = (*this->b)[pos2];
            possible_moves.push_back(f);
        }
        if (!rook_right_moved && (*this->b)[pos3]->getPiece() == "" && (*this->b)[pos4]->getPiece() == "") {
            Field *f = (*this->b)[pos4];
            possible_moves.push_back(f);
        }
    }
    return possible_moves;
}

std::vector<Field *> Game::getPawnMoves(Field *invoking, std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    std::pair<int, int> one_forward;
    std::pair<int, int> two_forward;
    std::vector<std::pair<int, int>> pawn_capture;
    int start_row;
    std::pair<int, int> en_passant;
    if (invoking->getPieceColor() == "white") {
        one_forward = {1, 0};
        start_row = 1;
        two_forward = {2, 0};
        pawn_capture = {{1, 1},
                        {1, -1}};
        en_passant = {-1, 0};
    } else {
        one_forward = {-1, 0};
        start_row = 6;
        two_forward = {-2, 0};
        pawn_capture = {{-1, 1},
                        {-1, -1}};
        en_passant = {1, 0};
    }
    if (Board::on_board(position, one_forward)) {
        Field *f = (*this->b)[std::make_pair(position.first + one_forward.first, position.second)];
        if (f->getPiece() == "") {
            possible_moves.push_back(f);
            if (position.first == start_row) { // pawn can move forward 2 fields from starting position
                Field *g = (*this->b)[std::make_pair(position.first + two_forward.first, position.second)];
                if (g->getPiece() == "") {
                    possible_moves.push_back(g);
                }
            }
        }
    }
    for (auto i : pawn_capture) {
        if (Board::on_board(position, i)) {
            Field *f = (*this->b)[std::make_pair(position.first + i.first, position.second + i.second)];
            if (f->getPieceColor() != invoking->getPieceColor() && f->getPieceColor() != "") {
                possible_moves.push_back(f);
            } else if (this->en_passant_possible) { // else if, because this can only lead to a correct move if the previous one wasn't true
                Field *g = (*this->b)[std::make_pair(position.first + i.first + en_passant.first,
                                                     position.second + i.second + en_passant.second)];
                if (g == this->en_passant_vulnerable) {
                    possible_moves.push_back(f);
                }
            }
        }
    }
    return possible_moves;
}

std::vector<Field *> Game::get_field_moves(Field *invoking) {
    std::pair<int, int> position = invoking->getPosition();
    QString p = invoking->getPiece();
    std::vector<Field *> possible_moves;
    if (p == "rook") {
        possible_moves = this->getStraightMoves(invoking, position, {{0,  1},
                                                                     {1,  0},
                                                                     {-1, 0},
                                                                     {0,  -1}});
    } else if (p == "knight") {
        possible_moves = this->getKnightMoves(invoking, position);
    } else if (p == "bishop") {
        possible_moves = this->getStraightMoves(invoking, position, {{1,  1},
                                                                     {1,  -1},
                                                                     {-1, 1},
                                                                     {-1, -1}});
    } else if (p == "queen") {
        possible_moves = this->getStraightMoves(invoking, position, {{0,  1},
                                                                     {1,  0},
                                                                     {-1, 0},
                                                                     {0,  -1},
                                                                     {1,  1},
                                                                     {1,  -1},
                                                                     {-1, 1},
                                                                     {-1, -1}});
    } else if (p == "king") {
        possible_moves = this->getKingMoves(invoking, position);
    } else if (p == "pawn") {
        possible_moves = this->getPawnMoves(invoking, position);
    } // ideally this would be a switch case, but that appears to not work with strings
    return possible_moves;
}

void Game::switch_turn() {
    if (this->turn == "white") {
        this->turn = "black";
    } else if (this->turn == "black") {
        this->turn = "white";
    } else {
        exit(EMPTY_TURN);
    }
    if (this->en_passant_possible && this->en_passant_vulnerable->getPieceColor() ==
                                     this->turn) { // only the second check is really necessary, but the first makes sure that this->en_passant_vulnerable exists
        this->en_passant_possible = false; // this can only be true for the duration of one turn, so after one move it is set to false
    }
    this->checkmate();
}

bool Game::under_attack(std::pair<int, int> position, QString &color, std::vector<Field *> move) {
    Field *from, *to;
    std::pair<QString, QString> to_field; // needed to assure no piece get destroyed in this process
    if (!move.empty() &&
        move.size() == 2) { // quick and dirty way to perform functions below on the field as if this move was made
        from = move[0];
        to = move[1];
        to_field = std::make_pair(to->getPiece(), to->getPieceColor());
        to->changeIcon(from->getPiece(), from->getPieceColor(),
                       to->isSelected());
        from->changeIcon("", "", from->isSelected());
    }
    Field *attacked;
    if (position.first == -1 && position.second == -1) { // if the king moved, we need to find the position here
        attacked = this->b->getKingPosition(color);
    } else { // otherwise we just assume the correct position was given in the function arguments
        attacked = (*this->b)[position];
    }
    std::vector<Field *> enemy_moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Field *f = (*this->b)[std::make_pair(i, j)];
            if (f->getPieceColor() != color && f->getPieceColor() != "") {
                std::vector<Field *> to_add = this->get_field_moves(f);
                enemy_moves.insert(enemy_moves.end(), to_add.begin(), to_add.end());
            }
        }
    }
    if (!move.empty() && move.size() == 2) { // revert the move because it hasn't actually happened yet
        from->changeIcon(to->getPiece(), to->getPieceColor(),
                         from->isSelected());
        to->changeIcon(to_field.first, to_field.second, to->isSelected());
    }
    return std::find(enemy_moves.begin(), enemy_moves.end(), attacked) != enemy_moves.end();
}

void Game::checkmate() {
    std::vector<Field *> sel = this->selected;
    this->selected.clear();
    bool moves_left = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Field *f = (*this->b)[std::make_pair(i, j)];
            f->clicked(); // not just this->get_field_moves, because some moves are calculated only after that
            if (!this->selected.empty()) {
                moves_left = true;
            }
            f->clicked(); // deselect the selected fields
        }
    }
    this->selected = sel;
    if (moves_left) {
        return; // the player is not out of moves yet, so there is no checkmate at this point
    }
    // if this point is reached, the player is out of moves and checkmate or stalemate is reached
    std::pair<int, int> king_position;
    if (this->turn == "white") {
        king_position = this->white_king_position;
    } else if (this->turn == "black") {
        king_position = this->black_king_position;
    } else { // this would mean an empty field has possible moves, this should not ever be possible
        exit(EMPTY_TURN); // if it does happen the program should crash instead of exhibiting who knows what unpredictable behaviour
    }
    QString result_notation;
    if (this->under_attack(king_position, this->turn)) { // king is in check with no moves left: checkmate
        if (this->turn == "white") { // turn has switched already
            result_notation = "0 - 1";
        } else if (this->turn == "black") {
            result_notation = "1 - 0";
        } else {
            exit(EMPTY_TURN);
        }
    } else { // no moves left but the king is not in check: stalemate
        result_notation = "1/2 - 1/2";
    }
    this->game_end->show();
    QStringList l = result_notation.split(" - ");
    this->history->insertRow(this->history->rowCount());
    this->history->resizeRowsToContents();
    for (int i = 0; i < 2; i++) {
        auto *x = new QTableWidgetItem(l[i]);
        x->setTextAlignment(Qt::AlignCenter);
        this->history->setItem(this->history->rowCount() - 1, i, x);
    }
}

std::map<QString, QString> Game::piece_to_letter() {
    return {{"queen",  "Q"},
            {"king",   "K"},
            {"rook",   "R"},
            {"knight", "N"},
            {"bishop", "B"},
            {"pawn",   " "}};
}

QString Game::reversible_algebraic(Field *origin,
                                   Field *destination) { // calculates reversible algebraic notation of move from origin to destination field
    std::pair<int, int> ori_pos = origin->getPosition();
    std::pair<int, int> dest_pos = destination->getPosition();
    QString rev_alg = Game::piece_to_letter()[origin->getPiece()] + Field::row_names()[ori_pos.second] +
                      QString::number(ori_pos.first + 1);
    if (destination->getPiece() != "") {
        rev_alg += " x ";
    } else {
        rev_alg += " - ";
    }
    rev_alg += Game::piece_to_letter()[destination->getPiece()] + Field::row_names()[dest_pos.second] +
               QString::number(dest_pos.first + 1);
    return rev_alg;
}

void Game::execute(Field *destination) {
    // executes move from last field clicked on board to destination field and fills in history table with correct move
    Field *origin = this->last_clicked;
    QString rev_alg = Game::reversible_algebraic(origin, destination);
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
            promoting_fields = this->b->get_promoting("white");
            last_row = 7;
            two_forward = {3, 1};
        } else {
            opponent_pawn = 1;
            promoting_fields = this->b->get_promoting("black");
            last_row = 0;
            two_forward = {4, 6};
        }
        std::pair<int, int> p1 = destination->getPosition();
        Field *epv = this->en_passant_vulnerable;
        if (epv == (*this->b)[std::make_pair(p1.first + opponent_pawn, p1.second)] && epv->getPieceColor() != "") {
            // take the en passant captured pawn off the board
            epv->changeIcon("", "", epv->isSelected());
            // change the notation from a simple move to a capture + notation e.p. for en passant
            QStringList l = rev_alg.split(" - ");
            rev_alg = l[0] + " x " + l[1] + " e.p.";
        } else if (p1.first == last_row) {
            // this else if for promoting might look a bit lost between the if and the else for en passant,
            // but since these moves cannot possibly occur at the same time, it is ok for them to be here together
            this->promoting = true;
            this->promoting_field = destination;
            for (auto f : promoting_fields) {
                f->setVisible(true);
            }
        } else { // check if this pawn can be captured by en passant in the next turn
            std::pair<int, int> p2 = origin->getPosition();
            if (p1.first == two_forward.first &&
                p2.first == two_forward.second) { // if pawn moved two fields forward
                this->en_passant_possible = true;
                this->en_passant_vulnerable = destination;
            }
        }
    } else if (destination->getPiece() == "king") {
        if (destination->getPieceColor() == "white") {
            this->white_king_position = destination->getPosition(); // we need to keep track of the king position for checking if it is in check
            this->white_king_moved = true; // we need to keep track of the king movement for the castling requirements
        } else {
            this->black_king_position = destination->getPosition(); // we need to keep track of the king position for checking if it is in check
            this->black_king_moved = true; // we need to keep track of the king movement for the castling requirements
        }
        std::pair<int, int> empos = destination->getPosition();
        if (std::abs(origin->getPosition().second - empos.second) == 2) {
            Field *rook_from, *rook_to;
            if (empos.second == 6) { // castling king-side
                rook_from = (*this->b)[std::make_pair(empos.first, 7)];
                rook_to = (*this->b)[std::make_pair(empos.first, 5)];
                rev_alg = "O - O";
            } else { // castling queen-side
                rook_from = (*this->b)[std::make_pair(empos.first, 0)];
                rook_to = (*this->b)[std::make_pair(empos.first, 3)];
                rev_alg = "O - O - O";
            }
            rook_to->changeIcon(rook_from->getPiece(), rook_from->getPieceColor(), rook_to->isSelected());
            rook_from->changeIcon("", "", rook_from->isSelected());
        }
    } else if (destination->getPiece() ==
               "rook") { // we need to keep track of the rook movement for the castling requirements
        if (origin->getPosition() == std::make_pair(0, 0)) {
            this->white_rook_left_moved = true;
        } else if (origin->getPosition() == std::make_pair(0, 7)) {
            this->white_rook_right_moved = true;
        } else if (origin->getPosition() == std::make_pair(7, 0)) {
            this->black_rook_left_moved = true;
        } else if (origin->getPosition() == std::make_pair(7, 7)) {
            this->black_rook_right_moved = true;
        }
    }
    int col;
    std::pair<int, int> king_position;
    QTableWidgetItem *x;
    if (this->turn == "white") {
        col = 0;
        king_position = this->black_king_position;
        this->history->insertRow(this->history->rowCount());
        this->history->resizeRowsToContents();
        for (int i = 1; i >= 0; i--) {
            x = new QTableWidgetItem(" ");
            x->setTextAlignment(Qt::AlignCenter);
            this->history->setItem(this->history->rowCount() - 1, i, x);
        }
        this->history->scrollToItem(x, QAbstractItemView::PositionAtBottom);
    } else if (this->turn == "black") {
        col = 1;
        king_position = this->white_king_position;
    } else {
        exit(EMPTY_TURN);
    }
    if (!this->promoting) {
        this->switch_turn(); // turn is switched here, everything below this uses the new turn color
        if (this->game_end->isVisible() && this->history->item(this->history->rowCount() - 1, 0)->text() != "1/2") {
            rev_alg += "#";
        } else if (this->under_attack(king_position, this->turn)) {
            rev_alg += "+";
        }
    }
    int m = 0;
    x = this->history->item(this->history->rowCount() - 1, col);
    if (x->text() != " ") {
        m += 1;
    }
    x = this->history->item(this->history->rowCount() - 1 - m, col);
    x->setText(rev_alg);
}
