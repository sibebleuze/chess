#ifndef CHESS_SERVER_H
#define CHESS_SERVER_H

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

    void sendMove(const QString &move);

    void receiveMove();

signals:

    void chessError(int exitcode);

private:
    QTcpServer *server;
    QTcpSocket *socket = nullptr;
    Game *game;

    QString last_reply;
    QString store_move = "";

    bool getReply(const QString &inReply, int cycles = 300);

    void sendCommand(const QString &command);
};


#endif //CHESS_SERVER_H
