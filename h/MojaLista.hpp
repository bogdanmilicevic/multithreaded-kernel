//
// Created by os on 5/14/24.
//
#ifndef PROJECT_BASE_V1_1_MOJALISTA_HPP
#define PROJECT_BASE_V1_1_MOJALISTA_HPP

#include "../h/MemoryAllocator.hpp"
//#include "../test/printing.hpp"
class TCB;

class MojaLista{
private:
    friend class TCB;
    friend class Sem;
    struct Elem
    {
        TCB *data;
        Elem *next;
        static void* operator new(size_t size) {
            void* ptr = MemoryAllocator::dohvatiMemoriju().memAlloc(size/MEM_BLOCK_SIZE+1);
            return ptr;
        }
        static void operator delete(void* ptr) {
            MemoryAllocator::dohvatiMemoriju().memFree(ptr);
        }
        Elem(TCB *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;
    int brojelemenata=0;
public:
    MojaLista() : head(nullptr), tail(nullptr) {}

    MojaLista(const MojaLista &) = delete;

    MojaLista &operator=(const MojaLista &) = delete;

    void addFirst(TCB *data)
    {
        Elem *elem = new Elem(data, head);
        elem->next=head;
        head = elem;
        if (!tail) { tail = head; }
        brojelemenata++;

    }

    void addLast(TCB *data)
    {
        Elem *elem = new Elem(data, nullptr);

        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
        brojelemenata++;
        /*Elem *tmp=head;
        while(tmp){
            printString("\nELementi:\n");
            printInt((uint64)tmp->data);
            printString("\n\n");
            tmp=tmp->next;
        }*/
    }

    TCB *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        TCB *ret = elem->data;
        brojelemenata--;
        delete elem;
        return ret;
    }

    TCB *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    Elem *peekFirstElem()
    {
        if (!head) { return nullptr; }
        return head;
    }
    TCB *removeLast();

    TCB *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }
    int dohvatibrojelemenata(){
        return brojelemenata;
    }
    bool izbaciElement(TCB* element);

    bool proveri(TCB*);

};
#endif //PROJECT_BASE_V1_1_MOJALISTA_HPP
