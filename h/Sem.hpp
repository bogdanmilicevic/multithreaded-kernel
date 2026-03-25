//
// Created by os on 5/12/24.
//

#ifndef PROJECT_BASE_V1_1_SEM_HPP
#define PROJECT_BASE_V1_1_SEM_HPP

#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
class Sem{
public:
    void* operator new (size_t n){
        return MemoryAllocator::dohvatiMemoriju().memAlloc(n/MEM_BLOCK_SIZE+1);
    }
    void operator delete (void* p){
        MemoryAllocator::dohvatiMemoriju().memFree(p);
    }
    void operator delete[](void *p) noexcept
    {
        MemoryAllocator::dohvatiMemoriju().memFree(p);
    }
    static Sem* semOpen(uint64 val){
        //uint64 velicina= (sizeof(Sem) + sizeof(MemoryAllocator::Cvor) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
        Sem* semafor= new Sem();
        semafor->vrednost=(int)val;
        return semafor;
    }

    int wait();

    int signal();

    /*int semClose();*/

    int semTry();

    int semTimed(size_t);

    int dohvativrednost()const{
        return vrednost;
    }
    int vrednost;

    ~Sem();



protected:
    void block();
    void unblock();
private:
    bool otvoren=true;
    MojaLista red;
};
#endif //PROJECT_BASE_V1_1_SEM_HPP
