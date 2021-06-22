#ifndef CHESS_CHESSOBJECT_H
#define CHESS_CHESSOBJECT_H

#include <QObject>
#include "../exit_codes.h"

class ChessObject : public QObject {
Q_OBJECT
public slots:

    void errorHandler(int exitcode);

signals:

    void chessError(int exitcode);
};


#endif //CHESS_CHESSOBJECT_H
