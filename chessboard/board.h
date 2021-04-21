#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include "line.h"


class Board : public QObject {
public:
    Board(QWidget *mainwidget);

    ~Board();

    Field *operator[](QString name);

    Field *operator[](std::pair<int, int> position);

public slots:

    void field_clicked();

private:
    std::vector<Line *> lines; // will be vector of 8 Lines
    std::vector<Field *> get_field_moves(Field *invoking);

    static std::map<QString, int> row_numbers();

    bool on_board(std::pair<int, int> position, std::pair<int, int> increment);

    std::vector<Field *> getKnightMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *>
    getStraightMoves(Field *invoking, std::pair<int, int> position, std::vector<std::pair<int, int>> directions);

    std::vector<Field *> getKingMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *> getPawnMoves(Field *invoking, std::pair<int, int> position);
};


#endif //CHESS_BOARD_H
