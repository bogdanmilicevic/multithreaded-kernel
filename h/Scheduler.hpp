//
// Created by os on 5/2/24.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP
#include "MojaLista.hpp"
class TCB;

class Scheduler
{
private:
    static MojaLista readyThreadQueue;

public:
    static MojaLista* dohvatiListu(){return &readyThreadQueue;}

    static TCB *get();

    static void put(TCB *tcb);

    static int brojelem;
};
#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
