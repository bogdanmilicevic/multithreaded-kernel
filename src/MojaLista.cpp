//
// Created by os on 5/14/24.
//


#include "../h/MojaLista.hpp"


TCB *MojaLista::removeLast() {
    if (!head) { return 0; }

    Elem *prev = 0;
    for (Elem *curr = head; curr && curr != tail; curr = curr->next)
    {
        prev = curr;
    }

    Elem *elem = tail;
    if (prev) { prev->next = 0; }
    else { head = 0; }
    tail = prev;

    TCB *ret = elem->data;
    brojelemenata--;
    delete elem;
    return ret;
}

bool MojaLista::izbaciElement(TCB *element) {
    if (!head) return false;

    Elem *trenutni = head;
    Elem *prethodni = nullptr;
    if(brojelemenata>0)brojelemenata--;

    if (head->data == element) {
        head = head->next;
        if (!head) {
            tail = nullptr;
            head=nullptr;
        }
        delete trenutni;
        return true;
    }
    while (trenutni && trenutni->data != element) {
        prethodni = trenutni;
        trenutni = trenutni->next;
    }

    if (!trenutni) return false;

    prethodni->next = trenutni->next;
    if (!trenutni->next) {
        tail = prethodni;
    }
    delete trenutni;
    return true;
}

bool MojaLista::proveri(TCB * tmp) {
    Elem *curr = head;
    for (; curr; curr = curr->next)
    {
        if(curr->data==tmp) return true;
    }
    return false;
}

