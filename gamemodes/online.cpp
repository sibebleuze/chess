#include "online.h"

Online::~Online() {
    if (this->socket != nullptr) {
        this->socket->close();
    }
    delete this->socket;
}

void Online::getReply(const QString &inReply, const QString &repeat) {
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

void Online::sendCommand(const QString &command) {
    this->socket->write(command.toLocal8Bit());
}
