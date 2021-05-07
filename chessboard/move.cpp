#include "move.h"

QString Move::reversible_algebraic(Field *origin,
                                   Field *destination) { // calculates reversible algebraic notation of move from origin to destination field
    std::pair<int, int> ori_pos = origin->getPosition();
    std::pair<int, int> dest_pos = destination->getPosition();
    QString rev_alg = Move::piece_to_letter()[origin->getPiece()] + Field::row_names()[ori_pos.second] +
                      QString::number(ori_pos.first + 1);
    if (destination->getPiece() != "") {
        rev_alg += " x ";
    } else {
        rev_alg += " - ";
    }
    rev_alg += Move::piece_to_letter()[destination->getPiece()] + Field::row_names()[dest_pos.second] +
               QString::number(dest_pos.first + 1);
    return rev_alg;
}

QString Move::execute(Field *origin,
                      Field *destination) { // executes move from origin to destination field and returns reversible algebraic notation of it
    QString rev_alg = Move::reversible_algebraic(origin, destination);
    // move the piece here
    destination->changeIcon(origin->getPiece(), origin->getPieceColor(), destination->isSelected());
    origin->changeIcon("", "", origin->isSelected());
    return rev_alg;
}

void Move::revert(QString rev_alg) {
    QStringList l = rev_alg.split(" - ");
}

std::map<QString, QString> Move::piece_to_letter() {
    return {{"queen",  "Q"},
            {"king",   "K"},
            {"rook",   "R"},
            {"knight", "N"},
            {"bishop", "B"},
            {"pawn",   " "}};
}
