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
    std::vector<Field *> get_field_moves(std::pair<int, int> position);

    static std::map<QString, int> row_numbers();

    bool on_board(std::pair<int, int> position, std::pair<int, int> increment);

    std::vector<Field *> getRookMoves(std::pair<int, int> position);

    std::vector<Field *> getKnightMoves(std::pair<int, int> position);

    std::vector<Field *> getBishopMoves(std::pair<int, int> position);

    std::vector<Field *> getQueenMoves(std::pair<int, int> position);

    std::vector<Field *> getKingMoves(std::pair<int, int> position);

    std::vector<Field *> getPawnMoves(std::pair<int, int> position);

    std::vector<Field *> getEmptyMoves(std::pair<int, int> position);
};


#endif //CHESS_BOARD_H
