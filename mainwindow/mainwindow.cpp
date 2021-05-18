#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    g = new Game(this);
}

MainWindow::~MainWindow() {
    delete this->g;
    delete this->ui;
}
