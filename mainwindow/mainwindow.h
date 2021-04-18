#ifndef OOPFP_MAINWINDOW_H
#define OOPFP_MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
//#include "../chessboard/field.h"
#include "../chessboard/line.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    Line *a, *b, *c, *d, *e, *f, *g, *h;

private:
    Ui::MainWindow *ui;
};

#endif //OOPFP_MAINWINDOW_H
