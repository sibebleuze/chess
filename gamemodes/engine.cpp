#include "engine.h"

Engine::Engine(QWidget *mainwidget, const QString &player_color) {
    this->stockfish = new QProcess();
    this->stockfish->setProgram("stockfish");
    this->stockfish->setReadChannel(QProcess::StandardOutput);
    this->g = new Game(mainwidget, player_color);
    QObject::connect(g, &Game::lockedTurn, this, &Engine::engineMove);
}

Engine::~Engine() {
    delete this->g;
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
        emit this->chessError(STOCKFISH_FAILURE);
        return;
    }
    if (!this->getReply("Stockfish", 300)) {
        return;
    }
    this->giveCommand("uci");
    if (!this->getReply("uciok", 300)) {
        return; // stockfish must send "uciok" after "uci" command
    }
    // 'Skill Level' ranges from 0 to 20, level 4 is already quite hard, so easy=0, medium=4, hard=8
    this->giveCommand("setoption name Skill Level value " + QString::number(4 * level));
    this->giveCommand("isready");
    if (!this->getReply("readyok", 300)) {
        return; // stockfish must send "readyok" after "isready" command
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
        emit this->chessError(STOCKFISH_FAILURE);
        return false;
    }
}

void Engine::engineMove() {
    QApplication::processEvents();
    QStringList white_history = this->g->getHistory("white");
    QStringList black_history = this->g->getHistory("black");
    if (black_history.length() == white_history.length() - 1) {
        black_history.push_back("  -  "); // both need to be equal length to not crash the following loop
    }
    QString moves = "";
    for (int i = 0; i < white_history.length(); i++) {
        for (int j = 0; j < 2; j++) {
            QString histitem;
            if (j == 0) {
                histitem = white_history.at(i);
            } else {
                histitem = black_history.at(i);
            }
            if (histitem.contains("+")) {
                histitem.chop(1);
            } else if (histitem.contains("#")) {
                // not breaking off here will result in a "bestmove (none)" answer from stockfish
                emit this->chessError(MOVE_AFTER_GAME_END);
            }
            if (histitem.contains(" e.p.")) {
                histitem.chop(5);
            }
            QStringList cleanup;
            if (histitem.contains(" - ")) {
                cleanup = histitem.split(" - ");
            } else if (histitem.contains(" x ")) {
                cleanup = histitem.split(" x ");
            } else {
                emit this->chessError(SEPARATOR_MISSING);
            }
            if (cleanup.at(0) == "O") {
                if (cleanup.length() == 2) {
                    cleanup.replace(1, "g");
                } else if (cleanup.length() == 3) {
                    cleanup.replace(1, "c");
                    cleanup.removeLast();
                }
                if (j == 0) {
                    cleanup.replace(0, "e1");
                    cleanup.replace(1, cleanup.at(1) + "1");
                } else {
                    cleanup.replace(0, "e8");
                    cleanup.replace(1, cleanup.at(1) + "8");
                }
            }
            for (auto field : cleanup) {
                if (field.length() == 3) {
                    field.remove(0, 1);
                }
                moves += field.toLower();
            }
            moves += " ";
        }
    }
    moves = moves.trimmed();
    if (moves == "") {
        this->giveCommand("position startpos");
    } else {
        this->giveCommand("position startpos moves " + moves);
    }
    this->giveCommand("go movetime 5000"); // stockfish gets 5 seconds to think
    if (!this->getReply("bestmove", 60)) { // and 1 second after that to print the answer
        return;
    }
    QString bestmove = this->last_reply.split(" ")[1];
    this->last_reply = ""; // reset last reply so it doesn't try to make the same move twice
    QString origin = bestmove.left(2);
    QString destination = bestmove.mid(2, 2);
    QString promote_piece = "";
    if (bestmove.length() > 4) {
        promote_piece = bestmove.right(1);
    }
    this->g->executeExternal(origin, destination, promote_piece.toUpper());
}

void Engine::giveCommand(const QString &command) {
    this->stockfish->write((command + "\n").toLocal8Bit());
}
