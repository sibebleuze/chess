#ifndef CHESS_FIELD_H
#define CHESS_FIELD_H

#include <QIcon>
#include <QPushButton>
#include <algorithm>
#include <map>
#include "../exit_codes.h"

class Field : public QPushButton {
public:
    Field(QWidget *mainwidget,
          int linenumber, int rownumber, // position of the field on the board (8x8)
          int x_offset, int y_offset, // x and y coordinate of bottom left corner of board (pixels)
          int field_side); // side length of a field

    void changeIcon(QString p = "", QString pc = "", bool select = false);

    void changeSelection();

    QString getPiece();

    QString getPieceColor();

    std::pair<int, int> getPosition();

    bool isSelected() const;

    static std::map<int, QString> row_names();

    static std::map<int, std::vector<QString>> field_colors();

    static std::map<int, QString> piece_colors();

    static std::map<int, std::vector<QString>> pieces();

private:
    QString field_color;
    QString piece;
    QString piece_color;

    std::pair<int, int> position;

    bool selected = false;
};


#endif //CHESS_FIELD_H
