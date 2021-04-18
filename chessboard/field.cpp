#include "field.h"

Field::Field(QWidget *mainwidget, int linenumber, int position, int left, int top, QString field_color, QString piece,
             QString piece_color) :
        QPushButton(mainwidget) {
    this->field_color = field_color;
    this->piece = piece;
    this->piece_color = piece_color;
    this->setObjectName(this->row_names[linenumber] + QString(position + 1));
    this->setGeometry(QRect(left, top, 50, 50));
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

void Field::change_icon(QString piece, QString piece_color, bool select) {
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

void Field::change_selection(bool do_emit) {
    this->selected = !this->selected; // switch status
    this->change_icon(this->piece, this->piece_color, this->selected); // switch icon appropriately
    if (do_emit) { // sometimes do_emit = false, when triggered by the Board instead of a click
        emit this->select_switch();
    }
}
