#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H


#include "board.h"

class Move {
    friend class Board; // since no instances of this class will be made, only Board can execute these functions
private:
    static QString reversible_algebraic(Field *origin, Field *destination);

    static void execute(Field *destination, Board *b);

    static void revert(const QString &rev_alg);

    static std::map<QString, QString> piece_to_letter();
};


#endif //CHESS_MOVE_H
