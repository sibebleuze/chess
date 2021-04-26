#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include "line.h"
#include <QLabel>

class Board : public QObject {
public:
    // justification of use of explicit: Clang-Tidy: Constructors that are callable with a single argument must be marked explicit to avoid unintentional implicit conversions
    explicit Board(QWidget *mainwidget, int x_offset = 100, int y_offset = 500);

    // justification of use of override: Clang-Tidy: Annotate this function with 'override' or (rarely) 'final'
    ~Board() override;

    Field *operator[](QString &name);

    Field *operator[](std::pair<int, int> position);

    void switch_turn();

public slots:

    void field_clicked();

private:
    QWidget *parent;
    QLabel *result = nullptr; // needs to be initialised, because it will be deleted in the Board destructor
    std::pair<int, int> bottomleft;
    Field *last_clicked = nullptr;
    std::vector<Field *> selected;
    QString turn = "white";
    Field *en_passant_vulnerable = nullptr;
    bool en_passant_possible = false;
    std::pair<int, int> white_king_position = std::make_pair(0, 4);
    std::pair<int, int> black_king_position = std::make_pair(7, 4);
    std::vector<Line *> lines; // will be vector of 8 Lines
    std::vector<Field *> get_field_moves(Field *invoking);

    static std::map<QString, int> row_numbers();

    static bool on_board(std::pair<int, int> position, std::pair<int, int> increment);

    std::vector<Field *> getKnightMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *>
    getStraightMoves(Field *invoking, std::pair<int, int> position, std::vector<std::pair<int, int>> directions);

    std::vector<Field *> getKingMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *> getPawnMoves(Field *invoking, std::pair<int, int> position);

    bool under_attack(std::pair<int, int> position, QString &color, std::vector<Field *> move = std::vector<Field *>());

    Field *getKingPosition(QString &color);

    void checkmate();
};


#endif //CHESS_BOARD_H
