#include "ui_mainwindow.h"
#include "mainwindow.h"

std::map<int, QString> MainWindow::mds() {
    return {{0, "localpvp"},
            {1, "localpve"},
            {2, "server"},
            {3, "client"}};
}

std::map<int, QString> MainWindow::mdtxts() {
    return {{0, "Player vs. player offline\nOne device"},
            {1, "Player vs. engine (offline)\nOne device"},
            {2, "Player vs. player online\nFirst device"},
            {3, "Player vs. player online\nSecond device"}};
}

std::map<int, std::pair<int, int>> MainWindow::mdpos() {
    return {{0, {150, 150}},
            {1, {450, 150}},
            {2, {150, 350}},
            {3, {450, 350}}};
}

std::map<int, QString> MainWindow::lvltxts() {
    return {{0, "Easy"},
            {1, "Medium"},
            {2, "Hard"}};
}

std::map<int, QString> MainWindow::mdinftxts() {
    return {{0, "Local"},
            {1, "Online"}};
}

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    for (int i = 0; i < 2; i++) {
        QString mode_info_txt = MainWindow::mdinftxts()[i];
        auto *mode_info = new QLabel(this);
        mode_info->setText(mode_info_txt);
        mode_info->setGeometry(380, 100 + 200 * i, 40, 30);
        mode_info->show();
        this->mode_infos.push_back(mode_info);
    }
    for (int i = 0; i < 4; i++) {
        QString mode = MainWindow::mds()[i];
        QString mode_txt = MainWindow::mdtxts()[i];
        std::pair<int, int> mode_pos = MainWindow::mdpos()[i];
        auto *mode_btn = new QPushButton(this);
        mode_btn->setObjectName(mode);
        mode_btn->setText(mode_txt);
        mode_btn->setGeometry(mode_pos.first, mode_pos.second, 200, 100);
        QObject::connect(mode_btn, &QPushButton::clicked, this, &MainWindow::modeChoice);
        mode_btn->show();
        if (i > 1) {
            mode_btn->setDisabled(true);
        }
        this->modes.push_back(mode_btn);
    }
}

MainWindow::~MainWindow() {
    delete this->ui;
    delete this->g;
    delete this->e;
    // delete this->s;
    // delete this->c;
    for (int i = 0; i < this->mode_infos.size(); i++) {
        delete this->mode_infos.back();
        this->mode_infos.pop_back();
    }
    for (int i = 0; i < this->modes.size(); i++) {
        delete this->modes.back();
        this->modes.pop_back();
    }
    for (int i = 0; i < this->levels.size(); i++) {
        delete this->levels.back();
        this->levels.pop_back();
    }
    delete this->portinfo;
    delete this->ipinfo;
    delete this->server_ip;
    delete this->port;
    delete this->own_ip;
}

void MainWindow::modeChoice() {
    for (auto mode : this->modes) {
        mode->hide();
    }
    for (auto mode_info : this->mode_infos) {
        mode_info->hide();
    }
    auto *emitting = (QPushButton *) (QObject::sender());
    QString mode = emitting->objectName();
    if (mode == "localpvp") {
        this->g = new Game(this);
    } else if (mode == "localpve") {
        for (int i = 0; i < 3; i++) {
            QString whitelvltxt = MainWindow::lvltxts()[i] + "\nPlay as white";
            auto *whitelvlbtn = new QPushButton(this);
            whitelvlbtn->setObjectName("white" + QString::number(i));
            whitelvlbtn->setText(whitelvltxt);
            whitelvlbtn->setGeometry(250 + 200 * i, 230, 100, 60);
            QObject::connect(whitelvlbtn, &QPushButton::clicked, this, &MainWindow::levelChoice);
            whitelvlbtn->show();
            this->levels.push_back(whitelvlbtn);
            QString blacklvltxt = MainWindow::lvltxts()[i] + "\nPlay as black";
            auto *blacklvlbtn = new QPushButton(this);
            blacklvlbtn->setObjectName("black" + QString::number(i));
            blacklvlbtn->setText(blacklvltxt);
            blacklvlbtn->setGeometry(250 + 200 * i, 330, 100, 60);
            QObject::connect(blacklvlbtn, &QPushButton::clicked, this, &MainWindow::levelChoice);
            blacklvlbtn->show();
            this->levels.push_back(blacklvlbtn);
        }
    } else if (mode == "server") {
        QNetworkAccessManager man(this);
        QNetworkRequest req(QUrl("https://myexternalip.com/raw"));
        QEventLoop loop;
        QNetworkReply *repl = man.get(req);
        QObject::connect(repl, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        QString ext_ip = repl->readAll();
        this->own_ip = new QLabel(this);
        this->own_ip->setText("Your IP address is " + ext_ip + "\nYou should pass this on to your opponent.");
        this->own_ip->setGeometry(400, 150, 300, 100);
        this->own_ip->show();
        this->portinfo = new QLabel(this);
        this->portinfo->setText("Set your desired port below.\nYou should pass this on to your opponent.");
        this->portinfo->setGeometry(400, 300, 300, 100);
        this->portinfo->show();
        this->port = new QLineEdit(this);
        this->port->setPlaceholderText("e.g. 16534");
        this->port->setText("16534");
        this->port->setGeometry(400, 410, 100, 30);
        this->port->show();
        this->submit_serverinfo = new QPushButton(this);
        this->submit_serverinfo->setObjectName("server");
        this->submit_serverinfo->setText("Submit");
        this->submit_serverinfo->setGeometry(250, 250, 50, 30);
        QObject::connect(this->submit_serverinfo, &QPushButton::clicked, this, &MainWindow::onlineSubmit);
        this->submit_serverinfo->show();
    } else if (mode == "client") {
        // make and show client labels and lineedits, geometry y equal and x ~ i
        this->ipinfo = new QLabel(this);
        this->ipinfo->setText("Set the correct IP address below.\nYou should get this from to your opponent.");
        this->ipinfo->setGeometry(400, 150, 300, 100);
        this->ipinfo->show();
        this->server_ip = new QLineEdit(this);
        this->server_ip->setPlaceholderText("e.g. 192.168.1.1");
        this->server_ip->setText("");
        this->server_ip->setGeometry(400, 260, 100, 30);
        this->server_ip->show();
        this->portinfo = new QLabel(this);
        this->portinfo->setText("Set the correct port below.\nYou should get this from to your opponent.");
        this->portinfo->setGeometry(400, 300, 300, 100);
        this->portinfo->show();
        this->port = new QLineEdit(this);
        this->port->setPlaceholderText("e.g. 16534");
        this->port->setText("16534");
        this->port->setGeometry(400, 410, 100, 30);
        this->port->show();
        this->submit_serverinfo = new QPushButton(this);
        this->submit_serverinfo->setObjectName("client");
        this->submit_serverinfo->setText("Submit");
        this->submit_serverinfo->setGeometry(250, 250, 50, 30);
        QObject::connect(this->submit_serverinfo, &QPushButton::clicked, this, &MainWindow::onlineSubmit);
        this->submit_serverinfo->show();
    }
}

void MainWindow::levelChoice() {
    for (auto level : this->levels) {
        level->hide();
    }
    auto *emitting = (QPushButton *) (QObject::sender());
    int lvl = emitting->objectName().rightRef(1).toInt();
    QString color = emitting->objectName().left(5);
    this->e = new Engine(this, color);
    QObject::connect(this->e, &Engine::chessError, this, &MainWindow::errorHandler);
    this->e->start(lvl);
    if (color == "black") {
        // first engine move needs to be triggered 'manually' here,
        // all others will be triggered by the switching of the turn during the game
        this->e->engineMove();
    }
}

void MainWindow::onlineSubmit() {
    auto *emitting = (QPushButton *) (QObject::sender());
    QString online = emitting->objectName();
    int serverport = this->port->text().toInt();
    if (serverport <= 1024 || serverport >= 49152) {
        return;
    }
    if (online == "client") {
        QString serverip = this->server_ip->text();
        if (serverip.count(".") != 3) {
            return;
        }
        this->ipinfo->hide();
        this->server_ip->hide();
        this->portinfo->hide();
        this->port->hide();
        this->submit_serverinfo->hide();
        // this goes to parameter *mainwindow, serverip goes to parameter serverip, serverport goes to parameter port
//        this->c = new Client(this, serverip, serverport);
    } else if (online == "server") {
        this->own_ip->hide();
        this->portinfo->hide();
        this->port->hide();
        this->submit_serverinfo->hide();
        // this goes to parameter *mainwindow, serverport goes to parameter port
//        this->s = new Server(this, serverport);
    } else {
        exit(ONLINE_TYPE_MISSING);
    }
}

void MainWindow::errorHandler(int exitcode) {
    emit this->chessError(exitcode);
}