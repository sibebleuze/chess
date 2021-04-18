#ifndef CHESS_FIELD_H
#define CHESS_FIELD_H

#include <QIcon>
#include <QPushButton>

class Field : public QPushButton {
public:
    Field(QWidget *mainwidget,
          QString name, // name of the field, e.g. 'a1'
          int left, int top, // x and y coordinate of top left corner of field/button
          QString field_color, // color of the field: 'light' or 'dark'
          QString piece = "", // which piece, if any, is on the field
          QString piece_color = ""); // which color the piece, if any, on the field has

    void change_icon(QString piece = "", QString piece_color = "", bool select = false);

public slots:

    void change_selection(bool do_emit = true);

signals:

    void select_switch() {};
private:
    QString field_color;
    QString piece;
    QString piece_color;
    bool selected = false;
};


#endif //CHESS_FIELD_H
