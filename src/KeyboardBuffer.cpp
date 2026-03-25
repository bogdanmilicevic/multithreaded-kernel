//
// Created by os on 5/28/24.
//
#include "../h/KeyboardBuffer.hpp"
void KeyboardBuffer::putc(char c) {
    if(count<KB_SIZE){
        buffer[tail]=c;
        tail=(tail+1)%KB_SIZE;
        count++;
        itemAvailable->signal();
    }
}

char KeyboardBuffer::getc() {
    itemAvailable->wait();
    mutex->wait();
    char c=buffer[head];
    head=(head+1)%KB_SIZE;
    count--;
    mutex->signal();
    return c;
}
