#include "inputBuffer.h"
#include <bits/stdc++.h>

using namespace std;

InputBuffer::InputBuffer(){
    buffer = NULL;
    buffer_lenght = 0;
    input_lenght = 0;
}

void InputBuffer::readInput(){
    ssize_t bytesRead = getline((&this->buffer),&(this->buffer_lenght),stdin);

    if (bytesRead<=0){
        cout<<"Error reading input\n";
        exit(EXIT_FAILURE);
    }

    this->input_lenght = bytesRead-1;
    this->buffer[bytesRead-1] = 0;
}


InputBuffer::~InputBuffer(){
    free(this->buffer);
    free(this);
}

char* InputBuffer::getBuffer(){
    return this->buffer;
}

void InputBuffer::setBuffer(char* a){
	this->buffer = a;
}
