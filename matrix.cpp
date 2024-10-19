#include "matrix.h"
#include <iostream>
#include <fstream>
using namespace std;


default_random_engine eng {static_cast<uint32_t>(time(nullptr))};

void Matrix::spawn()
{
    gaming = true;
    int row, col;
    do {
        col = rand_x(eng);
        row = rand_y(eng);
    } while (!matrix[row][col].empty());
    matrix[row][col] = Block(col,row,rand_init_value());
}

void Matrix::spawn(int x, int y)
{
    gaming = true;
    if (!matrix[y][x].empty())
        return;
    matrix[y][x] = Block(x,y,rand_init_value());
}

bool Matrix::update(bool& is_win)
{
    bool fully_updating = true;
    for (auto& r : matrix)
        for (auto& d : r) {
            d.update();
            if (!d.finish_mov())
                fully_updating = false;
        }
    if (fully_updating and updating)
        return final_exam(is_win);
    return false;
}

void Matrix::draw(QPixmap* dest) const
{
    QPixmap mat(mat_width*num_size.width() + (mat_width-1) * num_gap, mat_height * num_size.height() + (mat_height-1) * num_gap);
    QPainter mat_painter {&mat};
    mat_painter.setBrush(QBrush(mat_bgc));
    mat_painter.drawRect(0, 0, mat.width(), mat.height());
    mat_painter.end();
    for (auto& r : matrix)
        for (auto& d : r)
            d.draw(&mat);
    QPainter painter {dest};
    painter.drawPixmap(mat_pos, mat);
}

void Matrix::deal_input(Direction d)
{
    updating = true;
    switch (d) {
    case Direction::up:
        if (can_up()) {
            go_up();
            spa_new = true;
        }
        break;
    case Direction::down:
        if (can_down()) {
            go_down();
            spa_new = true;
        }
        break;
    case Direction::left:
        if (can_left()) {
            go_left();
            spa_new = true;
        }
        break;
    case Direction::right:
        if (can_right()) {
            go_right();
            spa_new = true;
        }
        break;
    }
}

bool Matrix::win() const
{
    for (auto& r : matrix)
        for (auto& d : r)
            if (d.win())
                return true;
    return false;
}

const Block* Matrix::win_pos() const
{
    for (auto& r : matrix)
        for (auto& d : r)
            if (d.win())
                return &d;
    return nullptr;
}

QList<QList<Block>> Matrix::empty_mat() const
{
    QList<QList<Block>> empty_mat;
    for (int i{}; i!=height; ++i) {
        QList<Block> line;
        for (int j{}; j!=width; ++j)
            line.append(Block{j,i});
        empty_mat.append(line);
    }
    return empty_mat;
}

void Matrix::read_mat()
{
    ifstream file("history.txt", ios::in);
    if (file.is_open()) {
        const int max_num = 8193;
        int cnt[max_num] = {0};
        int new_score = 0;
        cnt[4] = 4;
        for(int i = 8; i <= max_num; i *= 2) {
            cnt[i] = i + 2*cnt[i/2];
        }

        //int mat_height = 4;
        //int mat_width = 4;
        //int a[mat_height][mat_width] = {0};
        int a[4][4] = {0};

        for(int i = 0; i < mat_height; ++i) {
            for(int j = 0; j < mat_width; ++j) {
                file >> a[i][j];
                new_score += cnt[a[i][j]];
                matrix[i][j].value = a[i][j];
            }
        }

        score = new_score;
        file.close();
    } else {
        cout << "read file failure!" << endl;
        ofstream newFile("history.txt", ios::out);
        newFile << "0 0 0 0\n0 0 0 0\n0 0 2 0\n0 0 0 0";
        newFile.close();

        read_mat();
    }
}

void Matrix::write_mat()
{
    ofstream file("history.txt", ios::out | ios::trunc);
    if (file.is_open()) {
        for (auto& r : matrix){
            for (auto& d : r)
                file<<d.value<<" ";
            file<<endl;
        }
        file.close();
    } else {
        cout << "写入文件失败！" << endl;
    }
}

void Matrix::go_up()
{
    for (int r{1}; r!=height; ++r)
        for (int d{}; d!=width; ++d) {
            if (matrix[r][d].empty())
                continue;
            complex upper {find_up(r, d)};
            if (matrix[upper.real()][upper.imag()] == matrix[r][d]) {
                score+=2*matrix[r][d].value;    //更新得分

                matrix[r][d].mov(matrix[upper.real()][upper.imag()]);   // 产生移动的动态效果
                ++matrix[upper.real()][upper.imag()];       // 目标位置value更新
                matrix[r][d].clear();                       // 当前位置value清空
            }
            else if (matrix[upper.real()][upper.imag()].empty())
                matrix[r][d].mov(upper.imag(), upper.real());
            else
                matrix[r][d].mov(upper.imag(), matrix[upper.real()][upper.imag()].pos().imag()+1);
        }
}

void Matrix::go_down()
{
    for (int r{height-2}; r!=-1; --r)
        for (int d{}; d!=width; ++d) {
            if (matrix[r][d].empty())
                continue;
            complex downer {find_down(r, d)};
            if (matrix[downer.real()][downer.imag()] == matrix[r][d]) {
                score+=2*matrix[r][d].value;

                matrix[r][d].mov(matrix[downer.real()][downer.imag()]);
                ++matrix[downer.real()][downer.imag()];
                matrix[r][d].clear();
            }
            else if (matrix[downer.real()][downer.imag()].empty())
                matrix[r][d].mov(downer.imag(), downer.real());
            else
                matrix[r][d].mov(downer.imag(), matrix[downer.real()][downer.imag()].pos().imag()-1);
        }
}

void Matrix::go_left()
{
    for (int c{1}; c!=width; ++c)
        for (int r{}; r!=height; ++r) {
            if (matrix[r][c].empty())
                continue;
            complex lefter {find_left(r, c)};
            if (matrix[lefter.real()][lefter.imag()] == matrix[r][c]) {
                score+=2*matrix[r][c].value;

                matrix[r][c].mov(matrix[lefter.real()][lefter.imag()]);
                ++matrix[lefter.real()][lefter.imag()];
                matrix[r][c].clear();
            }
            else if (matrix[lefter.real()][lefter.imag()].empty())
                matrix[r][c].mov(lefter.imag(), lefter.real());
            else
                matrix[r][c].mov(matrix[lefter.real()][lefter.imag()].pos().real()+1, lefter.real());
        }
}

void Matrix::go_right()
{
    for (int c{width-2}; c!=-1; --c)
        for (int r{}; r!=height; ++r) {
            if (matrix[r][c].empty())
                continue;
            complex righter {find_right(r, c)};
            if (matrix[righter.real()][righter.imag()] == matrix[r][c]) {
                score+=2*matrix[r][c].value;

                matrix[r][c].mov(matrix[righter.real()][righter.imag()]);
                ++matrix[righter.real()][righter.imag()];
                matrix[r][c].clear();
            }
            else if (matrix[righter.real()][righter.imag()].empty())
                matrix[r][c].mov(righter.imag(), righter.real());
            else
                matrix[r][c].mov(matrix[righter.real()][righter.imag()].pos().real()-1, righter.real());
        }
}

int Matrix::test_go_up()
{
    int ret = 0;
    for (int r{1}; r!=height; ++r)
        for (int d{}; d!=width; ++d) {
            if (matrix[r][d].empty())
                continue;
            complex upper {find_up(r, d)};
            if (matrix[upper.real()][upper.imag()] == matrix[r][d]) {
                score+=2*matrix[r][d].value;

                matrix[r][d].mov(matrix[upper.real()][upper.imag()]);
                ++matrix[upper.real()][upper.imag()];
                matrix[r][d].clear();
            }
            else if (matrix[upper.real()][upper.imag()].empty())
                matrix[r][d].mov(upper.imag(), upper.real());
            else
                matrix[r][d].mov(upper.imag(), matrix[upper.real()][upper.imag()].pos().imag()+1);
        }
    return ret;
}

int Matrix::test_go_down()
{
    int ret = 0;
    for (int r{height-2}; r!=-1; --r)
        for (int d{}; d!=width; ++d) {
            if (matrix[r][d].empty())
                continue;
            complex downer {find_down(r, d)};
            if (matrix[downer.real()][downer.imag()] == matrix[r][d]) {
                ret+=2*matrix[r][d].value;

                matrix[r][d].mov(matrix[downer.real()][downer.imag()]);
                ++matrix[downer.real()][downer.imag()];
                matrix[r][d].clear();
            }
            else if (matrix[downer.real()][downer.imag()].empty())
                matrix[r][d].mov(downer.imag(), downer.real());
            else
                matrix[r][d].mov(downer.imag(), matrix[downer.real()][downer.imag()].pos().imag()-1);
        }
    return ret;
}

int Matrix::test_go_left()
{
    int ret = 0;
    for (int c{1}; c!=width; ++c)
        for (int r{}; r!=height; ++r) {
            if (matrix[r][c].empty())
                continue;
            complex lefter {find_left(r, c)};
            if (matrix[lefter.real()][lefter.imag()] == matrix[r][c]) {
                ret+=2*matrix[r][c].value;

                matrix[r][c].mov(matrix[lefter.real()][lefter.imag()]);
                ++matrix[lefter.real()][lefter.imag()];
                matrix[r][c].clear();
            }
            else if (matrix[lefter.real()][lefter.imag()].empty())
                matrix[r][c].mov(lefter.imag(), lefter.real());
            else
                matrix[r][c].mov(matrix[lefter.real()][lefter.imag()].pos().real()+1, lefter.real());
        }
    return ret;
}

int Matrix::test_go_right()
{
    int ret = 0;
    for (int c{width-2}; c!=-1; --c)
        for (int r{}; r!=height; ++r) {
            if (matrix[r][c].empty())
                continue;
            complex righter {find_right(r, c)};
            if (matrix[righter.real()][righter.imag()] == matrix[r][c]) {
                ret+=2*matrix[r][c].value;

                matrix[r][c].mov(matrix[righter.real()][righter.imag()]);
                ++matrix[righter.real()][righter.imag()];
                matrix[r][c].clear();
            }
            else if (matrix[righter.real()][righter.imag()].empty())
                matrix[r][c].mov(righter.imag(), righter.real());
            else
                matrix[r][c].mov(matrix[righter.real()][righter.imag()].pos().real()-1, righter.real());
        }
    return ret;
}

int Matrix::test_move(int index)
{
    int valid = false;
    int score_get = 0;
    //Matrix first_move_matrix = matrix;
    switch (index) {
    case 0:
        valid = can_up();
        if(valid) {
            score_get += test_go_up();
        }
        break;
    case 1:
        valid = can_left();
        if(valid) {
            score_get += test_go_left();
        }
        break;
    case 2:
        valid = can_down();
        if(valid) {
            score_get += test_go_down();
        }
        break;
    case 3:
        valid = can_right();
        if(valid) {
            score_get += test_go_right();
        }
        break;
    default:
        break;
    }

    if(!valid) return -1;
    else return score_get;
}

bool Matrix::can_up() const
{
    for (int r{1}; r!=height; ++r)
        for (int d{}; d!=width; ++d) {
            if (matrix[r][d].empty())
                continue;
            complex upper {find_up(r, d)};
            if (matrix[r][d]==matrix[upper.real()][upper.imag()] or matrix[upper.real()][upper.imag()].empty() or matrix[upper.real()+1][upper.imag()].empty())
                return true;
        }
    return false;
}

bool Matrix::can_down() const
{
    for (int r{height-2}; r!=-1; --r)
        for (int d{}; d!=width; ++d) {
            if (matrix[r][d].empty())
                continue;
            complex downer {find_down(r, d)};
            if (matrix[downer.real()][downer.imag()].empty() or matrix[r][d]==matrix[downer.real()][downer.imag()] or matrix[downer.real()-1][downer.imag()].empty())
                return true;
        }
    return false;
}

bool Matrix::can_left() const
{
    for (int c{1}; c!=width; ++c)
        for (int r{}; r!=height; ++r) {
            if (matrix[r][c].empty())
                continue;
            complex lefter {find_left(r, c)};
            if (matrix[lefter.real()][lefter.imag()].empty() or matrix[r][c]==matrix[lefter.real()][lefter.imag()] or matrix[lefter.real()][lefter.imag()+1].empty())
                return true;
        }
    return false;
}

bool Matrix::can_right() const
{
    for (int c{width-2}; c!=-1; --c)
        for (int r{}; r!=height; ++r) {
            if (matrix[r][c].empty())
                continue;
            complex righter {find_right(r, c)};
            if (matrix[righter.real()][righter.imag()].empty() or matrix[r][c]==matrix[righter.real()][righter.imag()] or matrix[righter.real()][righter.imag()-1].empty())
                return true;
        }
    return false;
}

std::complex<int> Matrix::find_up(int r, int c) const
{
    for (int i{r-1}; i!=-1; --i)
        if (!matrix[i][c].empty())
            return complex{i,c};
    return complex{0,c};
}

std::complex<int> Matrix::find_down(int r, int c) const
{
    for (int i{r+1}; i!=height; ++i)
        if (!matrix[i][c].empty())
            return complex{i,c};
    return complex{height-1,c};
}

std::complex<int> Matrix::find_left(int r, int c) const
{
    for (int i{c-1}; i!=-1; --i)
        if (!matrix[r][i].empty())
            return complex{r,i};
    return complex{r,0};
}

std::complex<int> Matrix::find_right(int r, int c) const
{
    for (int i{c+1}; i!=width; ++i)
        if (!matrix[r][i].empty())
            return complex{r,i};
    return complex{r,width-1};
}

/*void Matrix::setActionCallback(const ActionCallback& callback) {
    actionCallback = callback;
}*/

bool Matrix::final_exam(bool& is_win)
{
    updating = false;
    auto new_matrix = empty_mat();
    for (auto& r : matrix)
        for (auto& d : r) {
            if (d.empty())
                continue;
            d.final_exam();
            complex pos = d.pos();
            new_matrix[pos.imag()][pos.real()] = d;
        }
    matrix = new_matrix;

    if (win()) {
        is_win = true;
        return true;
    }
    if (spa_new) {
        spawn();
        spa_new = false;
    }
    if (lose()) {
        is_win = false;
        return true;
    }
    return false;
}
