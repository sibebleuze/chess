#include "board.h"

Board::Board(QWidget *mainwidget, int x_offset, int y_offset, int field_side) {
    QFont fnt;
    for (int i = 0; i < 4; i++) {
        Field *g, *h;
        g = new Field(mainwidget, 0, i, x_offset + int((8.5 - i) * field_side), y_offset - (i + 2) * field_side,
                      field_side);
        h = new Field(mainwidget, 7, i, x_offset + int((8.5 - i) * field_side), y_offset + (5 - i) * field_side,
                      field_side);
        g->setVisible(false);
        h->setVisible(false);
        this->white_promoting.push_back(g);
        this->black_promoting.push_back(h);
        for (int j = 0; j < 2; j++) {
            std::vector<Field *> fields;
            for (int linenumber = 0; linenumber < 8; linenumber++) {
                Field *f;
                f = new Field(mainwidget, 2 * i + j, linenumber, x_offset, y_offset, field_side);
                fields.push_back(f);
            }
            this->lines.push_back(fields);
            QLabel *q, *r;
            q = new QLabel(mainwidget);
            r = new QLabel(mainwidget);
            q->setObjectName("column" + QString::number(2 * i + j));
            r->setObjectName("row" + QString::number(2 * i + j));
            q->setGeometry(x_offset + int(2 / 5.0 * field_side) + field_side * (2 * i + j),
                           y_offset,
                           field_side / 2,
                           field_side / 2);
            r->setGeometry(x_offset - int(3 / 10.0 * field_side),
                           y_offset - int(7 / 10.0 * field_side) - field_side * (2 * i + j),
                           field_side / 2,
                           field_side / 2);
            q->setText(Field::column_names()[2 * i + j]);
            r->setText(QString::number(2 * i + j + 1));
            fnt = q->font();
            fnt.setPixelSize(field_side / 3);
            q->setFont(fnt);
            fnt = r->font();
            fnt.setPixelSize(field_side / 3);
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
            std::vector<Field *> fields = this->lines.back();
            for (int k = 0; k < 8; k++) {
                delete fields.back();
                fields.pop_back();
            }
            this->lines.pop_back();
            for (int l = 0; l < 2; l++) {
                // and this one has a length of 16
                delete this->row_column_nametags.back();
                this->row_column_nametags.pop_back();
            }
        }
    }
}

Field *Board::operator[](std::pair<int, int> position) {
    return this->lines[position.first][position.second];
}

std::map<QString, int> Board::column_numbers() {
    return {{"a", 0},
            {"b", 1},
            {"c", 2},
            {"d", 3},
            {"e", 4},
            {"f", 5},
            {"g", 6},
            {"h", 7}}; // map of rownames to linenumbers
}

Field *Board::operator[](const QString &name) { // overload bracket operator, chess notation has fields a1 -> h8
    // if e.g. name = 'd6',
    int row = name.rightRef(1).toInt() - 1; // then row = 5
    int column = Board::column_numbers()[name.left(1)]; // and column = 3
    // and then this will return (a pointer to) the field 'd6' of the board
    return (*this)[std::make_pair(row, column)];
}

bool Board::onBoard(std::pair<int, int> position, std::pair<int, int> increment) {
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
    emit this->chessError(KING_MISSING);
    // not sure if returning a nullpointer here will completely exit immediately, but it's the best I can do
    return nullptr;
}

std::vector<Field *> Board::getPromoting(const QString &color) {
    std::vector<Field *> prom;
    if (color == "white") {
        prom = this->white_promoting;
    } else if (color == "black") {
        prom = this->black_promoting;
    } else {
        emit this->chessError(COLOR_MISSING);
        // not sure if returning an empty vector here will completely exit immediately, but it's the best I can do
        return std::vector<Field *>();
    }
    return prom;
}
