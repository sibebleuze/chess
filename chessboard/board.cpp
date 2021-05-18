#include "board.h"

Board::Board(QWidget *mainwidget, int x_offset, int y_offset) {
    QFont fnt;
    for (int i = 0; i < 4; i++) {
        Field *g, *h;
        g = new Field(mainwidget, 0, i, x_offset + int((8.5 - i) * Field::side), y_offset - (i + 2) * Field::side);
        h = new Field(mainwidget, 7, i, x_offset + int((8.5 - i) * Field::side), y_offset + (5 - i) * Field::side);
        g->setVisible(false);
        h->setVisible(false);
        this->white_promoting.push_back(g);
        this->black_promoting.push_back(h);
        for (int j = 0; j < 2; j++) {
            Line *l = new Line(mainwidget, 2 * i + j, x_offset, y_offset);
            this->lines.push_back(l);
            QLabel *q, *r;
            q = new QLabel(mainwidget);
            r = new QLabel(mainwidget);
            q->setObjectName("column" + QString::number(2 * i + j));
            r->setObjectName("row" + QString::number(2 * i + j));
            q->setGeometry(x_offset + int(2 / 5.0 * Field::side) + Field::side * (2 * i + j),
                           y_offset,
                           Field::side / 2,
                           Field::side / 2);
            r->setGeometry(x_offset - int(3 / 10.0 * Field::side),
                           y_offset - int(7 / 10.0 * Field::side) - Field::side * (2 * i + j),
                           Field::side / 2,
                           Field::side / 2);
            q->setText(Field::row_names()[2 * i + j]);
            r->setText(QString::number(2 * i + j + 1));
            fnt = q->font();
            fnt.setPixelSize(Field::side / 3);
            q->setFont(fnt);
            fnt = r->font();
            fnt.setPixelSize(Field::side / 3);
            r->setFont(fnt);
            q->show();
            r->show();
            this->row_column_nametags.push_back(q);
            this->row_column_nametags.push_back(r);
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 4; i++) {
        // these first two vectors have a length of 4
        delete this->white_promoting.back();
        this->white_promoting.pop_back();
        delete this->black_promoting.back();
        this->black_promoting.pop_back();
        for (int j = 0; j < 2; j++) {
            // this one has a length of 8
            delete this->lines.back();
            this->lines.pop_back();
            for (int k = 0; k < 2; k++) {
                // and this one has a length of 16
                delete this->row_column_nametags.back();
                this->row_column_nametags.pop_back();
            }
        }
    }
}

Field *Board::operator[](std::pair<int, int> position) {
    return (*this->lines[position.first])[position.second];
}

bool Board::on_board(std::pair<int, int> position, std::pair<int, int> increment) {
    return 0 <= position.first + increment.first && position.first + increment.first < 8 &&
           0 <= position.second + increment.second && position.second + increment.second < 8;
} // checks if a move increment from a certain position is still on the board or not

Field *Board::getKingPosition(QString &color) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Field *f = (*this)[std::make_pair(i, j)];
            if (f->getPieceColor() == color && f->getPiece() == "king") {
                return f; // exactly one of these exists, so if we found it, we're sure it's the only one and we can return immediately
            }
        }
    }
    exit(KING_MISSING);
}

std::vector<Field *> Board::get_promoting(const QString &color) {
    std::vector<Field *> prom;
    if (color == "white") {
        prom = this->white_promoting;
    } else if (color == "black") {
        prom = this->black_promoting;
    } else {
        exit(EMPTY_PROMOTING);
    }
    return prom;
}
