#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <string>
using namespace std;

struct UserInformation {
    UserInformation(string m_name, int m_score) : name(m_name), score(m_score) {}
    string name;
    int score;
    //int rank;
};

static bool compareByScore(const UserInformation& userInfo1, const UserInformation& userInfo2) {
    return userInfo1.score > userInfo2.score;  // 按照降序排列
}

#endif // USERINFORMATION_H
