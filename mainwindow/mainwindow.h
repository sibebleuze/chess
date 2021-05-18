#ifndef OOPFP_MAINWINDOW_H
#define OOPFP_MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include "../gamecontrol/game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    Game *g;

private:
    Ui::MainWindow *ui;
};

#endif //OOPFP_MAINWINDOW_H
