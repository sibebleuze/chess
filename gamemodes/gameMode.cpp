#include "gameMode.h"

GameMode::~GameMode() {
    delete this->game;
}
