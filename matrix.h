#ifndef MATRIX_H
#define MATRIX_H

#include <QObject>
#include <QList>
#include <random>
#include <ctime>
#include <QPixmap>
#include <QPainter>
#include <cmath>

#include "config.h"
#include "block.h"

using std::default_random_engine;
using std::uniform_int_distribution;

extern default_random_engine eng;       // eng是一个随机数引擎，可以看作随机数种子

class Matrix {
public:
    Matrix(int mwidth =mat_width, int mheight =mat_height) : width{mwidth}, height{mheight},
        rand_x{0,mwidth-1}, rand_y{0,mheight-1}, updating{false}, spa_new{false}, gaming{false} {
        matrix = empty_mat();
    }

    void spawn();       // 随机位置加入一个2048数字对象
    void spawn(int x, int y);       // 指定位置加入一个2048数字对象
    bool update(bool& is_win);      // 每一帧更新自己，返回是否胜负已分
    void draw(QPixmap* dest) const;       // 绘制自己
    bool is_updating() const { return updating; }
    void deal_input(Direction d);       // 处理输入
    bool win() const;       // 是否胜利
    const Block* win_pos() const;        // 胜利之后使用此函数，返回胜利数字的指针
    bool lose() const { return !can_left() and !can_right() and !can_up() and !can_down(); }

    bool can_up() const;
    bool can_down() const;
    bool can_left() const;
    bool can_right() const;

    int test_go_up();
    int test_go_down();
    int test_go_left();
    int test_go_right();
    int test_move(int index);

    bool final_exam(bool& is_win);      // 期末考试，在移动完毕之后更新并整理数据，是龙是鼠，一测便知。返回是否刚刚分出胜负

    void read_mat();
    void write_mat();

    int score=0;
    bool game_return = false;

private:
    int width;
    int height;
    QList<QList<Block>> matrix;
    QList<QList<Block>> empty_mat() const;       // 返回一个空的数字矩阵
    static int rand_init_value() { return static_cast<int>(pow(2, uniform_int_distribution<>{num_init.real(),num_init.imag()}(eng))); }
    uniform_int_distribution<> rand_x;
    uniform_int_distribution<> rand_y;
    bool updating;

    void go_up();
    void go_down();
    void go_left();
    void go_right();
    complex<int> find_up(int r, int c) const;
    complex<int> find_down(int r, int c) const;
    complex<int> find_left(int r, int c) const;
    complex<int> find_right(int r, int c) const;
    bool spa_new;
    bool gaming;
};


#endif // MATRIX_H
