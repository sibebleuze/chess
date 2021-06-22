#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "history.h"
#include "../chessboard/board.h"

class Game : public ChessObject {
Q_OBJECT
public:
    explicit Game(QWidget *mainwidget, const QString &player_color = "", int x_offset = 100, int y_offset = 500,
                  int field_side = 50);

    ~Game() override;

    void switchTurn();

    static QString reversibleAlgebraic(Field *origin, Field *destination);

    static std::map<QString, QString> piece_to_letter();

    QString otherColor(QString color);

    QStringList getHistory(const QString &color);

    void executeExternal(const QString &move);

    QString revAlgToLongAlg(QString rev_alg, const QString &color);

public slots:

    void fieldClicked(bool control = false);

    void promote();

signals:

    void lockedTurn(QString move);

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

    bool hasMoved(QString color, const QString &query, int to_search = 2);
};


#endif //CHESS_GAME_H
