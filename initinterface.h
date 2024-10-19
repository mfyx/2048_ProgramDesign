#ifndef INITINTERFACE_H
#define INITINTERFACE_H

#include "rankwidget.h"
#include "config.h"
#include "userinformation.h"
#include <QMainWindow>
#include <QPainter>
#include <list>

namespace Ui {
class InitInterface;
}

enum Mode { NormalMode = 0, AiMode = 1, vsAiMode = 2 };
//extern bool compareByScore(const UserInformation& userInfo1, const UserInformation& userInfo2);

class InitInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit InitInterface(QWidget *parent = nullptr);
    ~InitInterface();

    std::list<UserInformation> rank_list;

signals:
    void startGame(int mode);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::InitInterface *ui;

    RankWidget* rankWidget;

    void read_file();
    void write_file();

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.drawPixmap(rect(),QPixmap(":/bk.jpg"),QRect());
    }
};

#endif // INITINTERFACE_H
