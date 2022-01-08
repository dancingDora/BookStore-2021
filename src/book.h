#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H

#include<vector>
#include<iostream>
#include<string>
#include<fstream>

#define FILENAME "library.txt"

using namespace std;

#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <fstream>
#include 'user&log.h'

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::fstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {
        file.open(file_name, std::fstream::in | std::fstream::out);
        if (!file) {
            file.open(file_name, std::ofstream::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        file.close();
    }

    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        return;
    }

    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        return;
    }

    int write(T &t) {
        int index;
        int num_T, del_head;
        (*this).get_info(num_T, 1);
        (*this).get_info(del_head, 2);
        if (!del_head)
            index = info_len * sizeof(int) + num_T * sizeofT;
        else {
            index = del_head;
            file.open(file_name);
            file.seekg(del_head);
            file.read(reinterpret_cast<char *>(&del_head), sizeof(int));
            (*this).write_info(del_head, 2);
            file.close();
        }
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return index;
    }

    void update(T &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    void read(T &t, const int index) {
        file.open(file_name);
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    void Delete(int index) {
        int del_head;
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&del_head), sizeof(int));
        del_head = index;
        (*this).write_info(del_head, 2);
        file.close();
    }
};

class Pair {
private:
    int value;
    char key[67];
public:
    friend class Block;

    friend class BlockList;

    Pair() : key(""), value(0) {}

    Pair(const string &str, const int &val) {
        strcpy(key, str.c_str());
        value = val;
    }

    bool operator==(const Pair &tmp) const {
        if (value != tmp.value) return 0;
        if (strcmp(key, tmp.key) != 0)return 0;
        return 1;
    }

    bool operator!=(const Pair &tmp) const {
        return !(*this == tmp);
    }

    bool operator<(const Pair &tmp) const {
        if (!strcmp(key, tmp.key)) return value < tmp.value;
        return strcmp(key, tmp.key) < 0;
    }

    bool operator>(const Pair &tmp) const {
        return !(*this < tmp) && *this != tmp;
    }
};

class Block {
private:
    int pre_index, nxt_index, size, pos;
    Pair array[2000];
public:
    friend class BlockList;

    Block() = default;

    Block(const Block &tmp) {
        *this = tmp;
    }
};

class BlockList {
private:
    MemoryRiver<Block> Blocks;

public:
    BlockList() = default;

    BlockList(const string &name) : Blocks(name) {}


    void InsertPair(const Pair &tmp) {
        int num, file_pos = sizeof(int) * 2;
        Blocks.get_info(num, 1);
        if (!num) {
            Block new_block;
            new_block.size = 1;
            new_block.array[0] = tmp;
            new_block.pos = Blocks.write(new_block);
            Blocks.update(new_block, new_block.pos);
            num++;
            Blocks.write_info(num, 1);
        } else {
            Block tmp_block;
            int cnt = 0;
            for (int i = file_pos; i;) {
                cnt++;
                Blocks.read(tmp_block, i);
                if (tmp < tmp_block.array[tmp_block.size - 1] || cnt == num) {
                    int it = std::lower_bound(tmp_block.array, tmp_block.array + tmp_block.size, tmp) - tmp_block.array;
                    for (int j = tmp_block.size; j > it; --j) {
                        tmp_block.array[j] = tmp_block.array[j - 1];
                    }
                    tmp_block.array[it] = tmp;
                    tmp_block.size++;
                    Blocks.update(tmp_block, i);
                    break;
                }
                i = tmp_block.nxt_index;
            }
            if (tmp_block.size >= 330) {
                Split(tmp_block.pos);
            }
        }
    }

    void FindKey(const string &TmpK, vector<int> &ans) {
        int num, file_pos = sizeof(int) * 2;
        Blocks.get_info(num, 1);
        Block tmp_block;
        for (int i = file_pos; i;) {
            Blocks.read(tmp_block, i);
            if (TmpK < tmp_block.array[0].key) break;
            if (TmpK > tmp_block.array[tmp_block.size - 1].key) {
                i = tmp_block.nxt_index;
                continue;
            }
            for (int j = 0; j < tmp_block.size; ++j) {
                if (TmpK == tmp_block.array[j].key) {
                    ans.push_back(tmp_block.array[j].value);
                    continue;
                }
                if (TmpK < tmp_block.array[j].key) break;
            }
            i = tmp_block.nxt_index;
        }
    }

    void DeletePair(const Pair &tmp) {
        int num, file_pos = sizeof(int) * 2;
        Blocks.get_info(num, 1);
        Block tmp_block;
        for (int i = file_pos; i;) {
            Blocks.read(tmp_block, i);
            if (tmp < tmp_block.array[0] || tmp > tmp_block.array[tmp_block.size - 1]) {//不在该块中
                i = tmp_block.nxt_index;
                continue;
            } else {
                for (int j = 0; j < tmp_block.size; ++j) {
                    if (tmp_block.array[j] == tmp) {
                        tmp_block.size--;
                        for (int k = j; k < tmp_block.size; ++k)
                            tmp_block.array[k] = tmp_block.array[k + 1];
                        Blocks.update(tmp_block, i);
                        break;
                    } else if (tmp_block.array[j] > tmp) break;
                }
                i = tmp_block.nxt_index;
                continue;
            }
        }
    }

    void Split(const int &pos) {
        Block tmp_block, new_block;
        int org_size, num;
        Blocks.get_info(num, 1);
        num++;
        Blocks.write_info(num, 1);
        Blocks.read(tmp_block, pos);
        org_size = tmp_block.size;
        tmp_block.size /= 2;
        new_block.size = org_size - tmp_block.size;
        new_block.pre_index = tmp_block.pos;
        new_block.nxt_index = tmp_block.nxt_index;
        for (int i = 0; i < new_block.size; ++i)
            new_block.array[i] = tmp_block.array[i + tmp_block.size];
        new_block.pos = Blocks.write(new_block);
        tmp_block.nxt_index = new_block.pos;
        Blocks.update(tmp_block, tmp_block.pos);
        Blocks.update(new_block, new_block.pos);
    }

    void Merge(const int &pos) {
        if (pos == 8) return;
        Block tmp_block, pre_block, nxt_block;
        int num, org_size;
        Blocks.get_info(num, 1);
        num--;
        Blocks.write_info(num, 1);
        Blocks.read(tmp_block, pos);
        Blocks.read(pre_block, tmp_block.pre_index);
        if (tmp_block.nxt_index) {
            Blocks.read(nxt_block, tmp_block.nxt_index);
            nxt_block.pre_index = pre_block.pos;
            Blocks.update(nxt_block, nxt_block.pos);
        }
        pre_block.nxt_index = tmp_block.nxt_index;
        org_size = pre_block.size;
        pre_block.size += tmp_block.size;
        for (int i = org_size; i < pre_block.size; ++i) {
            pre_block.array[i] = tmp_block.array[i - org_size];
        }
        Blocks.update(pre_block, pre_block.pos);
        Blocks.Delete(tmp_block.pos);
        if (pre_block.size >= 500)
            Split(pre_block.pos);
    }
};

class book {
public:
    Book();

    ~Book();

    string book_name;
    string ISBN;
    int book_price;
    string book_writer;
    char *book_keyword[];
    int quantity;
    double book_price;
    double total_cost;
};

class book_manager {
public:
    void selectOption(vect)
};

void selectOption(vector)

#endif //BOOKSTORE_BOOK_H


