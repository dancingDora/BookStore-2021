#ifndef BOOKSTORE_USERMANAGER_H
#define BOOKSTORE_USERMANAGER_H
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <cstring>
#include "blocklist.h"

using namespace std;

class User {
public:
    int privilege;
    int newBook;//记录该账户最后一次选的书
    char id[32];
    char passwd[32];
    char name[32];

    User();

    User &operator=(const User &arg);
};

class UserManager {
private:

    //定义枚举类型userStringTypeEnum
    enum userStringTypeEnum {
        stringId, stringPasswd, stringName
    };
    blocklist id_cmd;//用块状链表维护ID
    const string fname;
    fstream fs, fs1;
    int userNumber;//记录vector里的（已登陆的）非游客账号数量
    vector<User> userStack;
    //函数
    inline bool userStringCheck(userStringTypeEnum userStringType, string arg);//判断字符串合法性
    inline User freadUser(const int &offset);//在id存储文件中取出offset地址的User信息
public:
    UserManager();

    ~UserManager();

//辅助函数
    bool privilegeCheck(const int &privilegeNeed);

    void changeSelect(const int &offset);

//命令处理函数
    int userSelect();

    void su(const string &id, const string &passwd);

    void logout();

    void reg(const string &id, const string &passwd, const string &name);

    void useradd(const string &id, const string &passwd, const int &privilege, const string &name,
                 const int &registerFlag = 0);

    void repwd(const string &id, const string &oldpwd, const string &newpwd);

    void del(const string &id);
};

#endif //BOOKSTORE_USERMANAGER_H
