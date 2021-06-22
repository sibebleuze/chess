#include "engine.h"

Engine::Engine(QWidget *mainwidget, const QString &player_color) {
    this->stockfish = new QProcess();
    this->stockfish->setProgram("stockfish");
    this->stockfish->setReadChannel(QProcess::StandardOutput);
    this->game = new Game(mainwidget, player_color);
    QObject::connect(game, &Game::lockedTurn, this, &Engine::engineMove);
    QObject::connect(game, &Game::chessError, this, &Engine::errorHandler);
}

Engine::~Engine() {
    if (this->stockfish->state() == QProcess::Running) {
        this->giveCommand("quit");
        this->stockfish->waitForFinished(5000);
        this->stockfish->close(); // force shutdown of QProcess if it didn't respond
    }
    delete this->stockfish;
}

void Engine::start(int level) {
    QApplication::processEvents();
    bool started = false;
    this->stockfish->start();
    for (int i = 0; i < 300; i++) {
        QApplication::processEvents();
        if (this->stockfish->waitForStarted(100)) {
            started = true;
            break;
        }
    }
    if (!started) {
        emit this->chessError(STOCKFISH_NOT_FOUND);
        return;
    }
    if (!this->getReply("Stockfish")) {
        return; // stockfish gets 30 seconds to respond after startup command was issued
    }
    this->giveCommand("uci");
    if (!this->getReply("uciok")) {
        return; // stockfish must send "uciok" after "uci" command, it gets 30 seconds to do so
    }
    // 'Skill Level' ranges from 0 to 20, level 4 is already quite hard, so easy=0, medium=4, hard=8
    this->giveCommand("setoption name Skill Level value " + QString::number(4 * level));
    this->giveCommand("isready");
    if (!this->getReply("readyok")) {
        return; // stockfish must send "readyok" after "isready" command, it gets 30 seconds to do so
    }
}

bool Engine::getReply(const QString &inReply, int cycles) {
    do {
        QApplication::processEvents();
        if (this->stockfish->canReadLine()) {
            this->last_reply = QString::fromLocal8Bit(this->stockfish->readLine()).trimmed();
        } else {
            this->stockfish->waitForReadyRead(100);
            cycles -= 1;
        }
    } while (!this->last_reply.contains(inReply) && cycles > 0);
    if (this->last_reply.contains(inReply)) {
        return true;
    } else {
        emit this->chessError(STOCKFISH_NO_RESPONSE);
        return false;
    }
}

void Engine::engineMove() {
    QApplication::processEvents();
    QStringList white_history = this->game->getHistory("white");
    QStringList black_history = this->game->getHistory("black");
    if (black_history.length() == white_history.length() - 1) {
        black_history.push_back("  -  "); // both need to be equal length to not crash the following loop
    }
    QString moves = "";
    for (int i = 0; i < white_history.length(); i++) {
        for (int j = 0; j < 2; j++) {
            QString histitem = (j == 0) ? white_history.at(i) : black_history.at(i);
            moves += this->game->revAlgToLongAlg(histitem, (j == 0) ? "white" : "black") + " ";
        }
    }
    moves = moves.trimmed();
    QString comm_add = (moves == "") ? "" : " moves " + moves;
    this->giveCommand("position startpos" + comm_add);
    this->giveCommand("go movetime 5000"); // stockfish gets 5 seconds to think
    if (!this->getReply("bestmove", 350)) { // and 30 seconds after that to print the answer
        return;
    }
    QString move = this->last_reply.split(" ")[1];
    this->last_reply = ""; // reset last reply so it doesn't try to make the same move twice
    this->game->executeExternal(move);
}

void Engine::giveCommand(const QString &command) {
    this->stockfish->write((command + "\n").toLocal8Bit());
}
