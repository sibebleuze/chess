#ifndef CHESS_FIELD_H
#define CHESS_FIELD_H

#include <QIcon>
#include <QPushButton>

class Field : public QPushButton {
public:
    Field(QWidget *mainwidget, QString name, int left, int top, QString field_color, QString piece = "",
          QString piece_color = "");

    void change_icon(QString piece = "", QString piece_color = "", bool select = false);

public slots:

    void change_selection(bool do_emit = true);

signals:

    void select_switch() {};
private:
    QString field_color;
    QString piece;
    QString piece_color;
    bool selected = false;
};


#endif //CHESS_FIELD_H
