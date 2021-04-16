#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    f = new Field(750, 450, this);
}

MainWindow::~MainWindow() {
    delete f;
    delete ui;
}

void MainWindow::changeEvent(QEvent *event) {
    // this centers the window when it is no longer maximized
    // TODO: check if this is the desired behaviour
    if (event->type() == QEvent::WindowStateChange) {
        if (this->windowState() == Qt::WindowNoState) {
            this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
        }
    }
}
