#include "board.h"

Board::Board(QWidget *mainwidget) {
    for (int i = 0; i < 8; i++) {
        Line *l;
        l = new Line(mainwidget, i);
        for (int j = 0; j < 8; j++) {
            QObject::connect((*l)[j], &QPushButton::clicked, this, &Board::field_clicked);
        }
        this->lines.push_back(l);
    }
}

Board::~Board() {
    for (int i = 0; i < 8; i++) {
        delete this->lines.back();
        this->lines.pop_back();
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

Field *Board::operator[](QString name) { // overload bracket operator, chess notation has fields a1 -> h8
    // if e.g. name = 'a1',
    int row = Board::row_numbers()[name.left(1)]; // then row = 0
    int column = name.right(1).toInt() - 1; // and column = 0
    return (*this->lines[row])[column]; // and then this will return (a pointer to) the bottom left field of the board
} // TODO: check if this is ever used, if not, dismiss

Field *Board::operator[](std::pair<int, int> position) {
    return (*this->lines[position.first])[position.second];
}

void Board::field_clicked() {
    // the sender will always be a Field, and since we need to apply Field methods to it, it needs to be cast to a Field here
    Field *emitting = (Field *) (QObject::sender());
    if (this->selected) {
        this->selected = false;
        std::vector<Field *> to_deselect = this->get_field_moves(this->last_clicked);
        if (emitting->isSelected()) {
            // move the piece here
            emitting->changeIcon(this->last_clicked->getPiece(), this->last_clicked->getPieceColor(),
                                 emitting->isSelected());
            this->last_clicked->changeIcon("", "", this->last_clicked->isSelected());
            // selection status stays the same here, they all get deselected below
            // TODO: pawns promoting, en passant and castling
        }
        for (auto i : to_deselect) { // deselect everything
            i->changeSelection();
        }
    } else {
        if (emitting->getPieceColor() == this->turn) {
            std::vector<Field *> possible_moves = this->get_field_moves(emitting);
            for (auto i : possible_moves) {
                i->changeSelection();
            }
            if (!possible_moves.empty()) {
                this->selected = true;
                this->last_clicked = emitting;
                this->switch_turn();
            }
        }
    }
}

bool Board::on_board(std::pair<int, int> position, std::pair<int, int> increment) {
    return 0 <= position.first + increment.first && position.first + increment.first < 8 &&
           0 <= position.second + increment.second && position.second + increment.second < 8;
} // checks if a move increment from a certain position is still on the board or not

std::vector<Field *>
Board::getStraightMoves(Field *invoking, std::pair<int, int> position, std::vector<std::pair<int, int>> directions) {
    std::vector<Field *> possible_moves;
    for (auto i : directions) {
        int j = 1;
        Field *f;
        if (this->on_board(position, i)) {
            do {
                f = (*this)[std::make_pair(position.first + j * i.first, position.second + j * i.second)];
                if (f->getPieceColor() != invoking->getPieceColor()) { // can't 'capture' own pieces
                    possible_moves.push_back(f);
                }
                j += 1;
            } while (f->getPiece() == "" && this->on_board(position, std::make_pair(j * i.first, j * i.second)));
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
        if (this->on_board(position, i)) {
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
        if (this->on_board(position, i)) {
            Field *f = (*this)[std::make_pair(position.first + i.first, position.second + i.second)];
            if (f->getPieceColor() != invoking->getPieceColor()) { // can't 'capture' own pieces
                possible_moves.push_back(f);
            }
        }
    }
    // TODO: castling
    return possible_moves;
}

std::vector<Field *> Board::getPawnMoves(Field *invoking, std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    std::pair<int, int> one_forward;
    std::pair<int, int> two_forward;
    std::vector<std::pair<int, int>> pawn_capture;
    int start_row;
    if (invoking->getPieceColor() == "white") {
        one_forward = {1, 0};
        start_row = 1;
        two_forward = {2, 0};
        pawn_capture = {{1, 1},
                        {1, -1}};
    } else {
        one_forward = {-1, 0};
        start_row = 6;
        two_forward = {-2, 0};
        pawn_capture = {{-1, 1},
                        {-1, -1}};
    }
    if (this->on_board(position, one_forward)) {
        Field *f = (*this)[std::make_pair(position.first + one_forward.first, position.second)];
        if (f->getPiece() == "") {
            possible_moves.push_back(f);
            if (position.first == start_row) { // pawn can move forward 2 fields from starting position
                Field *f = (*this)[std::make_pair(position.first + two_forward.first, position.second)];
                if (f->getPiece() == "") {
                    possible_moves.push_back(f);
                }
            }
        }
    }
    for (auto i : pawn_capture) {
        if (this->on_board(position, i)) {
            Field *f = (*this)[std::make_pair(position.first + i.first, position.second + i.second)];
            if (f->getPieceColor() != invoking->getPieceColor() && f->getPieceColor() != "") {
                possible_moves.push_back(f);
            }
        }
    }
    // TODO: en passant
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
}
