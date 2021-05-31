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
    delete this->game;
    if (this->socket != nullptr) {
        this->socket->close();
    }
    delete this->socket;
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

void Server::getReply(const QString &inReply, const QString &repeat) {
    do {
        QApplication::processEvents();
        if (this->socket->canReadLine()) {
            this->last_reply = QString::fromLocal8Bit(this->socket->readLine()).trimmed();
        } else {
            if (repeat != "") {
                this->sendCommand(repeat);
            }
            this->socket->waitForReadyRead(100);
        }
    } while (!this->last_reply.contains(inReply));
}

void Server::sendCommand(const QString &command) {
    this->socket->write(command.toLocal8Bit());
}

void Server::errorHandler(int exitcode) {
    emit this->chessError(exitcode);
}
