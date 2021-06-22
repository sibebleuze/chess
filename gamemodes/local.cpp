#include "local.h"

Local::Local(QWidget *mainwidget) {
    this->game = new Game(mainwidget);
    QObject::connect(game, &Game::chessError, this, &Local::errorHandler);
}
