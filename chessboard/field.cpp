#include "field.h"

std::map<int, QString> Field::row_names() {
    return {{0, "a"},
            {1, "b"},
            {2, "c"},
            {3, "d"},
            {4, "e"},
            {5, "f"},
            {6, "g"},
            {7, "h"}}; // map of linenumbers to row names
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
    }; // map of linenumber and position in line to color of field
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
    }; // map of linenumber and position in line to piece on field
}

Field::Field(QWidget *mainwidget, int linenumber, int position, int x_offset, int y_offset) :
        QPushButton(mainwidget) {
    this->field_color = Field::field_colors()[linenumber][position];
    this->piece = Field::pieces()[linenumber][position];
    this->piece_color = Field::piece_colors()[linenumber];
    this->position = std::make_pair(linenumber, position);
    this->setObjectName(
            Field::row_names()[linenumber] + QString(position + 1)); // e.g. 'a1' when linenumber = 0 and position = 0
    this->setGeometry(QRect(x_offset + 50 * position, y_offset - 50 * linenumber, 50, 50));
    QString filename;
    if (this->piece != "") {
        filename = "../icons/" + this->piece_color + "_" + this->piece + "_" + this->field_color + ".svg";
    } else {
        filename = "../icons/" + this->field_color + ".svg";
    }
    this->setIcon(QIcon(filename));
    this->setIconSize(this->size());
    this->setFixedSize(this->size());
    this->show();
}

void Field::changeIcon(QString piece, QString piece_color, bool select) {
    this->piece = piece;
    this->piece_color = piece_color;
    QString extension;
    if (select) {
        extension = "_selected.svg";
    } else {
        extension = ".svg";
    }
    QString filename;
    if (this->piece != "") {
        filename = "../icons/" + this->piece_color + "_" + this->piece + "_" + this->field_color + extension;
    } else {
        filename = "../icons/" + this->field_color + extension;
    }
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

bool Field::isSelected() {
    return this->selected;
}

std::pair<int, int> Field::getPosition() {
    return this->position;
}
