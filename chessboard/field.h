#ifndef CHESS_FIELD_H
#define CHESS_FIELD_H

#include <QIcon>
#include <QPushButton>

class Field : public QPushButton {
public:
    Field(QWidget *mainwidget,
          int linenumber, int position, // position of the field on the board
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
    // below are some maps that are constant but needed to be initialized somewhere;
    // since they will only be used in the Field constructor, this seemed like a good place
    std::map<int, QString> row_names = {{0, "a"},
                                        {1, "b"},
                                        {2, "c"},
                                        {3, "d"},
                                        {4, "e"},
                                        {5, "f"},
                                        {6, "g"},
                                        {7, "h"}}; // map of linenumbers to row names
};


#endif //CHESS_FIELD_H
