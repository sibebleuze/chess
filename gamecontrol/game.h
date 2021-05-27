#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <QTableWidget>
#include <QHeaderView>
#include "history.h"
#include "../chessboard/board.h"

class Game : public QObject {
Q_OBJECT
public:
    // Clang-Tidy: Constructors that are callable with a single argument must
    // be marked explicit to avoid unintentional implicit conversions
    explicit Game(QWidget *mainwidget, const QString &player_color = "", int x_offset = 100, int y_offset = 500,
                  int field_side = 50);

    // Clang-Tidy: Annotate this function with 'override' or (rarely) 'final'
    ~Game() override;

    void switchTurn();

    static QString reversibleAlgebraic(Field *origin, Field *destination);

    static std::map<QString, QString> piece_to_letter();

    static QString otherColor(const QString &color);

    QStringList getHistory(QString color);

    void executeExternal(const QString &origin, const QString &destination, const QString &promote_piece);

public slots:

    void fieldClicked(bool control = false);

    void promote();

signals:

    void lockedTurn();

private:
    Board *board;

    QString turn = "white";

    QLabel *game_end;

    QString locked;

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

    std::vector<Field *> selected;

    std::vector<Field *> getKnightMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *>
    getStraightMoves(Field *invoking, std::pair<int, int> position, const std::vector<std::pair<int, int>> &directions);

    std::vector<Field *> getPawnMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *> getKingMoves(Field *invoking, std::pair<int, int> position);

    std::vector<Field *> getFieldMoves(Field *invoking);

    bool underAttack(std::pair<int, int> position, QString &color, std::vector<Field *> move = std::vector<Field *>());

    bool promoting = false;
    Field *promoting_field = nullptr;

    void checkmate();

    History *history;

    void execute(Field *destination);
};


#endif //CHESS_GAME_H
