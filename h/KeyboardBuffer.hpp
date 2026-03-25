//
// Created by os on 5/28/24.
//

#ifndef PROJECT_BASE_V1_1_ORIGINAL_KEYBOARDBUFFER_HPP
#define PROJECT_BASE_V1_1_ORIGINAL_KEYBOARDBUFFER_HPP
#include "../lib/hw.h"
#include "../h/Sem.hpp"
class KeyboardBuffer{
public:
    KeyboardBuffer():head(0),tail(0),count(0){
        mutex=Sem::semOpen(1);
        itemAvailable=Sem::semOpen(0);
    }
    void putc(char c);
    char getc();
    static KeyboardBuffer* instance(){
        static KeyboardBuffer _instance;
        return &_instance;
    }
private:
    static const size_t KB_SIZE=256;
    char buffer[KB_SIZE];
    size_t head,tail,count;
    Sem* mutex;
    Sem* itemAvailable;
};


#endif //PROJECT_BASE_V1_1_ORIGINAL_KEYBOARDBUFFER_HPP