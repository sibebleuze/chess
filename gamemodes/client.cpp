#include "client.h"

Client::Client(QWidget *mainwidget, const QString &server_ip, int server_port) {
    this->server_ip = server_ip;
    this->server_port = server_port;
    this->socket = new QTcpSocket(this);
    this->main = mainwidget;
    this->game = new Game(this->main, "black");
    QObject::connect(this->game, &Game::lockedTurn, this, &Client::serverMove);
    QObject::connect(this->game, &Game::chessError, this, &Client::errorHandler);
    this->socket->connectToHost(this->server_ip, this->server_port);
    do {
        QApplication::processEvents();
        qDebug() << "Connecting";
        this->socket->waitForConnected(100);
    } while (this->socket->state() != QAbstractSocket::ConnectedState && this->main->isVisible());
    if (this->main->isVisible()) {
        qDebug() << "Connected, now binding";
        this->socket->bind(QHostAddress(this->server_ip), this->server_port, QAbstractSocket::DontShareAddress);
        qDebug() << "Bound";
    }
    QObject::connect(this->socket, &QTcpSocket::readyRead, this, &Client::connection);
    QObject::connect(this->socket, &QTcpSocket::stateChanged, this, &Client::reconnect);
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
        this->socket->connectToHost(this->server_ip, this->server_port);
        do {
            QApplication::processEvents();
            qDebug() << "Reconnecting";
            this->socket->waitForConnected(100);
        } while (this->socket->state() != QAbstractSocket::ConnectedState && this->main->isVisible());
        if (this->main->isVisible()) {
            qDebug() << "Rebinding";
            this->socket->bind(QHostAddress(this->server_ip), this->server_port,
                               QAbstractSocket::DontShareAddress);
        }
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
