#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <QProcess>
#include "gameMode.h"

class Engine : public GameMode {
Q_OBJECT
public:
    Engine(QWidget *mainwidget, const QString &player_color);

    ~Engine() override;

    void start(int level) override;

public slots:

    void engineMove() override;

private:
//    Game *game;
    QProcess *stockfish;

//    QString last_reply = "";

    bool getReply(const QString &inReply, int cycles = 300);

    void giveCommand(const QString &command);
};


#endif //CHESS_ENGINE_H
