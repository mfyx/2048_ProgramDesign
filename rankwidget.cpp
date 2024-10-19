#include "rankwidget.h"
#include "ui_rankwidget.h"
#include <QTableWidgetItem>
#include <QVBoxLayout>

RankWidget::RankWidget(QWidget *parent, std::list<UserInformation>* m_rank_list)
    : QWidget(parent), rank_list(m_rank_list)
    , ui(new Ui::RankWidget)
{
    ui->setupUi(this);

    tableWidget = new QTableWidget(100, 2, this);
    tableWidget->setFont(QFont("宋体",14));
    tableWidget->setHorizontalHeaderLabels(QStringList() << "用户" << "得分");
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int row = 0;
    for (const UserInformation& userInfo : *rank_list) {
        QTableWidgetItem* userItem = new QTableWidgetItem(QString::fromStdString(userInfo.name));
        tableWidget->setItem(row, 0, userItem);

        QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(userInfo.score));
        tableWidget->setItem(row, 1, scoreItem);

        row++;
    }

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);
}

RankWidget::~RankWidget()
{
    delete ui;
}

