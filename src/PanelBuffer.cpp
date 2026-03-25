//
// Created by os on 5/29/24.
//
#include "../h/PanelBuffer.hpp"
void PanelBuffer::putc(char c) {
    if(count<KB_SIZE){
        buffer[tail]=c;
        tail=(tail+1)%KB_SIZE;
        count++;
        itemAvailable->signal();
    }
}

char PanelBuffer::getc() {
    itemAvailable->wait();
    mutex->wait();
    char c=buffer[head];
    head=(head+1)%KB_SIZE;
    count--;
    mutex->signal();
    return c;
}
