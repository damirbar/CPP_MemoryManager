//
// Created by damir on 14-Jan-18.
//

#ifndef CPP_EX3_V3_OPERATORS_H
#define CPP_EX3_V3_OPERATORS_H


#include "MemoryManagerValgrind.h"

static MemoryManagerValgrind manager;

/*  Overloaded new operator.
 *  Parameter: size of the block to return.
 *  Returns a block of memory.
 *
 *  This operator checks if the MemoryManager is fully initialized. If so,
 *  the block will be a pointer to the custom heap (memory pool). Else,
 *  the memory will be allocated from the original heap.
 */
void *operator new(size_t size) throw(std::bad_alloc) {
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

/*  Overloaded delete operator.
 *  Parameter: The block to return.
 *
 *  This operator checks if the MemoryManager is fully initialized. If so,
 *  the manager will look for its size and then will return the memory to
 *  the custom heap (memory pool). Else, the memory will be deallocated
 *  to the original heap.
 */
void operator delete(void *p) throw() {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's delete operator was called");
        manager.searchSizeAndReturnMemory((char *) p);
        return;
    }
    free(p);
}


/*  Overloaded new[] operator.
 *  Parameter: size of the block to return.
 *  Returns a block of memory.
 *
 *  This operator checks if the MemoryManager is fully initialized. If so,
 *  the block will be a pointer to the custom heap (memory pool). Else,
 *  the memory will be allocated from the original heap.
 */
void *operator new[](size_t size) throw(std::bad_alloc) {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's new[] operator was called");
        return manager.getMemoryBlock(size);
    }
    return malloc(size);
}


/*  Overloaded delete[] operator.
 *  Parameter: The block to return.
 *
 *  This operator checks if the MemoryManager is fully initialized. If so,
 *  the manager will look for its size and then will return the memory to
 *  the custom heap (memory pool). Else, the memory will be deallocated
 *  to the original heap.
 */
void operator delete[](void *p) throw() {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's delete[] operator was called");
        manager.searchSizeAndReturnMemory((char *) p);
        return;
    }
    free(p);
}


#endif //CPP_EX3_V3_OPERATORS_H
