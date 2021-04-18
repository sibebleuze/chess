#include "line.h"

Line::Line(QWidget *mainwidget, int linenumber, int x_offset, int y_offset) {
    for (int i = 0; i < 8; i++) {
        Field *f;
        f = new Field(mainwidget, linenumber, i, x_offset, y_offset);
//        QObject::connect(f, &QPushButton::clicked, f, &Field::change_selection);
        this->fields.push_back(f);
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
    // but this one needs to be overloaded anyway to connect to the fields vector,
    // so it might as well be done here
    return this->fields[i - 1];
}
