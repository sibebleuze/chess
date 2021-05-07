#include "board.h"

Board::Board(QWidget *mainwidget, int x_offset, int y_offset) {
    QFont fnt;
    for (int i = 0; i < 4; i++) {
        Field *g, *h;
        g = new Field(mainwidget, 0, i, x_offset + int((8.5 - i) * Field::side), y_offset - (i + 2) * Field::side);
        h = new Field(mainwidget, 7, i, x_offset + int((8.5 - i) * Field::side), y_offset + (5 - i) * Field::side);
        QObject::connect(g, &QPushButton::clicked, this, &Board::promote);
        QObject::connect(h, &QPushButton::clicked, this, &Board::promote);
        g->setVisible(false);
        h->setVisible(false);
        this->white_promoting.push_back(g);
        this->black_promoting.push_back(h);
        for (int j = 0; j < 2; j++) {
            Line *l;
            l = new Line(mainwidget, 2 * i + j, x_offset, y_offset);
            for (int k = 0; k < 8; k++) {
                QObject::connect((*l)[k], &QPushButton::clicked, this, &Board::field_clicked);
            }
            this->lines.push_back(l);
            QLabel *q, *r;
            q = new QLabel(mainwidget);
            r = new QLabel(mainwidget);
            q->setObjectName("column" + QString::number(2 * i + j));
            r->setObjectName("row" + QString::number(2 * i + j));
            q->setGeometry(x_offset + int(2 / 5.0 * Field::side) + Field::side * (2 * i + j),
                           y_offset,
                           Field::side / 2,
                           Field::side / 2);
            r->setGeometry(x_offset - int(3 / 10.0 * Field::side),
                           y_offset - int(7 / 10.0 * Field::side) - Field::side * (2 * i + j),
                           Field::side / 2,
                           Field::side / 2);
            q->setText(Field::row_names()[2 * i + j]);
            r->setText(QString::number(2 * i + j + 1));
            fnt = q->font();
            fnt.setPixelSize(Field::side / 3);
            q->setFont(fnt);
            fnt = r->font();
            fnt.setPixelSize(Field::side / 3);
            r->setFont(fnt);
            q->show();
            r->show();
            this->row_column_nametags.push_back(q);
            this->row_column_nametags.push_back(r);
        }
    }
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

Board::~Board() {
    for (int i = 0; i < 4; i++) {
        // these first two vectors have a length of 4
        delete this->white_promoting.back();
        this->white_promoting.pop_back();
        delete this->black_promoting.back();
        this->black_promoting.pop_back();
        for (int j = 0; j < 2; j++) {
            // this one has a length of 8
            delete this->lines.back();
            this->lines.pop_back();
            for (int k = 0; k < 2; k++) {
                // and this one has a length of 16
                delete this->row_column_nametags.back();
                this->row_column_nametags.pop_back();
            }
        }
    }
    delete this->game_end;
    delete this->history;
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
} // TODO: check if this is ever used, if not, dismiss (including Board::row_numbers above)

Field *Board::operator[](std::pair<int, int> position) {
    return (*this->lines[position.first])[position.second];
}

void Board::field_clicked() {
    // the sender will always be a Field, and since we need to apply Field methods to it, it needs to be cast to a Field here
    auto *emitting = (Field *) (QObject::sender());
    if (this->promoting) {
        return; // while promoting, no other field can be interacted with
    } else if (!this->selected.empty()) {
        Move::execute(emitting, this);
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
                    } else {
                        king_pos = this->black_king_position;
                        king_moved = this->black_king_moved;
                        row = 7;
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
        std::pair<int, int> pos1 = {row, 3};
        std::pair<int, int> pos2 = {row, 2};
        std::pair<int, int> pos3 = {row, 5};
        std::pair<int, int> pos4 = {row, 6};
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
    QString result_notation;
    if (this->under_attack(king_position, this->turn)) { // king is in check with no moves left: checkmate
        if (this->turn == "white") { // turn has switched already
            result_notation = "0 - 1";
        } else {
            result_notation = "1 - 0";
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

void Board::promote() {
    // the sender will always be a Field, and since we need to apply Field methods to it, it needs to be cast to a Field here
    auto *emitting = (Field *) (QObject::sender());
    std::vector<Field *> promoting_fields;
    int col;
    std::pair<int, int> king_position;
    if (this->turn == "white") {
        promoting_fields = this->white_promoting;
        col = 0;
        king_position = this->black_king_position;
    } else {
        promoting_fields = this->black_promoting;
        col = 1;
        king_position = this->white_king_position;
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
        x->setText(x->text() + Move::piece_to_letter()[emitting->getPiece()]);
        if (this->game_end->isVisible()) {
            x->setText(x->text() + "#");
        } else if (this->under_attack(king_position, this->turn)) {
            x->setText(x->text() + "+");
        }
    }
}
