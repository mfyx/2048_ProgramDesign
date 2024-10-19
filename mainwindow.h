#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPixmap>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <list>

#include "matrix.h"
#include "userinformation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, std::list<UserInformation>* m_rank_list = nullptr);
    ~MainWindow();
    bool ai_mode;
    Matrix matrix = Matrix();
    QPushButton* back_button;

    void ai_press();
    void start_game();      // （重新）开始游戏

signals:

    void backInitInterface();

public slots:
    void frame_update();        // 更新每一帧

protected:
    void keyPressEvent(QKeyEvent* event);
    void paintEvent(QPaintEvent* event);

private:
    Ui::MainWindow *ui;
    Matrix matrix_try;

    QTimer timer;
    QPixmap snapshot;
    void update_snapshot(bool message_box, bool win);       // 渲染图像，显示游戏数据
    void back_initinterface();
    void recover_game();
    void save_game();

    QLabel* score_label;
    QPushButton* restart_button;
    QPushButton* save_button;
    QPushButton* recover_button;

    std::list<UserInformation>* rank_list;
};

#endif // MAINWINDOW_H
