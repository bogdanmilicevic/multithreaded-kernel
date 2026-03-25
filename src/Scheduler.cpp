//
// Created by os on 5/2/24.
//
#include "../h/Scheduler.hpp"
MojaLista Scheduler::readyThreadQueue;
int Scheduler::brojelem=0;

TCB *Scheduler::get()
{
    brojelem--;
    return readyThreadQueue.removeFirst();

}

void Scheduler::put(TCB *ccb)
{
    brojelem++;
    readyThreadQueue.addLast(ccb);
}
