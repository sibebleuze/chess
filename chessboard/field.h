#ifndef CHESS_FIELD_H
#define CHESS_FIELD_H

#include <QIcon>
#include <QPushButton>

class Field {
public:
    Field(int left, int top, QWidget *mainwidget);
    ~Field();
    QPushButton *button;
};


#endif //CHESS_FIELD_H
