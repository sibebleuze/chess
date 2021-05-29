#include "history.h"

History::History(QWidget *mainwidget, int x_offset, int y_offset, int field_side) {
    QFont fnt;
    this->table = new QTableWidget(1, 2, mainwidget);
    this->table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fnt = this->table->font();
    fnt.setPixelSize(field_side / 3);
    this->table->setFont(fnt);
    this->table->setHorizontalHeaderLabels({"white", "black"});
    for (int i = 0; i < 2; i++) {
        QTableWidgetItem *hdr = this->table->horizontalHeaderItem(i);
        fnt = hdr->font();
        fnt.setPixelSize(field_side / 3);
        hdr->setFont(fnt);
        this->table->setHorizontalHeaderItem(i, hdr);
        this->table->setColumnWidth(i, int(2.5 * field_side));
    }
    this->table->setGeometry(x_offset + 10 * field_side, // next to the board at the right side
                             y_offset - 8 * field_side, // on the same height as the board
                             23 + this->table->verticalHeader()->width() + 5 * field_side,
            // two columns, a vertical header, a vertical scrollbar (21px) and 2 extra px
                             8 * field_side); // same height as the board
    // 1 row needed for this->history->verticalHeader() to have the correct width, but game should start with zero rows
    this->table->setRowCount(0);
    this->table->show();
}

History::~History() {
    delete this->table;
}

QString History::getLastText(QString &color) {
    return this->getItem(this->table->rowCount() - 1, color)->text();
}

void History::setMove(const QString &color, const QString &move, bool add) {
    QTableWidgetItem *x;
    this->addRowIfNecessary();
    x = this->getItem(this->active_row, color);
    if (add) {
        if (color == "black") {
            // a new row was initiated, in which the black field is empty, but since the addition needs to happen at a
            // non empty field, the previous row must be taken
            x = this->getItem(this->active_row - 1, color);
            // this problem does not occur for a white field, since the new row initiation does not happen there
        }
        x->setText(x->text() + move);
    } else {
        x->setText(move);
        if (color == "black") {
            this->active_row += 1;
        }
    }
    this->table->scrollToItem(x, QAbstractItemView::PositionAtBottom);
}

QTableWidgetItem *History::getItem(int row, const QString &color) {
    int col;
    if (color == "white") {
        col = 0;
    } else if (color == "black") {
        col = 1;
    } else {
        exit(COLOR_MISSING); // the input color of this function will be Game::turn
    }
    return this->table->item(row, col);
}

void History::setResult(const QString &result) {
    this->addRowIfNecessary();
    QStringList l = result.split(" - ");
    this->table->insertRow(this->table->rowCount());
    this->table->resizeRowsToContents();
    for (int i = 0; i < 2; i++) {
        auto *x = new QTableWidgetItem(l[i]);
        x->setTextAlignment(Qt::AlignCenter);
        this->table->setItem(this->table->rowCount() - 1, i, x);
    }
}

void History::addRowIfNecessary() {
    if (this->active_row == this->table->rowCount()) {
        this->table->insertRow(this->active_row);
        this->table->resizeRowsToContents();
        QTableWidgetItem *x;
        for (int i = 0; i < 2; i++) {
            x = new QTableWidgetItem(" ");
            x->setTextAlignment(Qt::AlignCenter);
            this->table->setItem(this->active_row, i, x);
        }
    }
}

QStringList History::getHistory(QString color) {
    QStringList l;
    if (color == "white" || color == "black") {
        for (int r = 0; r < this->table->rowCount(); r++) {
            l << this->getItem(r, color)->text();
        }
    } else {
        exit(COLOR_MISSING);
    }
    if (!l.isEmpty() && l.last() == " ") {
        l.removeLast();
    }
    return l;
}
