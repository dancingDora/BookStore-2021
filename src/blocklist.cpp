#include "blocklist.h"

//Node:
Node::Node() {
    offset = -1;
    memset(str, '\0', sizeof(str));
}

Node::Node(const int &_offset, const string &arg) {
    offset = _offset;
    memset(str, '\0', sizeof(str));
    strcpy(str, arg.c_str());
}

Node &Node::operator=(const Node &arg) {
    offset = arg.offset;
    strcpy(str, arg.str);
    return *this;
}

bool Node::operator==(const Node &arg) const {
    if (arg.offset != offset)return false;
    string s1 = arg.str, s2 = str;
    return str == arg.str;
}

bool Node::operator<(const Node &arg) const {
    return str < arg.str;
}

bool Node::operator<=(const Node &arg) const {
    return str <= arg.str;
}

//Block:
Block::Block() {
    nxt = -1;
    Node_num = 0;
}

Block &Block::operator=(const Block &arg) {
    nxt = arg.nxt;
    Node_num = arg.Node_num;
    for (int i = 0; i < Node_num; ++i)node[i] = arg.node[i];
    for (int i = Node_num; i < 330; ++i)node[i] = Node();
    return *this;
}

//blocklist:
blocklist::blocklist(const string &arg) : fname(arg) {
    fs.open(fname, ALL);
    if (!fs.is_open()) {
        fs.close();
        fs.open(fname, ALL | ios::trunc);//trunc会新建本地文件
        Block InitBlock;//在刚开始写入一个空块
        InitBlock.Node_num = 0;
        InitBlock.nxt = -1;
        for (int i = 0; i < 330; ++i)InitBlock.node[i] = Node();
        fs.seekp(0, ios::beg);
        fs.write(reinterpret_cast<char *>(&InitBlock), sizeof(InitBlock));
    }
    fs.close();
}

blocklist::~blocklist() {
    fs.close();
    fs1.close();
}

inline int blocklist::next(const int &offset) {//找到下一个块的offset
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(offset, ios::beg);
    int nxt_;
    fs.read(reinterpret_cast<char *>(&nxt_), sizeof(int));
    fs.close();
    return nxt_;
}


int blocklist::find_Block(const string &arg) {//找到要插入的点的位置,返回其在文件中的偏移量
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(0, ios::beg);
    int nxt_ = 0, now = 0, last = 0;
    char *temp = new char[64];
    string get_temp;
    while (nxt_ != -1) {
        last = now;
        now = nxt_;
        fs.read(reinterpret_cast<char *>(&nxt_), sizeof(int));//读取这个block的nxt数据
        Block tempB;
        fs.seekg(0, ios::beg);
        fs.read(reinterpret_cast<char *>(&tempB), sizeof(tempB));
        fs.clear();
        fs.seekg(now + 12, ios::beg);
        fs.read(temp, 64);
        get_temp = temp;//读取这个block中第一个Node的str值用于比较

        if (arg < get_temp)break;
        else if (nxt_ == -1) {
            last = now;
            break;
        }
        fs.clear();
        fs.seekg(nxt_, ios::beg);
    }
    delete[] temp;
    fs.close();
    return last;
}


int blocklist::find_Node(const string &arg) {
    int offset = find_Block(arg);//找到这个node所应该在的块的偏移量
    //取出对应Block的值
    Block tempBlock;
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(offset, ios::beg);
    fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    int pos = 0;
    for (pos = 0; pos < tempBlock.Node_num; ++pos) {
        string tempStr = tempBlock.node[pos].str;
        if (arg == tempStr)break;
        if (arg < tempStr) {
            pos = tempBlock.Node_num;
            break;
        }
    }
    return pos == tempBlock.Node_num ? -1 : tempBlock.node[pos].offset;
}

void blocklist::find_Node(const string &arg, vector<int> &alloc) {
    int offset = find_Block(arg);
    Block tempBlock;
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(offset, ios::beg);
    fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    for (int i = 0; i < tempBlock.Node_num; ++i) {
        string tempStr = tempBlock.node[i].str;
        if (arg == tempStr)alloc.push_back(tempBlock.node[i].offset);
        if (arg < tempStr)break;//理由同上
    }
}

void blocklist::merge(const int &offset1, const int &offset2) {
    Block tempBlock1, tempBlock2;
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(offset1, ios::beg);
    fs.read(reinterpret_cast<char *>(&tempBlock1), sizeof(tempBlock1));
    fs.clear();
    fs.seekg(offset2, ios::beg);
    fs.read(reinterpret_cast<char *>(&tempBlock2), sizeof(tempBlock2));
    fs.close();
    //合并相邻的两个块
    for (int i = 0; i < tempBlock2.Node_num; ++i)tempBlock1.node[i + tempBlock1.Node_num] = tempBlock2.node[i];
    tempBlock1.Node_num += tempBlock2.Node_num;
    tempBlock1.nxt = tempBlock2.nxt;
    //更新块状链表信息
    fs.open(fname, ios::out | ios::binary);
    fs.clear();
    fs.seekp(offset1, ios::beg);
    fs.write(reinterpret_cast<char *>(&tempBlock1), sizeof(tempBlock1));
    fs.close();
}


void blocklist::split(const int &offset, const int &cut_point) {
    Block tempBlock, newBlock = Block();
    //读入信息
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(offset, ios::beg);
    fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    for (int i = 0; i < tempBlock.Node_num - HALF_SIZE; ++i) {
        newBlock.node[i] = tempBlock.node[i + HALF_SIZE];
        tempBlock.node[i + HALF_SIZE] = Node();
    }
    newBlock.Node_num = tempBlock.Node_num - HALF_SIZE;
    tempBlock.Node_num = HALF_SIZE;
    newBlock.nxt = tempBlock.nxt;
    fs.open(fname, ALL);
    fs.clear();
    fs.seekp(0, ios::end);
    fs.clear();
    tempBlock.nxt = (int) fs.tellp();
    //更新块状链表信息
    fs.write(reinterpret_cast<char *>(&newBlock), sizeof(newBlock));
    fs.clear();
    fs.seekp(offset, ios::beg);
    fs.write(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
}


void blocklist::add_Node(const Node &arg) {
    int offset = find_Block(arg.str);
    Block tempBlock;
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(offset, ios::beg);
    fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    int pos = 0;
    for (pos = 0; pos < tempBlock.Node_num; ++pos)
        if (arg <= tempBlock.node[pos])break;

    for (int i = tempBlock.Node_num; i > pos; --i)tempBlock.node[i] = tempBlock.node[i - 1];
    tempBlock.Node_num++;
    tempBlock.node[pos] = arg;
    //转为写入模式
    fs.open(fname, ios::out | ios::binary);
    fs.clear();
    fs.seekp(offset, ios::beg);
    fs.write(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    if (tempBlock.Node_num > 330 - 10)split(offset, HALF_SIZE);
}


int blocklist::del_Node(const string &arg) {

    int offset = find_Block(arg);
    Block tempBlock;
    //读入tempBlock
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(offset, ios::beg);
    fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    //找到arg所在的位置pos
    int pos;
    for (pos = 0; pos < tempBlock.Node_num; ++pos) {
        string tempStr = tempBlock.node[pos].str;
        if (arg == tempStr)break;
        if (arg < tempStr) {
            pos = tempBlock.Node_num;
            break;
        }//如果此时的字典序已经大于arg了，那么就意味着找不到了，直接break
    }
    if (pos == tempBlock.Node_num)return -1;
    Node tempNode = tempBlock.node[pos];
    tempBlock.Node_num--;
    for (int i = pos; i < tempBlock.Node_num; ++i)tempBlock.node[i] = tempBlock.node[i + 1];
    tempBlock.node[tempBlock.Node_num] = Node();
    //转为写入模式
    fs.open(fname, ALL);
    fs.clear();
    fs.seekp(offset, ios::beg);
    fs.write(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    //转换为读取状态
    fs.open(fname, ALL);
    fs.clear();
    while (tempBlock.nxt != -1) {
        Block nextBlock;
        fs1.seekg(tempBlock.nxt, ios::beg);
        fs1.read(reinterpret_cast<char *>(&nextBlock), sizeof(nextBlock));
        if (tempBlock.Node_num + nextBlock.Node_num <= 330 - 10) {
            merge(offset, tempBlock.nxt);
            fs.seekg(offset, ios::beg);
            fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
        } else break;//如果不能继续合并就退出去
    }
    fs.close();
    return tempNode.offset;
}

int blocklist::del_Node(const Node &arg) {
    int offset = find_Block(arg.str);
    Block tempBlock;
    //读取tempBlock
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(offset, ios::beg);
    fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    int pos;
    for (pos = 0; pos < tempBlock.Node_num; ++pos) {
        if (arg == tempBlock.node[pos])break;
        if (arg < tempBlock.node[pos]) {
            pos = tempBlock.Node_num;
            break;
        }
    }
    if (pos == tempBlock.Node_num) return -1;
    Node tempNode = tempBlock.node[pos];
    tempBlock.Node_num--;
    for (int i = pos; i < tempBlock.Node_num; ++i)tempBlock.node[i] = tempBlock.node[i + 1];
    tempBlock.node[tempBlock.Node_num] = Node();
    //转为写入模式
    fs.seekp(offset, ios::beg);
    fs.write(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    fs.close();
    //转为读取模式
    fs.open(fname, ALL);
    fs.clear();
    while (tempBlock.nxt != -1) {
        Block nextBlock;
        fs1.seekg(tempBlock.nxt, ios::beg);
        fs1.read(reinterpret_cast<char *>(&nextBlock), sizeof(nextBlock));
        if (tempBlock.Node_num + nextBlock.Node_num <= 330 - 10) {
            merge(offset, tempBlock.nxt);
            fs.seekg(offset, ios::beg);
            fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
        } else break;//如果能合并相邻块就一直合并，直到不能合并退出来
    }
    fs.close();
    return tempNode.offset;
}

void blocklist::showDetail() {
    fs.open(fname, ALL);
    fs.clear();
    fs.seekg(0, ios::beg);
    Block tempBlock;
    fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    cout << fname << " Inf:" << endl;
    while (1) {
        cout << "Node_num=" << tempBlock.Node_num << " next_offset=" << tempBlock.nxt << endl;
        for (int i = 0; i < tempBlock.Node_num; ++i)cout << tempBlock.node[i].str << " ";
        puts("");
        if (tempBlock.nxt == -1)break;
        fs.clear();
        fs.seekg(tempBlock.nxt, ios::beg);
        fs.read(reinterpret_cast<char *>(&tempBlock), sizeof(tempBlock));
    }
    fs.close();
}

