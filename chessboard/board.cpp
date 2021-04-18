
#include "board.h"

Board::Board(QWidget *mainwidget) {
    for (int i = 0; i < 8; i++) {
        Line *l;
        l = new Line(mainwidget, i);
        this->lines.push_back(l);
    }
}

Board::~Board() {
    for (int i = 0; i < 8; i++) {
        delete this->lines.back();
        this->lines.pop_back();
    }
}

Field *Board::operator[](QString name) { // overload bracket operator, chess notation has fields a1 -> h8
    // if e.g. name = 'a1',
    int row = this->row_numbers[name.left(1)]; // then row = 0
    int column = name.right(1).toInt(); // and column = 1
    return (*this->lines[row])[column];
}
