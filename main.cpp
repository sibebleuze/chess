#include "mainwindow/mainwindow.h"
#include <QScreen>

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(qtres);
    QApplication app(argc, argv);
    MainWindow w;
    QObject::connect(&w, &MainWindow::chessError, &app, &QApplication::exit);
    w.setMinimumSize(1000, 600);
    w.move(QGuiApplication::screens()[0]->availableGeometry().center() -
           w.rect().center()); // window is centered on startup
//    w.showMaximized(); // window is opened maximized on startup // TODO: pick one!
    w.show(); // window is opened with minimum size (but not minimized)
    return QApplication::exec();
}
// TODO: put comments everywhere
// TODO: make all class objects a pointer (if this doesn't break everything (and if it's necessary))
