#ifndef CHESS_LOCAL_H
#define CHESS_LOCAL_H

#include "gameMode.h"

class Local : public GameMode {
Q_OBJECT
public:
    explicit Local(QWidget *mainwidget);
};


#endif //CHESS_LOCAL_H
