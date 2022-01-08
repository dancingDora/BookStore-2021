//
// Created by 陆逸凡 on 2021/12/22.
//

#ifndef BOOKSTORE_USER_LOG_H
#define BOOKSTORE_USER_LOG_H


#include<iostream>
#include<map>
#include<cstring>
#include<string>
#include'string_'

using namespace std;

class user {
private:
    map<string, person> mapUser;
    struct person {
        string user_id;
        string user_name;
        string user_password;
        int user_priority;
        bool online;
    } person_info;
public:
    user() {
    };

    ~user() {};

    bool su(string &_user_id, string &try_password = "") {
        if ((this->person_info.user_priority) > mapUser[_user_id].user_priority) {
            mapUser[_user_id].online = 1;
            return true;
        }
        if (try_password == user_info[_user_id].user_password) {
            mapUser[_user_id].online = 1;
            return true;
        } else return false;
    };

    void logout() {

    };
    bool register(string
    _user_id,
    string _password, string
    _user_name){
        if (mapUser.count(_user_id) != 0) {
            return false;
        }
        person person_info_1;
        person_info_1.user_id = _user_id;
        person_info_1.password = user_password;
        person_info_1.user_name = _user_name;
        person_info_1.user_priority = 1;
    };

    bool passwd(string &_user_id, string &old_password, string &new_password = "") {
        if (this->person.priority = 7) {
            mapUser[_user_id].user_password = old_password;
            return true;
        }
        if (old_password != mapUser[_user_id].user_password) {
            return false;
        }
        else  mapUser[_user_id].user_password=new_password;
    };

    bool useradd(string &_user_id,string &_password,int &_priority,string &_user_name) {
        if(_priority>=person.user_priority||mapUser.count(_user_id)!=0) return false;
        else{person person_info_1;
        person_info_1.user_id = _user_id;
        person_info_1.password = _password;
        person_info_1.user_name = _user_name;
        person_info_1.user_priority = _priority;
        mapUser.insert(_user_id,person_info_1);
        return true;}
    };

    void delete(){};
}

class log {

};

#endif //BOOKSTORE_USER_LOG_H
