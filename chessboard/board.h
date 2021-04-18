#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include "line.h"

class Board {
public:
    Board(QWidget *mainwidget);

    ~Board();
//    Field *operator[](QString name);
private:
    std::vector<Line *> lines; // will be vector of 8 Lines
};


#endif //CHESS_BOARD_H
