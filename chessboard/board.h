#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include "line.h"


class Board : public QObject {
public:
    Board(QWidget *mainwidget);

    ~Board();

    Field *operator[](QString name);

    Field *operator[](std::pair<int, int> position);

public slots:

    void field_clicked();

private:
    std::vector<Line *> lines; // will be vector of 8 Lines
    std::vector<Field *> get_field_moves(std::pair<int, int> position);

    static std::map<QString, int> row_numbers();

    static std::map<QString, std::vector<Field *>(*)(std::pair<int, int>, Board *)> pieces();
};


#endif //CHESS_BOARD_H
