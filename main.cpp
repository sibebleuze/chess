#include "mainwindow/mainwindow.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(qtres);
    QApplication app(argc, argv);
    MainWindow w;
    w.setMinimumSize(1000, 600);
    w.move(QGuiApplication::screens()[0]->availableGeometry().center() -
           w.rect().center()); // window is centered on startup
    w.showMaximized(); // window is opened maximized on startup
    return QApplication::exec();
}
// TODO: put comments everywhere
// TODO: make all class objects a pointer (if this doesn't break everything (and if it's necessary))
