#include "mainwindow/mainwindow.h"
#include <QScreen>

int main(int argc, char *argv[]) {
    QCoreApplication::addLibraryPath(".");
    Q_INIT_RESOURCE(qtres);
    QApplication app(argc, argv);
    MainWindow w;
    QObject::connect(&w, &MainWindow::chessError, &app, &QApplication::exit);
    w.setMinimumSize(1000, 600);
    // window is centered on startup
    w.move(QGuiApplication::screens()[0]->availableGeometry().center() - w.rect().center());
    w.show(); // window is opened with minimum size (but not minimized)
    return QApplication::exec();
}
// TODO: put comments everywhere
