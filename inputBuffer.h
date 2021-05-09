#include <bits/stdc++.h>

using namespace std;

class InputBuffer{
    private:
        char* buffer;
        size_t buffer_lenght;
        ssize_t input_lenght;
    public:
        char* getBuffer();
        InputBuffer();
        void readInput();
        ~InputBuffer();
        void setBuffer(char* a);
};