#ifndef CHESS_ONLINE_H
#define CHESS_ONLINE_H

#include <QTcpServer>
#include <QTcpSocket>
#include "gameMode.h"

class Online : public GameMode {
Q_OBJECT
public:
    ~Online() override;

    virtual void connection() = 0;

protected:
    QTcpSocket *socket = nullptr;
    QString store_move = "";

    void getReply(const QString &inReply, const QString &repeat);

    void sendCommand(const QString &command);
};


#endif //CHESS_ONLINE_H
