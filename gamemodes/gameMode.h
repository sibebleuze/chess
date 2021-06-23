#ifndef CHESS_GAMEMODE_H
#define CHESS_GAMEMODE_H

#include <QApplication>
#include "../gamecontrol/game.h"

class GameMode : public ChessObject {
Q_OBJECT
public:
    ~GameMode() override;

    // two functions below are only implemented in engine, but needs to be called on pointer to GameMode object
    virtual void start(int level) {};

    virtual void engineMove() {};

    // function below is only implemented in client, but needs to be called on pointer to GameMode object
    virtual void firstServerMove() {};

protected:
    Game *game = nullptr;
    QString last_reply = "";
};

#endif //CHESS_GAMEMODE_H
