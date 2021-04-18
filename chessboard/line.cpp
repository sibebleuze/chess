#include "line.h"
#include "../exit_codes.h"

QString row_name(int line) {
    switch (line) {
        case 0:
            return "a";
        case 1:
            return "b";
        case 2:
            return "c";
        case 3:
            return "d";
        case 4:
            return "e";
        case 5:
            return "f";
        case 6:
            return "g";
        case 7:
            return "h";
        default:
            exit(IMPOSSIBLE_LINE_NUMBER); // this should not be possible, only 8 rows exist on a chess board
    }
}

QString left_field_color(int line) {
    switch (line) {
        case 0:
        case 2:
        case 4:
        case 6:
            return "dark";
        case 1:
        case 3:
        case 5:
        case 7:
            return "light";
        default:
            exit(IMPOSSIBLE_LINE_NUMBER); // this should not be possible, only 8 rows exist on a chess board
    }
}

QString find_piece_color(int linenumber) {
    switch (linenumber) {
        case 0:
        case 1:
            return "white";
        case 6:
        case 7:
            return "black";
        case 2:
        case 3:
        case 4:
        case 5:
            return "";
        default:
            exit(IMPOSSIBLE_LINE_NUMBER); // this should not be possible, only 8 rows exist on a chess board
    }
}

QString find_piece(int line, int position) {
    switch (line) {
        case 0:
        case 7:
            switch (position) {
                case 0:
                case 7:
                    return "rook";
                case 1:
                case 6:
                    return "knight";
                case 2:
                case 5:
                    return "bishop";
                case 3:
                    return "queen";
                case 4:
                    return "king";
                default:
                    exit(IMPOSSIBLE_ROW_NUMBER); // this should not be possible, only 8 columns exist on a chess board
            };
        case 1:
        case 6:
            return "pawn";
        case 2:
        case 3:
        case 4:
        case 5:
            return "";
        default:
            exit(IMPOSSIBLE_LINE_NUMBER); // this should not be possible, only 8 rows exist on a chess board
    }
}

Line::Line(QWidget *mainwidget, int linenumber, int x_offset, int y_offset) {
    QString name = row_name(linenumber);
    QString field_color = left_field_color(linenumber);
    QString piece_color = find_piece_color(linenumber);
    for (int i = 0; i < 8; i++) {
        QString piece = find_piece(linenumber, i);
        Field *f;
        f = new Field(mainwidget, name + QString(i + 1), x_offset + 50 * i, y_offset - 50 * linenumber, field_color,
                      piece, piece_color);
        QObject::connect(f, &QPushButton::clicked, f, &Field::change_selection);
        this->fields.push_back(f);
        field_color = [field_color]() -> QString {
            if (field_color == "light") { return "dark"; }
            else { return "light"; }
        }();
    }
}

Line::~Line() {
    for (int i = 0; i < 8; i++) {
        delete this->fields.back();
        this->fields.pop_back();
    }
}

Field *Line::operator[](int i) {
    return this->fields[i - 1]; // overload bracket operator, chess notation has fields a1 -> h8
}
