#ifndef CHESS_HISTORY_H
#define CHESS_HISTORY_H

#include <QTableWidget>
#include <QHeaderView>
#include <QDebug> // TODO: remove QDebug from final project
#include "../exit_codes.h"

class History : public QObject {
Q_OBJECT
public:
    History(QWidget *mainwidget, int x_offset, int y_offset, int field_side);

    ~History() override;

    QString getLastText(const QString &color);

    void setMove(const QString &color, const QString &move, bool add = false);

    void setResult(const QString &result);

    QStringList getHistory(const QString &color = "");

signals:

    void chessError(int exitcode);

private:
    QTableWidget *table;
    int active_row = 0;

    QTableWidgetItem *getItem(int row, const QString &color);

    void addRowIfNecessary();
};


#endif //CHESS_HISTORY_H
