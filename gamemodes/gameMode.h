#ifndef CHESS_GAMEMODE_H
#define CHESS_GAMEMODE_H

#include <QApplication>
#include "../gamecontrol/game.h"

class GameMode : public ChessObject {
Q_OBJECT
public:
    ~GameMode() override;

    virtual void
    start(int level) {}; // only implemented in engine, but needs to be called on pointer to GameMode object
    virtual void engineMove() {}; // only implemented in engine, but needs to be called on pointer to GameMode object
    virtual void
    firstServerMove() {}; // only implemented in client, but needs to be called on pointer to GameMode object

protected:
    Game *game = nullptr;
    QString last_reply = "";
};

#endif //CHESS_GAMEMODE_H
