#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    b = new Board(this);
}

MainWindow::~MainWindow() {
    delete b;
    delete ui;
}
