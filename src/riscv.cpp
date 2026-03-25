//
// Created by os on 5/2/24.
//

#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
//#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Sem.hpp"
#include "../h/KeyboardBuffer.hpp"
#include "../h/PanelBuffer.hpp"
//#include "../test/printing.hpp"
int Riscv::rezim=0;

void Riscv::popSppSpie()
{
    if(rezim==0){
        ms_sstatus(Riscv::SSTATUS_SPP);
        rezim=1;
    }else{
        mc_sstatus(Riscv::SSTATUS_SPP);
    }

    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {

    uint64 scause = r_scause();
    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        uint64 broj;
        __asm__ volatile("ld %0, 80(s0)" :  "=r" (broj));
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)

        if (broj == 0x01) {
            uint64 brojBlokova;
            __asm__ volatile("ld %0, 88(s0)" : "=r" (brojBlokova));
            void *adresa = MemoryAllocator::dohvatiMemoriju().memAlloc(brojBlokova);
            __asm__ volatile("sd %0, 80(s0)": : "r" (adresa));
        } else if (broj == 0x02) {
            uint64 adresa;
            __asm__ volatile("ld %0, 88(s0)" : "=r" (adresa));
            uint64 uspeh;
            uspeh = MemoryAllocator::dohvatiMemoriju().memFree((void *) (adresa));
            __asm__ volatile("sd %0, 80(s0)": : "r" (uspeh));
        } else if (broj == 0x11) {
            TCB **handle;
            void (*funkcija)(void *);
            void *arg;
            void *stek;
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (handle));
            __asm__ volatile("ld %0, 96(s0)" :  "=r" (funkcija));
            __asm__ volatile("ld %0, 104(s0)" :  "=r" (arg));
            __asm__ volatile("ld %0, 112(s0)" :  "=r" (stek));

            *handle = TCB::createThread(funkcija, arg, stek);
            if (!handle) {
                __asm__ volatile("sd %0, 80(s0)": : "r" (-1));
            } else {
                __asm__ volatile("sd %0, 80(s0)": : "r" (0));
            }
        } else if (broj == 0x12) {
            //if(TCB::running==nullptr || TCB::running->isFinished()==true) __asm__ volatile("mv a0, %0" : : "r" (-1));
            TCB::running->setFinished(true);
            TCB *old = TCB::running;
            TCB::running = Scheduler::get();
            TCB::contextSwitch(&old->context, &TCB::running->context);
            __asm__ volatile("mv a0, %0" : : "r" (0));
            if (TCB::running == nullptr) __asm__ volatile("mv a0, %0" : : "r" (-1));
            __asm__ volatile("sd a0, 80(s0)");
        } else if (broj == 0x13) {
            TCB::dispatch();
        } else if (broj == 0x21) {
            Sem **semaphore;
            uint64 vrednost;
            __asm__ volatile("ld %0, 96(s0)" :  "=r" (semaphore));
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (vrednost));
            *semaphore = Sem::semOpen(vrednost);
            if (*semaphore == nullptr) {
                __asm__ volatile("sd %0, 80(s0)": : "r" (-1));
            } else {
                __asm__ volatile("sd %0, 80(s0)": : "r" (0));
            }
        } else if (broj == 0x22) {
            Sem* semaphore;
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (semaphore));
            delete semaphore;
            __asm__ volatile("sd %0, 80(s0)": : "r" (0));
        } else if (broj == 0x23) {
            Sem *semaphore;
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (semaphore));
            if (!semaphore->wait()) {
                __asm__ volatile("sd %0, 80(s0)": : "r" (0));
            } else {
                __asm__ volatile("sd %0, 80(s0)": : "r" (-1));
            }
        } else if (broj == 0x24) {
            Sem *semaphore;
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (semaphore));
            if (!semaphore->signal()) {
                __asm__ volatile("sd %0, 80(s0)": : "r" (0));
            } else {
                __asm__ volatile("sd %0, 80(s0)": : "r" (-1));
            }
        } else if (broj == 0x25) {
            Sem *semaphore;
            time_t vreme;
            __asm__ volatile("ld %0, 96(s0)" :  "=r" (semaphore));
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (vreme));
            int uspeh = semaphore->semTimed(vreme);
            __asm__ volatile("sd %0, 80(s0)": : "r" (uspeh));
        } else if (broj == 0x26) {
            Sem *semaphore;
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (semaphore));
            int uspeh = semaphore->semTry();
            __asm__ volatile("sd %0, 80(s0)": : "r" (uspeh));
        } else if (broj == 0x31) {
            time_t vreme;
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (vreme));
            int uspeh = TCB::timeSleep(vreme);
            __asm__ volatile("sd %0, 80(s0)": : "r" (uspeh));
        } else if (broj == 0x03) {
            TCB::dispatch();
        } else if(broj==0x41){
              char c=KeyboardBuffer::instance()->getc();
              __asm__ volatile("sd %0, 80(s0)": : "r" (c));
        } else if(broj==0x42){
              char c;
              __asm__ volatile("ld %0, 88(s0)" :  "=r" (c));
              PanelBuffer::instance()->putc(c);
        } else if(broj==0x54){
            TCB **handle;
            void (*funkcija)(void *);
            void *arg;
            void *stek;
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (handle));
            __asm__ volatile("ld %0, 96(s0)" :  "=r" (funkcija));
            __asm__ volatile("ld %0, 104(s0)" :  "=r" (arg));
            __asm__ volatile("ld %0, 112(s0)" :  "=r" (stek));

            *handle = TCB::createThread1(funkcija, arg, stek);
            if (!handle) {
                __asm__ volatile("sd %0, 80(s0)": : "r" (-1));
            } else {
                __asm__ volatile("sd %0, 80(s0)": : "r" (0));
            }
        }else if(broj==0x55){
            TCB* handle;
            __asm__ volatile("ld %0, 88(s0)" :  "=r" (handle));
            TCB::start(handle);
        }

            w_sstatus(sstatus);
            w_sepc(sepc);

        } else if (scause == 0x8000000000000001UL) {
            // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
            mc_sip(SIP_SSIP);
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::uvecajVremeSvima();
            TCB::timeSliceCounter++;
            if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
            {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
            }
            w_sstatus(sstatus);
            w_sepc(sepc);

        } else if (scause == 0x8000000000000009UL) {
            // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
            //console_handler();
            int x = plic_claim();
            while (*((char*)(CONSOLE_STATUS)) & CONSOLE_RX_STATUS_BIT) {
                char c = (*(char*)CONSOLE_RX_DATA);
                KeyboardBuffer::instance()->putc(c);
            }
            plic_complete(x);
            w_sstatus(sstatus);
            w_sepc(sepc);
        } else {
            /*printString("sepc: ");
            printInt(sepc);
            printString("\n scause: ");
            printInt(scause);
            printString("\n sstatus: ");
            printInt(sstatus);
            printString("\n");*/
            //thread_exit();
            w_sstatus(sstatus);
            w_sepc(sepc);
            // unexpected trap cause
        }
    }
