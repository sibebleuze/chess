#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    a = new Line(this, 0);
    b = new Line(this, 1);
    c = new Line(this, 2);
    d = new Line(this, 3);
    e = new Line(this, 4);
    f = new Line(this, 5);
    g = new Line(this, 6);
    h = new Line(this, 7);
}

MainWindow::~MainWindow() {
    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete g;
    delete h;
    delete ui;
}
