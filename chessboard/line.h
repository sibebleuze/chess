#ifndef CHESS_LINE_H
#define CHESS_LINE_H


#include "field.h"
#include <vector>
#include <map>

class Line {
public:
    Line(QWidget *mainwidget,
         int linenumber, // chess board has 8 lines/rows, constructor needs to know which one it is
         int x_offset = 100, // x coordinate of left of board
         int y_offset = 500); // y coordinate of bottom of board

    ~Line();

    Field *operator[](int i);

private:
    std::vector<Field *> fields; // will be vector of 8 Fields
    // below are some maps that are constant but needed to be initialized somewhere;
    // since they will only be used in the Line constructor, this seemed like a good place
    std::map<int, QString> row_names = {{0, "a"},
                                        {1, "b"},
                                        {2, "c"},
                                        {3, "d"},
                                        {4, "e"},
                                        {5, "f"},
                                        {6, "g"},
                                        {7, "h"}}; // map of linenumbers to row names
    std::map<int, QString> left_field_colors{{0, "dark"},
                                             {1, "light"},
                                             {2, "dark"},
                                             {3, "light"},
                                             {4, "dark"},
                                             {5, "light"},
                                             {6, "dark"},
                                             {7, "light"}}; // map of linenumbers to color of left field in the row
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
    };
};


#endif //CHESS_LINE_H
