//
// Created by os on 5/4/24.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#include "../lib/hw.h"

class MemoryAllocator {
public:
    struct Cvor{
        size_t velicina;
        Cvor* sledeci=nullptr;
        Cvor* prethodni=nullptr;
    };
    static MemoryAllocator& dohvatiMemoriju();

    void* memAlloc(size_t broj);

    int memFree(void * adrr);

private:
    int tryToJoin(Cvor* cvor);
    MemoryAllocator() {
        prvi=(Cvor*) HEAP_START_ADDR;
        prvi->velicina=(uint64)HEAP_END_ADDR-(uint64)HEAP_START_ADDR-sizeof(Cvor)+1;
        prvi->sledeci=nullptr;
        prvi->prethodni=nullptr;
    }
    Cvor* prvi;
    static bool napravljen;
    static MemoryAllocator* instance;
};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
