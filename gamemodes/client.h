#ifndef CHESS_CLIENT_H
#define CHESS_CLIENT_H

#include "../gamecontrol/game.h"

class Client : public QObject {
Q_OBJECT
public:
    Client(QWidget *mainwidget, const QString &server_ip, int server_port = 16534);

    ~Client();

    void firstServerMove();

public slots:

    void connection();

    void reconnect();

    void errorHandler(int exitcode);

    void serverMove(const QString &move);

signals:

    void chessError(int exitcode);

private:
    QWidget *main;
    QTcpSocket *socket;
    Game *game;
    QString store_move = "";

    QString server_ip;
    int server_port;

    QString last_reply = "";

    void getReply(const QString &inReply, const QString &repeat);

    void sendCommand(const QString &command);
};


#endif //CHESS_CLIENT_H
