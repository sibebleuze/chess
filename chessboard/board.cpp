#include "board.h"

Board::Board(QWidget *mainwidget, int x_offset, int y_offset) {
    this->parent = mainwidget;
    this->bottomleft = std::make_pair(x_offset, y_offset);
    for (int i = 0; i < 8; i++) {
        Line *l;
        l = new Line(mainwidget, i, x_offset, y_offset);
        for (int j = 0; j < 8; j++) {
            QObject::connect((*l)[j], &QPushButton::clicked, this, &Board::field_clicked);
        }
        this->lines.push_back(l);
    }
    for (int i = 0; i < 4; i++) {
        Field *g, *h;
        g = new Field(mainwidget, 0, i, x_offset + 425 - 50 * i, y_offset - 100 - 50 * i);
        h = new Field(mainwidget, 7, i, x_offset + 425 - 50 * i, y_offset + 250 - 50 * i);
        QObject::connect(g, &QPushButton::clicked, this, &Board::field_clicked);
        QObject::connect(h, &QPushButton::clicked, this, &Board::field_clicked);
        g->setVisible(false);
        h->setVisible(false);
        this->white_promoting.push_back(g);
        this->black_promoting.push_back(h);
    }
}

Board::~Board() {
    for (int i = 0; i < 8; i++) {
        delete this->lines.back();
        this->lines.pop_back();
    }
    delete this->result;
    for (int i = 0; i < 4; i++) {
        delete this->white_promoting.back();
        this->white_promoting.pop_back();
        delete this->black_promoting.back();
        this->black_promoting.pop_back();
    }
}

std::map<QString, int> Board::row_numbers() {
    return {{"a", 0},
            {"b", 1},
            {"c", 2},
            {"d", 3},
            {"e", 4},
            {"f", 5},
            {"g", 6},
            {"h", 7}}; // map of row names to line numbers
}

Field *Board::operator[](QString &name) { // overload bracket operator, chess notation has fields a1 -> h8
    // if e.g. name = 'a1',
    int row = Board::row_numbers()[name.left(1)]; // then row = 0
    int column = name.rightRef(1).toInt() - 1; // and column = 0
    return (*this->lines[row])[column]; // and then this will return (a pointer to) the bottom left field of the board
} // TODO: check if this is ever used, if not, dismiss

Field *Board::operator[](std::pair<int, int> position) {
    return (*this->lines[position.first])[position.second];
}

void Board::field_clicked() {
    // the sender will always be a Field, and since we need to apply Field methods to it, it needs to be cast to a Field here
    auto *emitting = (Field *) (QObject::sender());
    if (this->promoting) {
        std::vector<Field *> promoting_fields;
        if (emitting->getPieceColor() == "white") {
            promoting_fields = this->white_promoting;
        } else if (emitting->getPieceColor() == "black") {
            promoting_fields = this->black_promoting;
        } else {
            return; // this field is empty and is certainly not a field indicating what piece the player wants to promote to
        }
        if (std::find(promoting_fields.begin(), promoting_fields.end(), emitting) != promoting_fields.end()) {
            this->promoting_field->changeIcon(emitting->getPiece(), emitting->getPieceColor(),
                                              this->promoting_field->isSelected());
            this->promoting = false;
            for (auto f : promoting_fields) {
                f->setVisible(false);
            }
        }
    } else if (!this->selected.empty()) {
        if (emitting->isSelected()) {
            // move the piece here
            emitting->changeIcon(this->last_clicked->getPiece(), this->last_clicked->getPieceColor(),
                                 emitting->isSelected());
            this->last_clicked->changeIcon("", "", this->last_clicked->isSelected());
            // selection status stays the same here, they all get deselected below
            // TODO: pawns promoting
            // at this point the piece has moved, so actions where this->last_clicked was used before are now performed on emitting
            if (emitting->getPiece() == "pawn") { // take the en passant captured pawn off the board
                std::vector<Field *> promoting_fields;
                int opponent_pawn, last_row;
                if (emitting->getPieceColor() == "white") {
                    opponent_pawn = -1;
                    promoting_fields = this->white_promoting;
                    last_row = 7;
                } else {
                    opponent_pawn = 1;
                    promoting_fields = this->black_promoting;
                    last_row = 0;
                }
                std::pair<int, int> p1 = emitting->getPosition();
                if (this->en_passant_vulnerable ==
                    (*this)[std::make_pair(p1.first + opponent_pawn, p1.second)]) {
                    this->en_passant_vulnerable->changeIcon("", "", this->en_passant_vulnerable->isSelected());
                } else if (p1.first == last_row) {
                    // this else if for promoting might look a bit lost between the if and the else for en passant,
                    // but since these moves cannot possibly occur at the same time, it is ok for them to be here together
                    this->promoting = true;
                    this->promoting_field = emitting;
                    for (auto f : promoting_fields) {
                        f->setVisible(true);
                    }
                } else { // check if this pawn can be captured by en passant in the next turn
                    std::pair<int, int> p2 = this->last_clicked->getPosition();
                    if ((p1.first == 3 && p2.first == 1) ||
                        (p1.first == 4 && p2.first == 6)) { // if pawn moved two fields forward
                        this->en_passant_possible = true;
                        this->en_passant_vulnerable = emitting;
                    }
                }
            } else if (emitting->getPiece() == "king") {
                if (emitting->getPieceColor() == "white") {
                    this->white_king_position = emitting->getPosition(); // we need to keep track of the king position for checking if it is in check
                    this->white_king_moved = true; // we need to keep track of the king movement for the castling requirements
                } else {
                    this->black_king_position = emitting->getPosition(); // we need to keep track of the king position for checking if it is in check
                    this->black_king_moved = true; // we need to keep track of the king movement for the castling requirements
                }
                std::pair<int, int> empos = emitting->getPosition();
                if (std::abs(this->last_clicked->getPosition().second - empos.second) == 2) {
                    Field *rook_from, *rook_to;
                    if (empos.second == 6) { // castling king-side
                        rook_from = (*this)[std::make_pair(empos.first, 7)];
                        rook_to = (*this)[std::make_pair(empos.first, 5)];
                    } else { // castling queen-side
                        rook_from = (*this)[std::make_pair(empos.first, 0)];
                        rook_to = (*this)[std::make_pair(empos.first, 3)];
                    }
                    rook_to->changeIcon(rook_from->getPiece(), rook_from->getPieceColor(), rook_to->isSelected());
                    rook_from->changeIcon("", "", rook_from->isSelected());
                }
            } else if (emitting->getPiece() ==
                       "rook") { // we need to keep track of the rook movement for the castling requirements
                if (this->last_clicked->getPosition() == std::make_pair(0, 0)) {
                    this->white_rook_left_moved = true;
                } else if (this->last_clicked->getPosition() == std::make_pair(0, 7)) {
                    this->white_rook_right_moved = true;
                } else if (this->last_clicked->getPosition() == std::make_pair(7, 0)) {
                    this->black_rook_left_moved = true;
                } else if (this->last_clicked->getPosition() == std::make_pair(7, 7)) {
                    this->black_rook_right_moved = true;
                }
            }
            this->switch_turn();
        }
        for (auto i : this->selected) { // deselect everything
            i->changeSelection(); // keeping a list of selected fields is faster than going over all fields to see if they are selected or not
        }
        this->selected.clear();
        if (this->en_passant_possible && this->en_passant_vulnerable->getPieceColor() ==
                                         this->turn) { // only the second check is really necessary, but the first makes sure that this->en_passant_vulnerable exists
            this->en_passant_possible = false; // this can only be true for the duration of one turn, so after one move it is set to false
        }
    } else {
        if (emitting->getPieceColor() == this->turn) {
            std::vector<Field *> all_moves = this->get_field_moves(emitting);
            std::vector<Field *> possible_moves;
            for (auto i : all_moves) {
                std::vector<Field *> mv;
                mv.push_back(emitting);
                mv.push_back(i);
                std::pair<int, int> king_position;
                QString pc = emitting->getPieceColor();
                if (emitting->getPiece() == "king") {
                    king_position = std::make_pair(-1, -1);
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
                        } else {
                            king_pos = this->black_king_position;
                            king_moved = this->black_king_moved;
                            row = 7;
                        }
                        if (!this->under_attack(king_pos, this->turn)) {
                            if (!king_moved) {
                                std::pair<int, int> pos1 = std::make_pair(row, 3);
                                std::pair<int, int> pos2 = std::make_pair(row, 2);
                                std::pair<int, int> pos3 = std::make_pair(row, 5);
                                std::pair<int, int> pos4 = std::make_pair(row, 6);
                                // if the chosen move is left castling and the appropriate fields are not under attack
                                if ((i->getPosition().second == pos2.second &&
                                     !(this->under_attack(pos1, this->turn) ||
                                       this->under_attack(pos2, this->turn))) || // or
                                    // if the chosen move is right castling and the appropriate fields are not under attack
                                    (i->getPosition().second == pos4.second &&
                                     !(this->under_attack(pos3, this->turn) ||
                                       this->under_attack(pos4, this->turn)))) {
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
            if (!possible_moves.empty()) {
                this->last_clicked = emitting;
            }
        }
    }
}

bool Board::on_board(std::pair<int, int> position, std::pair<int, int> increment) {
    return 0 <= position.first + increment.first && position.first + increment.first < 8 &&
           0 <= position.second + increment.second && position.second + increment.second < 8;
} // checks if a move increment from a certain position is still on the board or not

std::vector<Field *>
Board::getStraightMoves(Field *invoking, std::pair<int, int> position,
                        const std::vector<std::pair<int, int>> &directions) {
    std::vector<Field *> possible_moves;
    for (auto i : directions) {
        int j = 1;
        Field *f;
        if (Board::on_board(position, i)) {
            do {
                f = (*this)[std::make_pair(position.first + j * i.first, position.second + j * i.second)];
                if (f->getPieceColor() != invoking->getPieceColor()) { // can't 'capture' own pieces
                    possible_moves.push_back(f);
                }
                j += 1;
            } while (f->getPiece() == "" && Board::on_board(position, std::make_pair(j * i.first, j * i.second)));
        }
    }
    return possible_moves;
}

std::vector<Field *> Board::getKnightMoves(Field *invoking, std::pair<int, int> position) {
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
            Field *f = (*this)[std::make_pair(position.first + i.first, position.second + i.second)];
            if (f->getPieceColor() != invoking->getPieceColor()) { // can't 'capture' own pieces
                possible_moves.push_back(f);
            }
        }
    }
    return possible_moves;
}

std::vector<Field *> Board::getKingMoves(Field *invoking, std::pair<int, int> position) {
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
            Field *f = (*this)[std::make_pair(position.first + i.first, position.second + i.second)];
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
        std::pair<int, int> pos1 = std::make_pair(row, 3);
        std::pair<int, int> pos2 = std::make_pair(row, 2);
        std::pair<int, int> pos3 = std::make_pair(row, 5);
        std::pair<int, int> pos4 = std::make_pair(row, 6);
        if (!rook_left_moved && (*this)[pos1]->getPiece() == "" && (*this)[pos2]->getPiece() == "") {
            Field *f = (*this)[pos2];
            possible_moves.push_back(f);
        }
        if (!rook_right_moved && (*this)[pos3]->getPiece() == "" && (*this)[pos4]->getPiece() == "") {
            Field *f = (*this)[pos4];
            possible_moves.push_back(f);
        }
    }
    return possible_moves;
}

std::vector<Field *> Board::getPawnMoves(Field *invoking, std::pair<int, int> position) {
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
        Field *f = (*this)[std::make_pair(position.first + one_forward.first, position.second)];
        if (f->getPiece() == "") {
            possible_moves.push_back(f);
            if (position.first == start_row) { // pawn can move forward 2 fields from starting position
                Field *g = (*this)[std::make_pair(position.first + two_forward.first, position.second)];
                if (g->getPiece() == "") {
                    possible_moves.push_back(g);
                }
            }
        }
    }
    for (auto i : pawn_capture) {
        if (Board::on_board(position, i)) {
            Field *f = (*this)[std::make_pair(position.first + i.first, position.second + i.second)];
            if (f->getPieceColor() != invoking->getPieceColor() && f->getPieceColor() != "") {
                possible_moves.push_back(f);
            } else if (this->en_passant_possible) { // else if, because this can only lead to a correct move if the previous one wasn't true
                Field *g = (*this)[std::make_pair(position.first + i.first + en_passant.first,
                                                  position.second + i.second + en_passant.second)];
                if (g == this->en_passant_vulnerable) {
                    possible_moves.push_back(f);
                }
            }
        }
    }
    return possible_moves;
}

std::vector<Field *> Board::get_field_moves(Field *invoking) {
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

void Board::switch_turn() {
    if (this->turn == "white") {
        this->turn = "black";
    } else {
        this->turn = "white";
    }
    this->checkmate();
}

bool Board::under_attack(std::pair<int, int> position, QString &color, std::vector<Field *> move) {
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
        attacked = this->getKingPosition(color);
    } else { // otherwise we just assume the correct position was given in the function arguments
        attacked = (*this)[position];
    }
    std::vector<Field *> enemy_moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Field *f = (*this)[std::make_pair(i, j)];
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

Field *Board::getKingPosition(QString &color) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Field *f = (*this)[std::make_pair(i, j)];
            if (f->getPieceColor() == color && f->getPiece() == "king") {
                return f; // exactly one of these exists, so if we found it, we're sure it's the only one and we can return immediately
            }
        }
    }
    exit(KING_MISSING);
}

void Board::checkmate() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Field *f = (*this)[std::make_pair(i, j)];
            if (f->getPieceColor() == this->turn) {
                std::vector<Field *> all_moves = this->get_field_moves(f);
                std::vector<Field *> possible_moves;
                for (auto k : all_moves) {
                    std::vector<Field *> mv;
                    mv.push_back(f);
                    mv.push_back(k);
                    std::pair<int, int> king_position;
                    QString pc = f->getPieceColor();
                    if (f->getPiece() == "king") {
                        king_position = std::make_pair(-1, -1);
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
                        possible_moves.push_back(k);
                    }
                }
                if (!possible_moves.empty()) {
                    return; // the player is not out of moves yet, so there is no checkmate at this point
                }
            }
        }
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
    QString result_text;
    if (this->under_attack(king_position, this->turn)) { // king is in check with no moves left: checkmate
        if (this->turn == "white") {
            result_text = "Checkmate. Black wins.";
        } else {
            result_text = "Checkmate. White wins.";
        }
    } else { // no moves left but the king is not in check: stalemate
        result_text = "Stalemate.";
    }
    this->result = new QLabel(this->parent);
    this->result->setObjectName(QString::fromUtf8("result"));
    this->result->setGeometry(QRect(this->bottomleft.first, this->bottomleft.second + 30, 400, 30));
    this->result->setText(result_text);
    this->result->show();
}
