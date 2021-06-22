#ifndef CHESS_CLIENT_H
#define CHESS_CLIENT_H

#include "online.h"

class Client : public Online {
Q_OBJECT
public:
    Client(QWidget *mainwidget, const QString &server_ip, int server_port = 16534);

    void firstServerMove() override;

public slots:

    void connection() override;

    void reconnect();

    void serverMove(const QString &move);

private:
    QWidget *main;

    QString server_ip;
    int server_port;
};


#endif //CHESS_CLIENT_H
