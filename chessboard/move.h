#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H


#include "field.h"

class Move {
    friend class Board; // since no instances of this class will be made, only Board can execute these functions
private:
    static QString execute(Field *origin, Field *destination);

    static QString reversible_algebraic(Field *origin, Field *destination);

    static void revert(QString rev_alg);

    static std::map<QString, QString> piece_to_letter();
};


#endif //CHESS_MOVE_H
