//
// Created by os on 5/4/24.
//
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
bool MemoryAllocator::napravljen=false;
MemoryAllocator* MemoryAllocator::instance=nullptr;

void* MemoryAllocator::memAlloc(size_t broj) {

    uint16 indikator = 0;
    //if(broj<0) return nullptr;
    Cvor *trenutni = prvi;
    for (; trenutni != nullptr; trenutni = trenutni->sledeci) {
        if (trenutni->velicina >= broj * MEM_BLOCK_SIZE) {
            indikator = 1;
            break;
        }
    }
    if (indikator == 0) return nullptr;
    uint64 ostatak = 0;
    if (trenutni) ostatak = trenutni->velicina - broj * MEM_BLOCK_SIZE;
    if (ostatak < (uint64) MEM_BLOCK_SIZE) {
        if (trenutni->prethodni) {
            trenutni->prethodni->sledeci = trenutni->sledeci;
        } else {
            prvi = trenutni->sledeci;
            prvi->prethodni = nullptr;
        }
        if (trenutni->sledeci) trenutni->sledeci->prethodni = trenutni->prethodni;
    } else {
        Cvor *novi = (Cvor *) ((char *) trenutni + broj * MEM_BLOCK_SIZE);
        if (trenutni->prethodni) {
            trenutni->prethodni->sledeci = novi;
        } else {
            prvi = novi;
        }
        if (trenutni->sledeci) trenutni->sledeci->prethodni = novi;

        novi->prethodni = trenutni->prethodni;
        novi->sledeci = trenutni->sledeci;
        novi->velicina = trenutni->velicina - broj * MEM_BLOCK_SIZE;
    }
    trenutni->velicina = broj * MEM_BLOCK_SIZE;
    trenutni->sledeci = nullptr;
    trenutni->prethodni = nullptr;

    return (char *) trenutni + sizeof(Cvor);

}

int MemoryAllocator::memFree(void *adrr) {
    if(adrr == nullptr || (uint64)adrr > (uint64)HEAP_END_ADDR || (uint64)adrr < (uint64)HEAP_START_ADDR)
        return  -1;
    Cvor* trenutni=nullptr;

    if(!prvi || (char*)adrr<(char*)prvi){
        trenutni=nullptr;
    }
    else{
        for(trenutni=prvi;trenutni->sledeci && adrr>trenutni->sledeci;trenutni=trenutni->sledeci);
    }
    Cvor* novi=(Cvor*)((char*)adrr-sizeof(Cvor));
    novi->prethodni=trenutni;
    if(trenutni)novi->sledeci=trenutni->sledeci;
    else novi->sledeci=prvi;
    if(novi->sledeci)novi->sledeci->prethodni=novi;
    if(trenutni) trenutni->sledeci=novi;
    else prvi=novi;

    tryToJoin(novi);
    tryToJoin(trenutni);
    return 0;
}

int MemoryAllocator::tryToJoin(Cvor *trenutni) {
    if(!trenutni) return 0;
    if(trenutni->sledeci && (char*)trenutni+trenutni->velicina==(char*)(trenutni->sledeci)){
        trenutni->velicina+=trenutni->sledeci->velicina;
        trenutni->sledeci=trenutni->sledeci->sledeci;
        if(trenutni->sledeci) trenutni->sledeci->prethodni=trenutni;
        return 1;
    }
    return 0;
}

MemoryAllocator &MemoryAllocator::dohvatiMemoriju() {
    static MemoryAllocator instance;
    return instance;
}
