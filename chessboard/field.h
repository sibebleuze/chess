#ifndef CHESS_FIELD_H
#define CHESS_FIELD_H

#include <QIcon>
#include <QPushButton>

class Field : public QPushButton {
public:
    Field(QWidget *mainwidget,
          int linenumber, int position, // position of the field on the board (8x8)
          int x_offset, int y_offset); // x and y coordinate of bottom left corner of board (pixels)

    void change_icon(QString piece = "", QString piece_color = "", bool select = false);

    void change_selection();
private:
    QString field_color;
    QString piece;
    QString piece_color;
    bool selected = false;
};


#endif //CHESS_FIELD_H
