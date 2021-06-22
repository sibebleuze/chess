#ifndef OOPFP_MAINWINDOW_H
#define OOPFP_MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../gamemodes/local.h"
#include "../gamemodes/engine.h"
#include "../gamemodes/server.h"
#include "../gamemodes/client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    static std::map<int, QString> mds();

    static std::map<int, QString> mdtxts();

    static std::map<int, std::pair<int, int>> mdpos();

    static std::map<int, QString> lvltxts();

    static std::map<int, QString> mdinftxts();

public slots:

    void modeChoice();

    void levelChoice();

    void onlineSubmit();

    void errorHandler(int exitcode);

signals:

    void chessError(int exitcode);

private:
    Ui::MainWindow *ui;
    GameMode *gm = nullptr;

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
