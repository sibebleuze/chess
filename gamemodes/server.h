#ifndef CHESS_SERVER_H
#define CHESS_SERVER_H

#include "online.h"

class Server : public Online {
Q_OBJECT
public:
    explicit Server(QWidget *mainwidget, int port = 16534);

    ~Server() override;

public slots:

    void connection() override;

    void clientMove(const QString &move);

private:
    QTcpServer *server;
};


#endif //CHESS_SERVER_H
