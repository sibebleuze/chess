#include "mainwindow/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setMinimumSize(1500, 900);
    w.showMaximized(); // window is opened maximized on startup
    return app.exec();
}
