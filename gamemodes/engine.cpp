#include "engine.h"
#include <QDebug>

Engine::Engine(QWidget *mainwidget) {
    this->stockfish = new QProcess();
    this->stockfish->setProgram("stockfish");
    this->stockfish->setReadChannel(QProcess::StandardOutput);
    this->g = new Game(mainwidget);
}

Engine::~Engine() {
    delete this->g;
    if (this->stockfish->state() == QProcess::Running) {
        this->stockfish->write("quit");
        this->stockfish->waitForFinished(1000);
        this->stockfish->close();
    }
    delete this->stockfish;
}

void Engine::start(int level) {
    this->stockfish->start();
    if (!this->stockfish->waitForStarted()) {
        emit this->chessError(STOCKFISH_FAILURE);
        return;
    }
    this->stockfish->waitForReadyRead();
    while (this->stockfish->canReadLine()) {
        this->stockfish->readLine();
//        qDebug() << this->stockfish->readLine();
    }
    this->stockfish->write("uci\n");
    if (!this->getReply("uciok")) {
        return; // stockfish must send "uciok" after "uci" command
    }
    this->stockfish->write(("setoption name Skill Level value " + QString::number(5 * level + 5) + "\n").toLocal8Bit());
    this->stockfish->write("isready\n");
    if (!this->getReply("readyok")) {
        return; // stockfish must send "readyok" after "isready" command
    }
}

bool Engine::getReply(QString inReply) {
    bool ready = this->stockfish->waitForReadyRead();
    do {
        if (!ready || !this->stockfish->canReadLine()) {
            emit this->chessError(STOCKFISH_FAILURE);
            return false;
        }
        this->last_reply = QString::fromLocal8Bit(this->stockfish->readLine()).trimmed();
//        qDebug() << "Line:" << line;
    } while (this->last_reply.contains(inReply));
    return true;
}
