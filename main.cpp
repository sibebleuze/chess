#include "mainwindow/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w; // TODO: put comments everywhere
    w.setMinimumSize(1000, 600);
    w.move(QApplication::desktop()->availableGeometry().center() - w.rect().center()); // window is centered on startup
    w.showMaximized(); // window is opened maximized on startup
    return app.exec();
}
