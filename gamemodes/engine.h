#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <QProcess>
#include "../gamecontrol/game.h"

class Engine : public QObject {
Q_OBJECT
public:
    Engine(QWidget *mainwidget);

    ~Engine();

    void start(int level);

signals:

    void chessError(int exitcode);

private:
    Game *g;
    QProcess *stockfish;
    QString last_reply;

    bool getReply(QString inReply);
};


#endif //CHESS_ENGINE_H
