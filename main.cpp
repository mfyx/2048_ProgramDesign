#include "mainwindow.h"
#include "initinterface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InitInterface initInterface;

    MainWindow w(nullptr, &initInterface.rank_list);
    MainWindow w2(nullptr, &initInterface.rank_list);
    w.ai_mode = false;
    w2.ai_mode = false;


    QObject::connect(&initInterface, &InitInterface::startGame, [&](int mode) {
        initInterface.hide();
        w2.back_button->hide();

        if(mode == NormalMode || mode == AiMode) {
            w.ai_mode = mode;   //NormalMode的值0对应非AI模式，AiMode的值1对应AI模式，所以可以直接赋值
            w.show();
        }
        if(mode == vsAiMode) {
            w.ai_mode = false;
            w2.ai_mode = true;
            w.move(50+650, 50);
            w2.move(50, 50);
            w.show();
            w2.show();
        }
    } );

    QObject::connect(&w, &MainWindow::backInitInterface, [&]() {
        initInterface.show();

        w.hide();
        w2.hide();
        w.ai_mode = false;
        w2.ai_mode = false;
        w.start_game();
        w2.start_game();

        initInterface.show();
    } );


    initInterface.show();

    return a.exec();
}

