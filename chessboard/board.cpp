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

namespace board {
    std::vector<Field *> getRookMoves(std::pair<int, int> position, Board *b) {
        std::vector<Field *> possible_moves;
        Field *f = (*b)[position];
        possible_moves.push_back(f);
        return possible_moves;
    }

    std::vector<Field *> getKnightMoves(std::pair<int, int> position, Board *b) {
        std::vector<Field *> possible_moves;
        Field *f = (*b)[position];
        possible_moves.push_back(f);
        return possible_moves;
    }

    std::vector<Field *> getBishopMoves(std::pair<int, int> position, Board *b) {
        std::vector<Field *> possible_moves;
        Field *f = (*b)[position];
        possible_moves.push_back(f);
        return possible_moves;
    }

    std::vector<Field *> getQueenMoves(std::pair<int, int> position, Board *b) {
        std::vector<Field *> possible_moves;
        Field *f = (*b)[position];
        possible_moves.push_back(f);
        return possible_moves;
    }

    std::vector<Field *> getKingMoves(std::pair<int, int> position, Board *b) {
        std::vector<Field *> possible_moves;
        Field *f = (*b)[position];
        possible_moves.push_back(f);
        return possible_moves;
    }

    std::vector<Field *> getPawnMoves(std::pair<int, int> position, Board *b) {
        std::vector<Field *> possible_moves;
        Field *f = (*b)[position];
        possible_moves.push_back(f);
        return possible_moves;
    }

    std::vector<Field *> getEmptyMoves(std::pair<int, int> position, Board *b) {
        std::vector<Field *> possible_moves;
        Field *f = (*b)[position];
        possible_moves.push_back(f);
        return possible_moves;
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

std::map<QString, std::vector<Field *>(*)(std::pair<int, int>, Board *)> Board::pieces() {
    return {
            {"rook",   board::getRookMoves},
            {"knight", board::getKnightMoves},
            {"bishop", board::getBishopMoves},
            {"queen",  board::getQueenMoves},
            {"king",   board::getKingMoves},
            {"pawn",   board::getPawnMoves},
            {"",       board::getEmptyMoves}
    }; // map of piece names to move calculator functions
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

std::vector<Field *> Board::get_field_moves(std::pair<int, int> position) {
    Field *f = (*this)[position];
    return Board::pieces()[f->getPiece()](position, this);
}
