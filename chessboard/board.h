#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include "line.h"

class Board {
public:
    Board(QWidget *mainwidget);

    ~Board();

    Field *operator[](QString name);

private:
    std::vector<Line *> lines; // will be vector of 8 Lines
    std::map<QString, int> row_numbers = {{"a", 0},
                                          {"b", 1},
                                          {"c", 2},
                                          {"d", 3},
                                          {"e", 4},
                                          {"f", 5},
                                          {"g", 6},
                                          {"h", 7}}; // map of row names to line numbers
};


#endif //CHESS_BOARD_H
