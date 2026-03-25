//
// Created by os on 5/2/24.
//
#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Sem.hpp"
#include"../h/MemoryAllocator.hpp"
#include"../h/syscall_cpp.hpp"
#include "../h/PanelBuffer.hpp"
//#include "../test/printing.hpp"
void ispisNaKonzolu(void*){
    while(1){
        while((*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT)){
            *((char*)CONSOLE_TX_DATA)=PanelBuffer::instance()->getc();
        }
    }
}
extern void userMain();
int main()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    thread_t nitZaKonzolu;
    thread_t main1;
    thread_t userMain1;
    thread_create(&main1, nullptr, nullptr);
    thread_create(&nitZaKonzolu, ispisNaKonzolu, nullptr);
    thread_create(&userMain1, reinterpret_cast<void (*)(void*)>(userMain), nullptr);
    TCB::running = main1;
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    while(!userMain1->isFinished()){
        thread_dispatch();
    }
    thread_dispatch();
      
    delete nitZaKonzolu;
    delete userMain1;
    delete main1;
    return 0;
}

