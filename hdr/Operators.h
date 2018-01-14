//
// Created by damir on 14-Jan-18.
//

#ifndef CPP_EX3_V3_OPERATORS_H
#define CPP_EX3_V3_OPERATORS_H


#include "MemoryManagerValgrind.h"

static MemoryManagerValgrind manager;


void *operator new(size_t size) {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's new operator was called");
        void *ret = manager.getMemoryBlock(size);
        if (!ret) {
            puts("Not enough memory on the manager's heap!");
        } else {
            return ret;
        }
    }

    return malloc(size);
}

void operator delete(void *p) {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's delete operator was called");
        manager.searchSizeAndReturnMemory((char *) p);
        return;
    }
    free(p);
}

void *operator new[](size_t size) {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's new[] operator was called");
        return manager.getMemoryBlock(size);
    }
    return malloc(size);
}

void operator delete[](void *p) {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's delete[] operator was called");
        manager.searchSizeAndReturnMemory((char *) p);
        return;
    }
    free(p);
}


#endif //CPP_EX3_V3_OPERATORS_H
