#include "board.h"

Board::Board(QWidget *mainwidget) {
    for (int i = 0; i < 8; i++) {
        Line *l;
        l = new Line(mainwidget, i);
        for (int j = 1; j < 9; j++) {
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

Field *Board::operator[](QString name) { // overload bracket operator, chess notation has fields a1 -> h8
    // if e.g. name = 'a1',
    int row = this->row_numbers[name.left(1)]; // then row = 0
    int column = name.right(1).toInt(); // and column = 1
    return (*this->lines[row])[column];
}

void Board::field_clicked() {
    Field *emitting = (Field *) (QObject::sender());
    emitting->change_selection();
}
