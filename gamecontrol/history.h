#ifndef CHESS_HISTORY_H
#define CHESS_HISTORY_H

#include <QTableWidget>
#include <QHeaderView>
#include "../exit_codes.h"

class History {
public:
    History(QWidget *mainwidget, int x_offset, int y_offset, int field_side);

    ~History();

    QString getLastText(QString &color);

    void setMove(const QString &color, const QString &move, bool add = false);

    void setResult(const QString &result);

private:
    QTableWidget *table;
    int active_row = 0;

    QTableWidgetItem *getItem(int row, const QString &color);

    void addRowIfNecessary();
};


#endif //CHESS_HISTORY_H
