#include "field.h"

namespace field {
    // below are some maps that are constant but needed to be initialized somewhere;
    // since they will only be used in the Field constructor, this seemed like a good place
    std::map<int, QString> row_names = {{0, "a"},
                                        {1, "b"},
                                        {2, "c"},
                                        {3, "d"},
                                        {4, "e"},
                                        {5, "f"},
                                        {6, "g"},
                                        {7, "h"}}; // map of linenumbers to row names
    std::map<int, std::vector<QString>> field_colors = {
            {0, {"dark",  "light", "dark",  "light", "dark",  "light", "dark",  "light"}},
            {1, {"light", "dark",  "light", "dark",  "light", "dark",  "light", "dark"}},
            {2, {"dark",  "light", "dark",  "light", "dark",  "light", "dark",  "light"}},
            {3, {"light", "dark",  "light", "dark",  "light", "dark",  "light", "dark"}},
            {4, {"dark",  "light", "dark",  "light", "dark",  "light", "dark",  "light"}},
            {5, {"light", "dark",  "light", "dark",  "light", "dark",  "light", "dark"}},
            {6, {"dark",  "light", "dark",  "light", "dark",  "light", "dark",  "light"}},
            {7, {"light", "dark",  "light", "dark",  "light", "dark",  "light", "dark"}}
    }; // map of linenumber and position in line to color of field
    std::map<int, QString> piece_colors = {{0, "white"},
                                           {1, "white"},
                                           {2, ""},
                                           {3, ""},
                                           {4, ""},
                                           {5, ""},
                                           {6, "black"},
                                           {7, "black"}}; // map of the color of the pieces in the row
    std::map<int, std::vector<QString>> pieces = {
            {0, {"rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook"}},
            {1, {"pawn", "pawn",   "pawn",   "pawn",  "pawn", "pawn",   "pawn",   "pawn"}},
            {2, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {3, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {4, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {5, {"",     "",       "",       "",      "",     "",       "",       ""}},
            {6, {"pawn", "pawn",   "pawn",   "pawn",  "pawn", "pawn",   "pawn",   "pawn"}},
            {7, {"rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook"}}
    }; // map of linenumber and position in line to piece on field
}

Field::Field(QWidget *mainwidget, int linenumber, int position, int x_offset, int y_offset) :
        QPushButton(mainwidget) {
    this->field_color = field::field_colors[linenumber][position];
    this->piece = field::pieces[linenumber][position];
    this->piece_color = field::piece_colors[linenumber];
    this->position = std::make_pair(linenumber, position);
    this->setObjectName(
            field::row_names[linenumber] + QString(position + 1)); // e.g. 'a1' when linenumber = 0 and position = 0
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
