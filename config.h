#ifndef CONFIG_H
#define CONFIG_H

#include <QSize>
#include <QFont>
#include <complex>
#include <QColor>
#include <QPoint>
#include <string>

using std::complex;
using namespace std;

// 游戏配置
//const QSize game_size {312, 480};      // 游戏大小
const QSize game_size {600, 860};      // 游戏大小  600 = 4*128 + 2*32 + 3*8
//const QSize game_size {1024, 900};      // 游戏大小
//const QSize game_size {512-128-64-8, 512-32};      // 游戏大小
const QColor game_bgc {"#BBADA0"};      // 游戏背景颜色
//const QColor game_bgc {"#CCCCCC"};      // 游戏背景颜色

// 矩阵配置
static int mat_length = 4;
static int mat_width = mat_length;        // 矩阵的宽
static int mat_height = mat_length;       // 矩阵的高
//const QColor mat_bgc {"#FFFFFF"};       // 矩阵的背景颜色
const QColor mat_bgc {"#DED4BE"};       // 矩阵的背景颜色
//const QPoint mat_pos {16,16};       // 矩阵的位置
const QPoint mat_pos {32,32};       // 矩阵的位置

// 帧率配置
const int frame_ps {64};        // 游戏帧率

// 数字配置
//const int num_speed {15};        // 移动速度
const int num_speed {15};        // 移动速度
//const QSize num_size {64,64};       // 数字的大小
const QSize num_size {128,128};       // 数字的大小
const double num_rect_radius {0.16};        // 圆角矩形弯曲幅度与大小之比
const QFont num_font {QString{"黑体"},36};        // 真·数字的大小
const int num_gap {8};      // 数字之间的间隔
const int num_win {2048};       // 达到这个数字就赢了
const complex num_init {1,1};       // 随机初始值范围，2的n次方

// 2048传统主题色配置
const complex color_2 {QColor{"#EEE4DA"}, QColor{"#776E65"}};     // 数字2：浅灰色背景，深灰色前景
const complex color_4 {QColor{"#EDE0C8"}, QColor{"#776E65"}};     // 数字4：深灰色背景，深灰色前景
const complex color_8 {QColor{"#F2B179"}, QColor{"#F9F6F2"}};     // 数字8：橙色背景，白色前景
const complex color_16 {QColor{"#F59563"}, QColor{"#F9F6F2"}};    // 数字16：深橙色背景，白色前景
const complex color_32 {QColor{"#F67C5F"}, QColor{"#F9F6F2"}};    // 数字32：红橙色背景，白色前景
const complex color_64 {QColor{"#F65E3B"}, QColor{"#F9F6F2"}};    // 数字64：深红色背景，白色前景
const complex color_128 {QColor{"#EDCF72"}, QColor{"#F9F6F2"}};   // 数字128：浅黄色背景，白色前景
const complex color_256 {QColor{"#EDCC61"}, QColor{"#F9F6F2"}};   // 数字256：浅黄色背景，白色前景
const complex color_512 {QColor{"#EDC850"}, QColor{"#F9F6F2"}};   // 数字512：浅黄色背景，白色前景
const complex color_1024 {QColor{"#EDC53F"}, QColor{"#F9F6F2"}};  // 数字1024：金色背景，白色前景
const complex color_2048 {QColor{"#EDC22E"}, QColor{"#F9F6F2"}};  // 数字2048：金色背景，白色前景
const complex color_4096 {QColor{}, QColor{}};                    // 可根据需要配置
const complex color_8192 {QColor{}, QColor{}};                    // 可根据需要配置
const complex color_16384 {QColor{}, QColor{}};                   // 可根据需要配置


#endif // CONFIG_H
