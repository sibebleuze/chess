#ifndef CHESS_FIELD_H
#define CHESS_FIELD_H

#include <QIcon>
#include <QPushButton>
#include <algorithm>
#include <map>

class Field : public QPushButton {
public:
    Field(QWidget *mainwidget,
          int linenumber, int position, // position of the field on the board (8x8)
          int x_offset = 100, int = 500); // x and y coordinate of bottom left corner of board (pixels)

    void changeIcon(QString piece = "", QString piece_color = "", bool select = false);

    void changeSelection();

    QString getPiece();

    QString getPieceColor();

    std::pair<int, int> getPosition();

    bool isSelected();

private:
    QString field_color;
    QString piece;
    QString piece_color;
    std::pair<int, int> position;
    bool selected = false;

    static std::map<int, QString> row_names();

    static std::map<int, std::vector<QString>> field_colors();

    static std::map<int, QString> piece_colors();

    static std::map<int, std::vector<QString>> pieces();
};


#endif //CHESS_FIELD_H
