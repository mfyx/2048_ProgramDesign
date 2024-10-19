#include "block.h"

bool Block::finish_mov() const
{
    if (!moving)
        return true;
    QRect now {calc_this_rect()};
    QRect dest {calc_rect(x_dest, y_dest)};
    if (x_mov > 0)
        return now.x()>=dest.x();
    else if (x_mov < 0)
        return now.x()<=dest.x();
    else
        return (y_mov>0) ? now.y()>=dest.y() : now.y()<=dest.y();
}

void Block::draw(QPixmap* dest) const
{
    complex colors {QColor{},QColor{}};
    switch (show_value) {
    case 2:
        colors = color_2;
        break;
    case 4:
        colors = color_4;
        break;
    case 8:
        colors = color_8;
        break;
    case 16:
        colors = color_16;
        break;
    case 32:
        colors = color_32;
        break;
    case 64:
        colors = color_64;
        break;
    case 128:
        colors = color_128;
        break;
    case 256:
        colors = color_256;
        break;
    case 512:
        colors = color_512;
        break;
    case 1024:
        colors = color_1024;
        break;
    case 2048:
        colors = color_2048;
        break;
    case 4096:
        colors = color_4096;
        break;
    case 8192:
        colors = color_8192;
        break;
    case 16384:
        colors = color_16384;
        break;
    default:
        return;
    }
    QPainter painter {dest};
    painter.setBrush(QBrush{colors.real()});
    painter.setPen(QPen{colors.imag()});
    painter.setFont(num_font);
    QRect box {calc_this_rect()};
    painter.drawRoundedRect(box, box.width()*num_rect_radius, box.height()*num_rect_radius);
    painter.drawText(box, Qt::AlignCenter, QString{}.setNum(show_value));
}

void Block::mov(int dx, int dy)
{
    moving = true;
    x_dest = dx;
    y_dest = dy;
    x_dist = dx-x;
    y_dist = dy-y;
}

void Block::mov(const Block &destination)
{
    moving = true;
    x_dest = destination.x_dest;
    y_dest = destination.y_dest;
    x_dist = x_dest-x;
    y_dist = y_dest-y;
}

void Block::update()
{
    if (moving) {
        x_mov += x_dist*num_speed;
        y_mov += y_dist*num_speed;
    }
}

void Block::final_exam()
{
    show_value = value;
    x = x_dest;
    y = y_dest;
    x_mov = 0;
    y_mov = 0;
    moving = false;
}
