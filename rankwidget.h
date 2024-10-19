#ifndef RANKWIDGET_H
#define RANKWIDGET_H

//#include "config.h"
#include "userinformation.h"
#include <QWidget>
#include <QTableWidget>
#include <list>

namespace Ui {
class RankWidget;
}

class RankWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RankWidget(QWidget *parent = nullptr, std::list<UserInformation>* m_rank_list = nullptr);
    ~RankWidget();

private:
    Ui::RankWidget *ui;

    QTableWidget* tableWidget;
    std::list<UserInformation>* rank_list;
};

#endif // RANKWIDGET_H
