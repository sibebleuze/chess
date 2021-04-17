#ifndef CHESS_LINE_H
#define CHESS_LINE_H


#include "field.h"
#include <vector>

class Line {
public:
    Line(QWidget *mainwidget, QString name, int y_offset, QString field_color,
         std::vector<QString> pieces = std::vector<QString>());

    ~Line();

    Field *operator[](int i);

private:
//    QVector<Field *> fields;
    std::vector<Field *> fields; // will be vector of 8 Fields
};


#endif //CHESS_LINE_H
