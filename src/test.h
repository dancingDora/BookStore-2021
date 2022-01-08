//
// Created by 陆逸凡 on 2021/12/21.
//

#ifndef BOOKSTORE_TEST_H
#define BOOKSTORE_TEST_H
int main(){
        while (true) {
            try {
                string input = getLine();
                if (input.empty())
                    continue;
                processLine(input, program, state);
            } catch (ErrorException &ex) {
                cout  << ex.getMessage() << endl;
            }
        }
        return 0;
}
#endif //BOOKSTORE_TEST_H
