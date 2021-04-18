#ifndef CHESS_LINE_H
#define CHESS_LINE_H


#include "field.h"
#include <vector>

class Line {
public:
    Line(QWidget *mainwidget,
         int linenumber,
         int x_offset = 100, // x coordinate of left of board
         int y_offset = 500); // y coordinate of bottom of board

    ~Line();

    Field *operator[](int i);

private:
//    QVector<Field *> fields;
    std::vector<Field *> fields; // will be vector of 8 Fields
};


#endif //CHESS_LINE_H
