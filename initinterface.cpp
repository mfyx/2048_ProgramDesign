#include "initinterface.h"
#include "ui_initinterface.h"
#include "config.h"

//#include <QSoundEffect>
#include <QSize>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

InitInterface::InitInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InitInterface)
{
    ui->setupUi(this);
    //setFixedSize(QSize(360, 545));    // 设置固定窗口大小
    setFixedSize(QSize(500, 800));    // 设置固定窗口大小

    //设置按钮透明
    ui->pushButton->setStyleSheet("background-color: rgba(255, 255, 255, 127);");
    ui->pushButton_2->setStyleSheet("background-color: rgba(255, 255, 255, 127);");
    ui->pushButton_3->setStyleSheet("background-color: rgba(255, 255, 255, 127);");
    ui->pushButton_4->setStyleSheet("background-color: rgba(255, 255, 255, 127);");
    //ui->spinBox->hide();

    // 播放音乐
    //QSoundEffect * startSound = new QSoundEffect(this);
    //startSound->setSource(QUrl::fromLocalFile("://Chain_Y.wav"));
    //startSound->setSource(QUrl::fromLocalFile("://bgm_0.wav"));
    //startSound->setLoopCount(QSoundEffect::Infinite);  //设置无限循环
    //startSound->setVolume(0.3f);  //设置音量，在0到1之间
    //startSound->play();

    read_file();
}

InitInterface::~InitInterface()
{
    write_file();
    delete ui;
}

//普通模式
void InitInterface::on_pushButton_clicked()
{
    //mat_height = ui->spinBox->value();
    //mat_width = ui->spinBox->value();
    emit startGame(NormalMode);
}

//AI模式
void InitInterface::on_pushButton_2_clicked()
{
    //mat_height = ui->spinBox->value();
    //mat_width = ui->spinBox->value();
    emit startGame(AiMode);
}

//人机对战模式
void InitInterface::on_pushButton_3_clicked()
{
    //mat_height = ui->spinBox->value();
    //mat_width = ui->spinBox->value();
    emit startGame(vsAiMode);
}

//排行榜
void InitInterface::on_pushButton_4_clicked()
{
    rankWidget = new RankWidget(nullptr, &rank_list);
    rankWidget->show();
}

void InitInterface::read_file()
{
    ifstream file("rank.txt", ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string name;
            int score;
            iss >> name >> score;
            rank_list.push_back(UserInformation(name, score));
            rank_list.sort(compareByScore);
        }
        file.close();
    } else {
        cout << "读取文件失败！" << endl;
    }
}

void InitInterface::write_file()
{
    ofstream file("rank.txt", ios::out | ios::trunc);
    if (file.is_open()) {
        for (auto& user_info : rank_list) {
            file << user_info.name << " " << user_info.score << endl;
        }
        //file << "lzy 1000" << endl;
        file.close();
    } else {
        cout << "写入文件失败！" << endl;
    }
}
