#include "field.h"

Field::Field(int left, int top, QWidget *mainwidget) {
    button = new QPushButton(mainwidget);
    button->setObjectName(QString::fromUtf8("pushButton"));
    button->setGeometry(QRect(left, top, 50, 50));
    button->setIcon(QIcon("../icons/white_knight_light.svg"));
    button->setIconSize(button->size());
    button->setFixedSize(button->size());
    button->show();
}

Field::~Field() {
    delete button;
}
