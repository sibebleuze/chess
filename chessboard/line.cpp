#include "line.h"

Line::Line(QWidget *mainwidget, QString name, int y_offset, QString field_color, std::vector<QString> pieces) {
    int x_offset = 300;
    for (int i = 0; i < 8; i++) {
        Field *f;
        f = new Field(mainwidget, name + QString(i + 1), x_offset + 50 * i, y_offset, field_color);
        this->fields.push_back(f);
        if (field_color == "light") {
            field_color = "dark";
        } else {
            field_color = "light";
        }
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
