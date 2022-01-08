#include <iostream>
#include<fstream>
#include "bookstore.h"

using namespace std;

int main(int argc, const char *argv[]) {
     Bookstore work;
    string Command_line;
    while (getline(cin, Command_line)) {
        if (Command_line == "exit" || Command_line == "quit")break;
        work.operation(Command_line);
    }
    return 0;
}
