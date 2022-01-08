#include<iostream>
#include<fstream>
#include<cstring>
#include<cctype>
#include<cmath>
#include<string>
#include<vector>

#define ALL (ios::in|ios::out|ios::binary)
#define HALF_SIZE 160
using namespace std;

class Node {
public:
    int offset;
    char str[64];

    Node();

    Node(const int &_offset, const string &arg);

    Node &operator=(const Node &arg);

    bool operator==(const Node &arg) const;

    bool operator<(const Node &arg) const;

    bool operator<=(const Node &arg) const;
};

class Block {
public:
    int nxt, Node_num;
    Node node[330];

    Block();

    Block &operator=(const Block &arg);
};

class blocklist {
private:
    const string fname;
    fstream fs, fs1;

    inline int next(const int &offset);
    void split(const int &offset, const int &cut_point);
    void merge(const int &offset1, const int &offset2);
    int find_Block(const string &);
public:

    blocklist(const string &);

    ~blocklist();

    int find_Node(const string &);
    void find_Node(const string &, vector<int> &);
    void add_Node(const Node &);
    int del_Node(const string &);
    int del_Node(const Node &);
    void showDetail();
};
