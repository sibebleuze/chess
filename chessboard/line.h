#ifndef CHESS_LINE_H
#define CHESS_LINE_H


#include <vector>
#include "field.h"

class Line {
public:
    Line(QWidget *mainwidget,
         int linenumber, // chess board has 8 lines/rows, constructor needs to know which one it is
         int x_offset, // x coordinate of left of board
         int y_offset); // y coordinate of bottom of board

    ~Line();

    Field *operator[](int i);

private:
    std::vector<Field *> fields; // will be vector of 8 Fields
};


#endif //CHESS_LINE_H
