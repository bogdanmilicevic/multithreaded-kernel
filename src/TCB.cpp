//
// Created by os on 5/2/24.
//


#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/syscall_c.hpp"
//#include "../test/printing.hpp"
TCB *TCB::running = nullptr;
MojaLista TCB::listaBlokiranih;
uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body,void *arg,void* stek)
{
   TCB*tmp= new TCB(body,arg, DEFAULT_TIME_SLICE,stek);
   if (body != nullptr) { Scheduler::put(tmp); }
   return tmp;
}

void TCB::yield()
{
    uint64 broj=0x03;
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
}


void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->blokiran) { Scheduler::put(old);}
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->argument);
    TCB::running->setFinished(true);
    thread_dispatch();
    //thread_exit();
}

void TCB::uvecajVremeSvima() {
    MojaLista::Elem* tmp=listaBlokiranih.peekFirstElem();
    while(tmp){
        tmp->data->uvecajrenutnoCeka();
        if(tmp->data->dohvatiTrenutnoCeka()>=tmp->data->dohvatiTrebaDaCeka()){
            TCB* trenutni=tmp->data;
            tmp=tmp->next;
            TCB::listaBlokiranih.izbaciElement(trenutni);
            trenutni->promeniStanje(70);
            trenutni->blokiran=false;
            Scheduler::put(trenutni);
        }
       else{
           tmp=tmp->next;
       }
    }
}

int TCB::timeSleep(time_t vreme){
    TCB* old=running;
    if(vreme!=0){
        TCB::running->postaviTrebaDaCeka(vreme);
        TCB::running->postaviTrenutnoCeka();
        TCB::running->blokiran=true;
        TCB::listaBlokiranih.addLast(TCB::running);
        TCB::running=Scheduler::get();
        contextSwitch(&old->context,&TCB::running->context);
    }
    return 0;
}

TCB *TCB::createThread1(TCB::Body body, void *arg, void *stek) {
    return new TCB(body,arg, DEFAULT_TIME_SLICE,stek);
}
