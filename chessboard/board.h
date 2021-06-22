#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include <QLabel>
#include "field.h"
#include "chessObject.h"

class Board : public ChessObject {
Q_OBJECT
public:
    Board(QWidget *mainwidget, int x_offset, int y_offset, int field_side);

    ~Board() override;

    Field *operator[](std::pair<int, int> position);

    Field *operator[](const QString &name);

    std::vector<Field *> getPromoting(const QString &color);

    static bool onBoard(std::pair<int, int> position, std::pair<int, int> increment);

    Field *getKingPosition(QString &color);

    static std::map<QString, int> column_numbers();

private:
    std::vector<std::vector<Field *>> lines; // will be vector of 8 'lines' of fields
    std::vector<QLabel *> row_column_nametags;
    std::vector<Field *> white_promoting;
    std::vector<Field *> black_promoting;
};

#endif //CHESS_BOARD_H
