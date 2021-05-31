#include "client.h"

Client::Client(QWidget *mainwidget, const QString &server_ip, int server_port) {
    this->server_ip = server_ip;
    this->server_port = server_port;
    this->socket = new QTcpSocket(this);
    this->game = new Game(mainwidget, "black");
    QObject::connect(this->game, &Game::lockedTurn, this, &Client::serverMove);
    QObject::connect(this->game, &Game::chessError, this, &Client::errorHandler);
    bool connected;
    do {
        QApplication::processEvents();
        connected = this->socket->bind(QHostAddress(this->server_ip), this->server_port,
                                       QAbstractSocket::DontShareAddress);
    } while (!connected);
    QObject::connect(this->socket, &QTcpSocket::readyRead, this, &Client::connection);
    QObject::connect(this->socket, &QTcpSocket::stateChanged, this, &Client::reconnect);
}

Client::~Client() {
    delete this->game;
    this->socket->close();
    delete this->socket;
}

void Client::connection() {
    this->last_reply = QString::fromLocal8Bit(this->socket->readLine()).trimmed();
    if (this->last_reply == "isready") {
        // the client needs to respond to every 'isready' with 'readyok',
        // since this is the only command the server will not respond to
        this->sendCommand("readyok");
        // all other server responses will pass through here, but will not enter the if statement;
        // just reading in the reply should not disturb getReply
    }
}

void Client::reconnect() {
    if (this->socket->state() != QAbstractSocket::BoundState) {
        bool connected;
        do {
            QApplication::processEvents();
            connected = this->socket->bind(QHostAddress(this->server_ip), this->server_port,
                                           QAbstractSocket::DontShareAddress);
        } while (!connected);
    }
    if (this->socket->state() == QAbstractSocket::BoundState && this->store_move != "") {
        this->serverMove(this->store_move);
    }
}

void Client::firstServerMove() {
    if (this->socket->state() == QAbstractSocket::BoundState) {
        this->getReply("move:", "");
        QString mv = this->last_reply.split(":")[1];
        this->sendCommand("receivedmove:" + mv);
        this->getReply("receivedok:" + mv, "receivedmove:" + mv);
        // only now that both machines have confirmation the move has arrived, it is performed
        this->game->executeExternal(mv);
    } else {
        this->reconnect();
        this->firstServerMove();
    }
}

void Client::serverMove(const QString &move) {
    if (this->socket->state() == QAbstractSocket::BoundState) {
        this->store_move = "";
        QString to_send = this->game->revAlgToLongAlg(move, "black");
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
        // the socket is disconnected, wait to send until it reconnected
        this->store_move = move;
        this->reconnect();
    }
}

void Client::getReply(const QString &inReply, const QString &repeat) {
    do {
        QApplication::processEvents();
        if (this->socket->canReadLine()) {
            this->last_reply = QString::fromLocal8Bit(this->socket->readLine()).trimmed();
        } else {
            if (repeat != "") {
                this->sendCommand(repeat);
                qDebug() << repeat;
            }
            this->socket->waitForReadyRead(100);
        }
    } while (!this->last_reply.contains(inReply));
}

void Client::sendCommand(const QString &command) {
    this->socket->write(command.toLocal8Bit());
}

void Client::errorHandler(int exitcode) {
    emit this->chessError(exitcode);
}
