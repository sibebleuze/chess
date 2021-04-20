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
    std::vector<Field *> possible_moves = this->get_field_moves(emitting->getPosition());
    for (auto i : possible_moves) {
        i->changeSelection();
    }
//    emitting->changeSelection();
}

bool Board::on_board(std::pair<int, int> position, std::pair<int, int> increment) {
    return 0 <= position.first + increment.first && position.first + increment.first < 8 &&
           0 <= position.second + increment.second && position.second + increment.second < 8;
} // checks if a move increment from a certain position is still on the board or not

std::vector<Field *> Board::getRookMoves(std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    Field *f = (*this)[position];
    possible_moves.push_back(f);
    return possible_moves;
}

std::vector<Field *> Board::getKnightMoves(std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    Field *invoking = (*this)[position];
    possible_moves.push_back(invoking);
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

std::vector<Field *> Board::getBishopMoves(std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    Field *f = (*this)[position];
    possible_moves.push_back(f);
    return possible_moves;
}

std::vector<Field *> Board::getQueenMoves(std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    Field *f = (*this)[position];
    possible_moves.push_back(f);
    return possible_moves;
}

std::vector<Field *> Board::getKingMoves(std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    Field *invoking = (*this)[position];
    possible_moves.push_back(invoking);
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
    // castling

    return possible_moves;
}

std::vector<Field *> Board::getPawnMoves(std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    Field *f = (*this)[position];
    possible_moves.push_back(f);
    return possible_moves;
}

std::vector<Field *> Board::getEmptyMoves(std::pair<int, int> position) {
    std::vector<Field *> possible_moves;
    return possible_moves;
}

std::vector<Field *> Board::get_field_moves(std::pair<int, int> position) {
    Field *f = (*this)[position];
    QString p = f->getPiece();
    if (p == "rook") {
        return this->getRookMoves(position);
    } else if (p == "knight") {
        return this->getKnightMoves(position);
    } else if (p == "bishop") {
        return this->getBishopMoves(position);
    } else if (p == "queen") {
        return this->getQueenMoves(position);
    } else if (p == "king") {
        return this->getKingMoves(position);
    } else if (p == "pawn") {
        return this->getPawnMoves(position);
    } else {
        return this->getEmptyMoves(position);
    } // ideally this would be a switch case, but that appears to not work with strings
}
