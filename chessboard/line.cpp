#include "line.h"

Line::Line(QWidget *mainwidget, int linenumber, int x_offset, int y_offset) {
    for (int rownumber = 0; rownumber < 8; rownumber++) {
        Field *f;
        f = new Field(mainwidget, linenumber, rownumber, x_offset, y_offset);
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
