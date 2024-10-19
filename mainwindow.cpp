#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"

#include <QKeyEvent>
#include <QString>
#include <QInputDialog>
#include <cstdlib>
#include <ctime>
#include <iostream>
//#include <string>
using namespace std;

MainWindow::MainWindow(QWidget *parent, std::list<UserInformation>* m_rank_list)
    : QMainWindow(parent), rank_list(m_rank_list)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(game_size);    // 设置固定窗口大小

    QString font_size("font-size: 32pt;");
    QFont font("宋体", 24); // 创建一个字体对象，设置字体为Arial，大小为12

    score_label = new QLabel("得分：0", this);
    //score_label->setGeometry(15, 350, 200, 30);      // 设置位置和大小
    score_label->setGeometry(50, 700, 400, 60);      // 设置位置和大小
    score_label->setStyleSheet(font_size);  // 设置文字大小为16点

    restart_button = new QPushButton("重新开始", this);
    //restart_button->setGeometry(180, 350, 100, 30);      // 设置位置和大小
    restart_button->setGeometry(360, 700, 200, 60);      // 设置位置和大小
    restart_button->setFont(font); // 设置按钮的字体为所创建的字体对象
    restart_button->setStyleSheet("background-color: rgba(255, 255, 255, 127);");
    connect(restart_button, &QPushButton::clicked, this, &MainWindow::start_game);

    back_button = new QPushButton("返回菜单", this);
    //back_button->setGeometry(180, 390, 100, 30);      // 设置位置和大小
    back_button->setGeometry(360, 780, 200, 60);      // 设置位置和大小
    back_button->setFont(font); // 设置按钮的字体为所创建的字体对象
    back_button->setStyleSheet("background-color: rgba(255, 255, 255, 127);");
    connect(back_button, &QPushButton::clicked, this, &MainWindow::back_initinterface);

    save_button = new QPushButton("保存记录", this);
    //save_button->setGeometry(35, 310, 100, 30);      // 设置位置和大小
    save_button->setGeometry(70, 620, 200, 60);      // 设置位置和大小
    save_button->setFont(font); // 设置按钮的字体为所创建的字体对象
    save_button->setStyleSheet("background-color: rgba(255, 255, 255, 127);");
    connect(save_button, &QPushButton::clicked, this, &MainWindow::save_game);

    recover_button = new QPushButton("恢复记录", this);
    //recover_button->setGeometry(180, 310, 100, 30);      // 设置位置和大小
    recover_button->setGeometry(360, 620, 200, 60);      // 设置位置和大小
    recover_button->setFont(font); // 设置按钮的字体为所创建的字体对象
    recover_button->setStyleSheet("background-color: rgba(255, 255, 255, 127);");
    connect(recover_button, &QPushButton::clicked, this, &MainWindow::recover_game);

    connect(&timer, &QTimer::timeout, this, &MainWindow::ai_press);

    connect(&timer, SIGNAL(timeout()), this, SLOT(frame_update()));

    start_game();

    //recover_game();

    timer.start(1000/frame_ps);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recover_game()
{
    matrix.read_mat();
    bool is_win = false;
    matrix.final_exam(is_win);
}

// 接收键盘输入
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat() || matrix.win() || matrix.lose() || matrix.is_updating())
        return;

    if (event->key() == Qt::Key_Space)
        start_game();

    switch (event->key()) {
    case Qt::Key_A:
        matrix.deal_input(Direction::left); // 调用matrix的deal_input属性处理输入
        break;
    case Qt::Key_D:
        matrix.deal_input(Direction::right);
        break;
    case Qt::Key_W:
        matrix.deal_input(Direction::up);
        break;
    case Qt::Key_S:
        matrix.deal_input(Direction::down);
        break;
    }
}

// 刷新每一帧
void MainWindow::frame_update()
{
    bool win;

    bool message_box = matrix.update(win);  //更新矩阵

    update_snapshot(message_box, win);      //更新快照
    update();                               //绘制快照
}

// 更新快照
void MainWindow::update_snapshot(bool message_box, bool win)
{
    QPixmap pix {game_size};
    QPainter painter {&pix};
    painter.setBrush(QBrush{game_bgc});
    painter.drawRect(QRect{QPoint{0,0},game_size});
    painter.end();

    matrix.draw(&pix);      // 在快照中绘制矩阵

    snapshot = pix; // 用snapshot保存更新后的快照
    if (message_box) {
        //QMessageBox::information(this, QString{"胜负已分"}, QString{"你"}+((win) ? "赢" : "输")+"了，按空格键重新开始");
        matrix.game_return = true;
        if(win) {
            //QMessageBox::information(this, QString{"游戏结束"}, QString{"恭喜你，成功玩到2048，你最终的得分是"} + QString::number(matrix.score) );

            QMessageBox::StandardButton reply = QMessageBox::question(this, QString{"游戏结束"},
                QString{"恭喜你，成功玩到2048，你最终的得分是"} + QString::number(matrix.score) + QString{"\n是否更新成绩？"}, QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QString name = QInputDialog::getText(this, "加入名人堂", "请输入用户名:");
                if (!name.isEmpty()) {
                    // 检查用户是否已经存在
                    bool userExists = false;
                    for (auto& userInfo : *rank_list) {
                        if (userInfo.name == name.toStdString()) {
                            // 用户已存在，更新成绩
                            userInfo.score = matrix.score;
                            userExists = true;
                            break;
                        }
                    }

                    if (!userExists) {
                        // 用户不存在，添加用户
                        rank_list->push_back(UserInformation(name.toStdString(), matrix.score));
                    }

                    //rank_list->push_back(UserInformation(name.toStdString(), matrix.score));
                    rank_list->sort(compareByScore);
                    QMessageBox::information(this, QString{"加入名人堂"}, QString{"成绩更新成功！"} );
                }
            }

        } else {
            //QMessageBox::information(this, QString{"游戏结束"}, QString{"你最终的得分是"} + QString::number(matrix.score) );
            QMessageBox::StandardButton reply = QMessageBox::question(this, QString{"游戏结束"},
                QString{"你最终的得分是"} + QString::number(matrix.score) + QString{"\n是否更新成绩？"}, QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QString name = QInputDialog::getText(this, "加入名人堂", "请输入用户名:");
                if (!name.isEmpty()) {
                    // 检查用户是否已经存在
                    bool userExists = false;
                    for (auto& userInfo : *rank_list) {
                        if (userInfo.name == name.toStdString()) {
                            // 用户已存在，更新成绩
                            userInfo.score = matrix.score;
                            userExists = true;
                            break;
                        }
                    }

                    if (!userExists) {
                        // 用户不存在，添加用户
                        rank_list->push_back(UserInformation(name.toStdString(), matrix.score));
                    }

                    //rank_list->push_back(UserInformation(name.toStdString(), matrix.score));
                    rank_list->sort(compareByScore);
                    QMessageBox::information(this, QString{"加入名人堂"}, QString{"成绩更新成功！"} );
                }
            }
        }
    }

}

// 绘制快照
void MainWindow::paintEvent(QPaintEvent* event)
{
    score_label->setText(QString("得分：") + QString::number(matrix.score));

    event->accept();
    QPainter painter(this);
    painter.drawPixmap(0, 0, snapshot);
}

void MainWindow::start_game()
{
    matrix = Matrix();  // 重新初始化矩阵
    matrix.spawn();
}

void MainWindow::save_game()
{
    matrix.write_mat();
}

void MainWindow::back_initinterface()
{
    emit backInitInterface();
}

void MainWindow::ai_press()
{
    if(!ai_mode || matrix.game_return) return;

    // 初始化随机数生成器
    srand(static_cast<unsigned int>(time(nullptr)));

    // 随机选择一个按键
    Qt::Key keys[] = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D};

    //cout<<"ai_press"<<endl;
    const int search_length = 5;
    const int operations_num = 4;
    int score_get;
    int max_score = 0;
    int max_index = 0;
    Matrix first_move_matrix;

    // 检测哪种走法最好
    for(int i = 0; i < operations_num; ++i) {
        score_get = 0;
        first_move_matrix = matrix;
        int t = first_move_matrix.test_move(i);
        if(t == -1) continue;
        else {
            score_get += t;
            Matrix search_move_matrix = first_move_matrix;
            for(int j = 0; j < search_length; ++j) {
                score_get += search_move_matrix.test_move(rand()%operations_num);
                /*for(int k = 0; k < operations_num; ++k) {
                    score_get += search_move_matrix.test_move(k);
                }*/
            }

            if(score_get >= max_score) {
                max_score = score_get;
                max_index = i;
            }
        }
    }

    // 走最好的那种走法
    int index = max_index;
    first_move_matrix = matrix;
    if(first_move_matrix.test_move(max_index) == -1) {
        bool valid = false;
        do {
            index = rand() % 4;
            switch (index) {
            case 0:
                valid = matrix.can_up();
                break;
            case 1:
                valid = matrix.can_left();
                break;
            case 2:
                valid = matrix.can_down();
                break;
            case 3:
                valid = matrix.can_right();
                break;
            default:
                break;
            }
            if (!matrix.can_up() && !matrix.can_down() && !matrix.can_left() && !matrix.can_right()) {
                break;
            }
        } while(!valid);
    }

    Qt::Key selectedKey = keys[index];
    // 创建按键事件并发送
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, selectedKey, Qt::NoModifier);
    QCoreApplication::postEvent(this, event);

    // 创建按键释放事件并发送
    QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, selectedKey, Qt::NoModifier);
    QCoreApplication::postEvent(this, releaseEvent);
}
