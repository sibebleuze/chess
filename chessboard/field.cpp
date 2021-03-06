#include "field.h"

std::map<int, QString> Field::column_names() {
    return {{0, "a"},
            {1, "b"},
            {2, "c"},
            {3, "d"},
            {4, "e"},
            {5, "f"},
            {6, "g"},
            {7, "h"}}; // map of linenumbers to column names
}

std::map<int, std::vector<QString>> Field::field_colors() {
    return {
            {0, {"dark",  "light", "dark",  "light", "dark",  "light", "dark",  "light"}},
            {1, {"light", "dark",  "light", "dark",  "light", "dark",  "light", "dark"}},
            {2, {"dark",  "light", "dark",  "light", "dark",  "light", "dark",  "light"}},
            {3, {"light", "dark",  "light", "dark",  "light", "dark",  "light", "dark"}},
            {4, {"dark",  "light", "dark",  "light", "dark",  "light", "dark",  "light"}},
            {5, {"light", "dark",  "light", "dark",  "light", "dark",  "light", "dark"}},
            {6, {"dark",  "light", "dark",  "light", "dark",  "light", "dark",  "light"}},
            {7, {"light", "dark",  "light", "dark",  "light", "dark",  "light", "dark"}}
    }; // map of rownumber and linenumber to color of field
}

std::map<int, QString> Field::piece_colors() {
    return {{0, "white"},
//            {1, ""},
            {1, "white"},
            {2, ""},
            {3, ""},
            {4, ""},
            {5, ""},
//            {6, ""},
            {6, "black"},
            {7, "black"}}; // map of the color of the pieces in the row
}

std::map<int, std::vector<QString>> Field::pieces() {
    return {
            {0, {"rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook"}},
//            {1, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {1, {"pawn", "pawn",   "pawn",   "pawn",  "pawn", "pawn",   "pawn",   "pawn"}},
            {2, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {3, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {4, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {5, {"",     "",       "",       "",      "",     "",       "",       ""}},
//            {6, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {6, {"pawn", "pawn",   "pawn",   "pawn",  "pawn", "pawn",   "pawn",   "pawn"}},
            {7, {"rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook"}}
    }; // map of rownumber and linenumber to piece on field
}

Field::Field(QWidget *mainwidget, int rownumber, int linenumber, int x_offset, int y_offset, int field_side) :
        QPushButton(mainwidget) {
    this->field_color = Field::field_colors()[rownumber][linenumber];
    this->piece = Field::pieces()[rownumber][linenumber];
    this->piece_color = Field::piece_colors()[rownumber];
    this->position = std::make_pair(rownumber, linenumber);
    this->setObjectName(
            Field::column_names()[linenumber] +
            QString::number(rownumber + 1)); // e.g. 'a1' when linenumber = 0 and rownumber = 0
    this->setGeometry(x_offset + field_side * linenumber, y_offset - field_side * (rownumber + 1),
            // rownumber + 1, because otherwise (x_offset, y_offset) isn't the bottom left point of the board
                      field_side, field_side);
    QString filename = (this->piece == "") ? "://" + this->field_color + ".svg" :
                       "://" + this->piece_color + "_" + this->piece + "_" + this->field_color + ".svg";
    this->setIcon(QIcon(filename));
    this->setIconSize(Field::size());
    this->setFixedSize(this->size());
    this->show();
}

void Field::changeIcon(QString p, QString pc, bool select) {
    this->piece = std::move(p);
    this->piece_color = std::move(pc);
    QString extension = (select) ? "_selected.svg" : ".svg";
    QString filename = (this->piece == "") ? "://" + this->field_color + extension :
                       "://" + this->piece_color + "_" + this->piece + "_" + this->field_color + extension;
    this->setIcon(QIcon(filename));
    this->setIconSize(this->size());
    this->show();
}

void Field::changeSelection() {
    this->selected = !this->selected; // switch status
    this->changeIcon(this->piece, this->piece_color, this->selected); // switch icon appropriately
}

QString Field::getPiece() {
    return this->piece;
}

QString Field::getPieceColor() {
    return this->piece_color;
}

bool Field::isSelected() const {
    return this->selected;
}

std::pair<int, int> Field::getPosition() {
    return this->position;
}
