#include "line.h"

Line::Line(QWidget *mainwidget, int linenumber, int x_offset, int y_offset) {
    QString field_color = this->left_field_colors[linenumber];
    QString piece_color = this->piece_colors[linenumber];
    for (int i = 0; i < 8; i++) {
        QString piece = pieces[linenumber][i];
        Field *f;
        f = new Field(mainwidget, linenumber, i, x_offset + 50 * i, y_offset - 50 * linenumber, field_color,
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

Field *Line::operator[](int i) { // overload bracket operator, chess notation has fields a1 -> h8
    // this could be done in the overloaded Board[] operator,
    // but this one needs to be defined anyway to connect to the fields vector,
    // so it might as well be done here
    return this->fields[i - 1];
}
