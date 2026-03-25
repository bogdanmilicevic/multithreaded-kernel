//
// Created by os on 5/12/24.
//
#include "../h/Sem.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../h/MojaLista.hpp"
void Sem::block() {
    TCB* old=TCB::running;
    old->blokiran=true;
    red.addLast(old);
    TCB::running=Scheduler::get();
    TCB::contextSwitch(&old->context,&TCB::running->context);
    /*red.addLast(TCB::running);
    TCB::running->blokiran=true;
    TCB::dispatch();*/
}

void Sem::unblock() {
   if(red.dohvatibrojelemenata()>0) {
        TCB* nit=red.removeFirst();
        while(nit && nit->dohvatiStanje()==70){
            nit->promeniStanje(-1);
            nit=red.removeFirst();
        }
        if(nit){
            if(nit->dohvatiStanje()!=-1){
                nit->promeniStanje(0);
                TCB::listaBlokiranih.izbaciElement(nit);
            }
            nit->blokiran=false;
            Scheduler::put(nit);
        }
    }
   /*TCB* nit=red.removeFirst();
    if(nit)nit->blokiran=false;
    if(!nit->isFinished())Scheduler::put(nit);*/
}

int Sem::wait() {
    for(MojaLista::Elem* tmp=red.peekFirstElem();tmp!=nullptr;tmp=tmp->next){
        if(tmp->data->dohvatiStanje()==70)
            vrednost++;
    }
    if(--vrednost<0) {
        block();
    }
    else{
        TCB::running->promeniStanje(0);
    }
    return 0;
}

int Sem::signal() {
    if(++vrednost<=0) unblock();
    return 0;
}

int Sem::semTry() {
    if(vrednost<=0) return -1;

    if(--vrednost==0) return 0;

    return 1;
}

int Sem::semTimed(size_t vreme) {
    for(MojaLista::Elem* tmp=red.peekFirstElem();tmp!=nullptr;tmp=tmp->next){
        if(tmp->data->dohvatiStanje()==70)
            vrednost++;
    }
    TCB::running->postaviTrebaDaCeka(vreme);
    TCB::running->postaviTrenutnoCeka();
    if(--vrednost<0  && vreme!=0){
        TCB::listaBlokiranih.addLast(TCB::running);
        block();
    }

    //sem_wait(this);
    int k=TCB::running->dohvatiStanje();
    return k;
}

Sem::~Sem() {
    while(red.dohvatibrojelemenata()>0){
        TCB * tcb=red.removeFirst();
        tcb->promeniStanje(-2);
        Scheduler::put(tcb);
    }
    this->otvoren=false;
}






