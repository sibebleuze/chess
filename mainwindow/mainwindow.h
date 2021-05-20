#ifndef OOPFP_MAINWINDOW_H
#define OOPFP_MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../gamecontrol/game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    static std::map<int, QString> mds();

    static std::map<int, QString> mdtxts();

    static std::map<int, std::pair<int, int>> mdpos();

    static std::map<int, QString> lvltxts();

    static std::map<int, QString> mdinftxts();

public slots:

    void mode_choice();

    void level_choice();

    void online_submit();

private:
    Ui::MainWindow *ui;
    Game *g = nullptr;
    // Engine *e = nullptr;
    // Server *s = nullptr;
    // Client *c = nullptr;

    std::vector<QLabel *> mode_infos;
    std::vector<QPushButton *> modes;

    std::vector<QPushButton *> levels;

    QLabel *portinfo = nullptr;
    QLabel *ipinfo = nullptr;
    QLineEdit *server_ip = nullptr;
    QLineEdit *port = nullptr;
    QLabel *own_ip = nullptr;
    QPushButton *submit_serverinfo = nullptr;
};

#endif //OOPFP_MAINWINDOW_H
