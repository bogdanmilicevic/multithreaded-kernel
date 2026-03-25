//
// Created by os on 5/6/24.
//
#include "../h/syscall_c.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"
void* mem_alloc (size_t size){
    uint64  broj=0x01;
    uint64  pomoc;
    if((size+sizeof(MemoryAllocator::Cvor))%MEM_BLOCK_SIZE==0)pomoc=(size+sizeof(MemoryAllocator::Cvor))/MEM_BLOCK_SIZE;
    else pomoc=(size+sizeof(MemoryAllocator::Cvor))/MEM_BLOCK_SIZE+1;
    __asm__ volatile("mv a1, %0" : : "r" (pomoc));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    void* adresa;
    __asm__ volatile("mv %0, a0" :  "=r" (adresa));
    if(adresa) return adresa;
    return nullptr;
}

int mem_free (void* addr){
    uint64 broj=0x02;
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)addr));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));

    return uspeh;
}

int thread_create (thread_t* handle,void(*start_routine)(void*),void* arg){
    uint64 broj=0x11;
    void *stack=start_routine != nullptr ? mem_alloc(DEFAULT_STACK_SIZE): nullptr;
    __asm__ volatile("mv a4, %0" : : "r" (stack));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
}

int thread_exit (){
    uint64 broj=0x12;
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
};
void thread_dispatch (){
    uint64 broj=0x13;
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
};
int sem_open (sem_t* handle,unsigned init){
    uint64 broj=0x21;
    __asm__ volatile("mv a2, %0" : : "r" (handle));
    __asm__ volatile("mv a1, %0" : : "r" (init));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
}
int sem_close (sem_t handle){
    uint64 broj=0x22;
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
}
int sem_wait (sem_t id){
    uint64 broj=0x23;
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
};
int sem_signal (sem_t id){
    uint64 broj=0x24;
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
};
int sem_timedwait(sem_t id,time_t timeout){
    uint64 broj=0x25;
    __asm__ volatile("mv a2, %0" : : "r" (id));
    __asm__ volatile("mv a1, %0" : : "r" (timeout));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
}
int sem_trywait(sem_t id){
    uint64 broj=0x26;
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
}
int time_sleep (time_t vreme){
    uint64 broj=0x31;
    __asm__ volatile("mv a1, %0" : : "r" (vreme));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
}
int thread_createCpp (thread_t* handle,void(*start_routine)(void*),void* arg){
    uint64 broj=0x54;
    void *stack=start_routine != nullptr ? mem_alloc(DEFAULT_STACK_SIZE): nullptr;
    __asm__ volatile("mv a4, %0" : : "r" (stack));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
}
char getc (){
    uint64 broj=0x41;
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    char uspeh;
    __asm__ volatile("mv %0, a0" :  "=r" (uspeh));
    return uspeh;
}

void putc (char c){
    uint64 broj=0x42;
    __asm__ volatile("mv a1, %0" : : "r" (c));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
}
void start(TCB* handle){
    uint64 broj=0x55;
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
}