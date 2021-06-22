#include "server.h"

Server::Server(QWidget *mainwidget, int port) {
    this->server = new QTcpServer(this);
    QObject::connect(this->server, &QTcpServer::newConnection, this, &Server::connection);
    this->server->listen(QHostAddress::AnyIPv4, port);
    this->game = new Game(mainwidget, "white");
    QObject::connect(this->game, &Game::lockedTurn, this, &Server::clientMove);
    QObject::connect(this->game, &Game::chessError, this, &Server::errorHandler);
}

Server::~Server() {
    this->server->close();
    delete this->server;
}

void Server::connection() {
    this->socket = this->server->nextPendingConnection();
    this->sendCommand("isready");
    this->getReply("readyok", "isready");
    if (this->store_move != "") {
        this->clientMove(this->store_move);
    }
}

void Server::clientMove(const QString &move) {
    if (this->socket != nullptr) {
        this->store_move = "";
        QString to_send = this->game->revAlgToLongAlg(move, "white");
        this->sendCommand("move:" + to_send);
        this->getReply("receivedmove:" + move, "move:" + to_send);
        this->sendCommand("receivedok:" + move);
        this->getReply("move:", "receivedok:" + move);
        QString mv = this->last_reply.split(":")[1];
        this->sendCommand("receivedmove:" + mv);
        this->getReply("receivedok:" + mv, "receivedmove:" + mv);
        // only now that both machines have confirmation the move has arrived, it is performed
        this->game->executeExternal(mv);
    } else {
        // there is no socket yet, wait to send until there is
        this->store_move = move;
    }
}
