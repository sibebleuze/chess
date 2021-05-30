#include "server.h"

Server::Server(QWidget *mainwidget, int port) {
    this->server = new QTcpServer(this);
    QObject::connect(this->server, &QTcpServer::newConnection, this, &Server::connection);
    this->server->listen(QHostAddress::AnyIPv4, port);
    this->game = new Game(mainwidget, "white");
    QObject::connect(this->game, &Game::lockedTurn, this, &Server::sendMove);
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
    if (!this->getReply("readyok")) {
        return;
    }
    QObject::connect(this->socket, &QTcpSocket::readyRead, this, &Server::receiveMove);
    if (this->store_move != "") {
        this->sendMove(this->store_move);
    }
}

void Server::errorHandler(int exitcode) {
    emit this->chessError(exitcode);
}

void Server::sendMove(const QString &move) {
    if (this->socket != nullptr) {
        this->store_move = "";
        this->sendCommand(this->game->revAlgToLongAlg(move, "white"));
        if (!this->getReply("receivedmove")) {
            return;
        }
        this->sendCommand("receivedok");
    } else {
        this->store_move = move;
    }
}

void Server::receiveMove() {
    if (!this->getReply("move:")) {
        return;
    }
    QString move = this->last_reply.split(":")[1];
    this->last_reply = ""; // reset last reply so it doesn't try to make the same move twice
    this->sendCommand("receivedmove");
    if (!this->getReply("receivedok")) {
        return;
    }
    this->game->executeExternal(move);
}

bool Server::getReply(const QString &inReply, int cycles) {
    do {
        QApplication::processEvents();
        if (this->socket->canReadLine()) {
            this->last_reply = QString::fromLocal8Bit(this->socket->readLine()).trimmed();
        } else {
            this->socket->waitForReadyRead(100);
            cycles -= 1;
        }
    } while (!this->last_reply.contains(inReply) && cycles > 0);
    if (this->last_reply.contains(inReply)) {
        return true;
    } else {
        emit this->chessError(CLIENT_NO_RESPONSE);
        return false;
    }
}

void Server::sendCommand(const QString &command) {
    this->socket->write(command.toLocal8Bit());
}
