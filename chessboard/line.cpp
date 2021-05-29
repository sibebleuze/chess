#include "line.h"

Line::Line(QWidget *mainwidget, int rownumber, int x_offset, int y_offset, int field_side) {
    for (int linenumber = 0; linenumber < 8; linenumber++) {
        Field *f;
        f = new Field(mainwidget, rownumber, linenumber, x_offset, y_offset, field_side);
        this->fields.push_back(f);
    }
}

Line::~Line() {
    for (int i = 0; i < 8; i++) {
        delete this->fields.back();
        this->fields.pop_back();
    }
}

Field *Line::operator[](int i) {
    return this->fields[i];
}
