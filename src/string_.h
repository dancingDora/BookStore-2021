//
// Created by 陆逸凡 on 2021/12/21.
//

#ifndef BOOKSTORE_STRING__H
#define BOOKSTORE_STRING__H

#include<iostream>
#include<cstring>
#include<string>

class exp {
private:
public:
    //将【】中内容取出
//    string takeToken(string &x) {
//        string ans;
//        int lhs, rhs;
//        for (int i = 0; i < size(x); i++) {
//            if (x[i] == '[')lhs = i;
//            if (x[i] == ']') {
//                rhs = i;
//                break;
//            }
//        }
//        for (int j = 0; j <= (rhs - lhs); j++) {
//            ans[j] = x[lhs + j];
//        }
//        return ans;
//    };
    string getToken(string &x) {
        int tmp = 0;
        string ans;
        for (; tmp < x.size(); tmp++) {
            if (x[tmp] != ' ') break;
        }
        for (int i = tmp; i < x.size(); i++) {
            ans[i-tmp]=x[i];
            if(x[i+1]==' ')return ans;
        }
    };
    //输出方括号的下标
    int findF;
    //输出圆括号的下标
    int findY;

    //把最前面的字符切掉
    string cutToken(string &x) {
        int tmp = 0;
        string ans;
        for (; tmp < x.size(); tmp++) {
            if (x[tmp] != ' ') break;
        }
        for (int i = tmp; i < x.size(); i++) {
            if (x[i] == ' ') {
                for (int j = i + 1; j < x.size(); j++) {
                    if (x[j] != ' ') {
                        tmp = j;
                        break;
                    }
                }
                break;
            }
        }
        for(int i=0;i<x.size()-tmp;i++) {
            ans[i]=x[i+tmp];
        }
        return ans;
    }
};





#endif //BOOKSTORE_STRING__H
