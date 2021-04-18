#ifndef CHESS_FIELD_H
#define CHESS_FIELD_H

#include <QIcon>
#include <QPushButton>

class Field : public QPushButton {
public:
    Field(QWidget *mainwidget,
          int linenumber, int position, // position of the field on the board (8x8)
          int x_offset, int y_offset); // x and y coordinate of bottom left corner of board (pixels)

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
};


#endif //CHESS_FIELD_H
