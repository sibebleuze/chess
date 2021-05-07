#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include "line.h"
#include "move.h"

class Board : public QObject {
    friend class Move;
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

    void promote();

private:
    QString turn = "white";

    QLabel *game_end;
    std::vector<QLabel *> row_column_nametags;

    bool en_passant_possible = false;
    // these are not necessarily used and are thus not initialised in the constructor, nor deleted in the destructor
    Field *en_passant_vulnerable = nullptr;
    Field *last_clicked = nullptr;

    // initial values below are the starting positions of both kings
    std::pair<int, int> white_king_position = std::make_pair(0, 4);
    std::pair<int, int> black_king_position = std::make_pair(7, 4);
    // booleans below keep track of which of the kings and rooks already moved, this is needed for castling
    bool white_king_moved = false;
    bool black_king_moved = false;
    bool white_rook_left_moved = false;
    bool white_rook_right_moved = false;
    bool black_rook_left_moved = false;
    bool black_rook_right_moved = false;

    std::vector<Line *> lines; // will be vector of 8 Lines

    static std::map<QString, int> row_numbers();

    static bool on_board(std::pair<int, int> position, std::pair<int, int> increment);

    std::vector<Field *> selected;


    std::vector<Field *> getKnightMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *>
    getStraightMoves(Field *invoking, std::pair<int, int> position, const std::vector<std::pair<int, int>> &directions);

    std::vector<Field *> getPawnMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *> getKingMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *> get_field_moves(Field *invoking);

    bool under_attack(std::pair<int, int> position, QString &color, std::vector<Field *> move = std::vector<Field *>());

    Field *getKingPosition(QString &color);

    std::vector<Field *> white_promoting;
    std::vector<Field *> black_promoting;
    bool promoting = false;
    Field *promoting_field = nullptr;


    void checkmate();

    QTableWidget *history;
};

#endif //CHESS_BOARD_H
