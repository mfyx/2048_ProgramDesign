#ifndef BLOCK_H
#define BLOCK_H

#include <QObject>
#include <QList>
#include <random>
#include <ctime>
#include <QPixmap>
#include <QPainter>
#include <cmath>

#include "config.h"

enum class Direction { up, down, left, right };
enum class Move_type { horizontal, vertical };

class Block {        // Block类是一个可视化数字
public:
    Block(int xx, int yy, int val =0) : value{val}, show_value{val}, x{xx}, y{yy}, x_mov{}, y_mov{}, x_dest{xx}, y_dest{yy}, moving{false} { }
    bool finish_mov() const;        // 判断移动是否完成
    bool operator==(const Block& source) const { return value==source.value; }
    bool empty() const { return value==0; }
    void draw(QPixmap* dest) const;     // 绘制自己
    void mov(int dx, int dy);       // 下达移动指令，移动到目标位置
    void mov(const Block& destination);      // 重载移动函数，移动到对应destination的目标所在位置
    void update();      // 每一帧更新自己
    void operator++() { value *= 2; }
    bool win() const { return value>=num_win; }
    complex<int> pos() const { return complex{x_dest,y_dest}; }
    void final_exam();      // 期末考试，在移动完毕之后更新其数据
    void clear() { value = 0; }
    QRect calc_this_rect() const { return QRect{QPoint{x*(num_size.width()+num_gap),y*(num_size.height()+num_gap)}+QPoint{x_mov,y_mov},num_size}; }

    int value;
private:
    int show_value;
    int x;
    int y;
    int x_mov;
    int y_mov;
    int x_dist;
    int y_dist;
    int x_dest;
    int y_dest;
    bool moving;
    Direction mov_d;
    static QRect calc_rect(int x, int y) { return QRect{QPoint{x*(num_size.width()+num_gap),y*(num_size.height()+num_gap)},num_size}; }
};

#endif // BLOCK_H
