#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include <QLabel>

#include "line.h"

class Board {
public:
    Board(QWidget *mainwidget, int x_offset, int y_offset, int field_side);

    ~Board();

    Field *operator[](std::pair<int, int> position);

    std::vector<Field *> getPromoting(const QString &color);

    static bool onBoard(std::pair<int, int> position, std::pair<int, int> increment);

    Field *getKingPosition(QString &color);

private:
    std::vector<Line *> lines; // will be vector of 8 Lines
    std::vector<QLabel *> row_column_nametags;
    std::vector<Field *> white_promoting;
    std::vector<Field *> black_promoting;
};

#endif //CHESS_BOARD_H
