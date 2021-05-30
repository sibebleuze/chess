#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <QProcess>
#include "../gamecontrol/game.h"

class Engine : public QObject {
Q_OBJECT
public:
    // Clang-Tidy: Single-argument constructors must be marked explicit to avoid unintentional implicit conversions
    explicit Engine(QWidget *mainwidget, const QString &player_color);

    // Clang-Tidy: Annotate this function with 'override' or (rarely) 'final'
    ~Engine() override;

    void start(int level);

public slots:

    void engineMove();

    void errorHandler(int exitcode);

signals:

    void chessError(int exitcode);

private:
    Game *g;
    QProcess *stockfish;
    QString last_reply;

    bool getReply(const QString &inReply, int cycles = 50);

    void giveCommand(const QString &command);
};


#endif //CHESS_ENGINE_H
