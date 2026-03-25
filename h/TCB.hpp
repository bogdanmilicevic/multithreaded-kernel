//
// Created by os on 5/2/24.
//

#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP
#include "../lib/hw.h"
#include "Scheduler.hpp"
#include "MemoryAllocator.hpp"
#include "MojaLista.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() {
        delete[] stack;
    }

    static void* operator new(size_t size) {
        void* ptr = MemoryAllocator::dohvatiMemoriju().memAlloc(((size+sizeof(MemoryAllocator::Cvor)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE));
        return ptr;
    }
    void operator delete[](void *ptr)
    {
        MemoryAllocator::dohvatiMemoriju().memFree(ptr);
    }
    static void operator delete(void* ptr) {
        MemoryAllocator::dohvatiMemoriju().memFree(ptr);
    }
    void postaviTrebaDaCeka(time_t vreme){this->trebadaceka=vreme;}

    time_t dohvatiTrenutnoCeka()const{return trenutnoceka;}

    time_t dohvatiTrebaDaCeka()const{return trebadaceka;}

    void postaviTrenutnoCeka(){ trenutnoceka=0;}

    void uvecajrenutnoCeka(){trenutnoceka++;}

    int dohvatiStanje()const{return stanje;}

    void promeniStanje(int k){stanje=k;}

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body,void *arg,void* stek);

    static TCB *createThread1(Body body,void *arg,void* stek);

    static void start(TCB* tcb){if(tcb->body)Scheduler::put(tcb);}

    static void yield();

    static TCB *running;

    static void uvecajVremeSvima();

    static int timeSleep(time_t);


private:
    TCB(Body body,void* arg, uint64 timeSlice,void* stek) :
            body(body),
            stack((uint8*)stek),
            context({ (uint64)&threadWrapper,
                     stack ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0}),
            timeSlice(timeSlice),
            finished(false),argument(arg),blokiran(false)
    {
        /*if (body != nullptr) { Scheduler::put(this); }*/
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };
    friend class Sem;
    Body body;
    uint8 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    void* argument;
    bool blokiran;
    friend class Riscv;
    static MojaLista listaBlokiranih;
    int stanje=0;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();



    static uint64 timeSliceCounter;
    time_t trenutnoceka=0;
    time_t trebadaceka=0;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;
};
#endif //PROJECT_BASE_V1_1_TCB_HPP
