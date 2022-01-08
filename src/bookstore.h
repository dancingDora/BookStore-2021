//
// Created by 陆逸凡 on 2021/12/25.
//

#ifndef BOOKSTORE_BOOKSTORE_H
#define BOOKSTORE_BOOKSTORE_H
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include "usermanager.h"

using namespace std;

class Book {
public:
    char ISBN[24];
    char name[64];
    char author[64];
    char keyword[64];
    double price;
    int quantity;

    Book();

    Book &operator=(const Book &arg);

    bool operator<(const Book &arg) const;
};


class FileManager {
private:
    const string fnameBook, fnameFinance, fnameLog;//分别存放书名文件，收支统计文件，日志文件
    fstream fs;
public:
    FileManager();

    //对财务文件操作函数：
    inline void financeInit(int &tradeNum, double &income, double &expense);
    inline void financeInitWrite(int &num, double &income, double &expense);
    inline void freadFinance(const int &time);
    inline void financeWrite(double &price, bool &sgn);
    //对书籍文件操作函数:
    inline void bookInit(int &bookNum);
    inline void bookInitWrite(int &bookNum);
    inline void freadBook(int offset, Book &arg);
    inline void freadBook(vector<Book> &vec);
    inline int fwriteBook(Book &arg);
    inline void fwriteBook(int offset, Book &arg);
};


class Bookstore {
private:
    enum logTypeEnum {
        reportMyself, reportEmployee, reportLog, reportFinance
    };
    enum findTypeEnum {
        findName, findAuthor, findKeyword
    };
    enum bookStringTypeEnum {
        stringISBN, stringBookName, stringAuthor, stringKeyword
    };
    //变量
    blocklist isbn_cmd;
    blocklist name_cmd;
    blocklist author_cmd;
    blocklist keyword_cmd;//上边四个是对每个参数都维护一个块状数组，便于在sqrt(n)复杂度下对每个参数进行删，查，改，插入操作
    UserManager user_cmd;//用户信息类
    FileManager File_cmd;//控制文件修改和读写的类
    int tradeNumber;//总交易次数
    double totIncome, totExpense;//总收入、总支出
    int bookNumber;//图书总数
//辅助函数
    inline void splitString(string &arg, string &ret, int keywordFlag = 0);

    inline bool bookStringCheck(bookStringTypeEnum bookStringType, const string &arg);

    inline void printBook(const Book &arg);

    void addFinance(double price, bool sgn);

    int find(const string &ISBN);

    void find(findTypeEnum findType, const string &key,
              vector<int> &vec);
//功能函数
    void showLog(logTypeEnum logType);
    void showFinance(int time = -1);
    void import(const int &quantity, const double &price);

    void buy(const string &ISBN, const int &quantity);

    void select(const string &ISBN);

    void modify(const int &offset, const string &ISBN, string name, string author, string keyword, const double &price);

public:
    Bookstore();

    void operation(string cmd);

};

#endif //BOOKSTORE_BOOKSTORE_H
