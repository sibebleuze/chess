#include "chessObject.h"

void ChessObject::errorHandler(int exitcode) {
    emit this->chessError(exitcode);
}
