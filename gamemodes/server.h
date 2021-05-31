#ifndef CHESS_SERVER_H
#define CHESS_SERVER_H

#include <limits>
#include <QTcpServer>
#include <QTcpSocket>
#include "../gamecontrol/game.h"

class Server : public QObject {
Q_OBJECT
public:
    explicit Server(QWidget *mainwidget, int port = 16534);

    ~Server() override;

public slots:

    void connection();

    void errorHandler(int exitcode);

    void clientMove(const QString &move);

signals:

    void chessError(int exitcode);

private:
    QTcpServer *server;
    QTcpSocket *socket = nullptr;
    Game *game;
    QString store_move = "";

    QString last_reply = "";

    void getReply(const QString &inReply, const QString &repeat);

    void sendCommand(const QString &command);
};


#endif //CHESS_SERVER_H
