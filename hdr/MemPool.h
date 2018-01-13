//
// Created by damir on 10-Jan-18.
//

#ifndef CPP_EX3_V3_MEMPOOL_H
#define CPP_EX3_V3_MEMPOOL_H

#include <iostream>


/** Singleton class.
 *  This class acts as the custom memory manager's heap.
 *  This class requests a block of the given size from the original
 *  heap and keeps it as its field.
 */
class MemPool {

    friend class FreeList;
    friend class MemoryManager;

    char *_pool;

    //  Default ctor. Private to keep it a singleton.
    MemPool();

    //  Deleted copy ctor to avoid the copying of the singleton.
    MemPool(const MemPool &other) = delete;

    //  Deleted assignment operator to avoid the copying of the singleton.
    void operator=(const MemPool &other) = delete;

    //  Returns the memory pool's memory block.
    char *getPool();

public:

    //  Dtor. Frees the memory block of the memory pool.
    ~MemPool();

    //  Returns the singleton's static instance.
    static MemPool &getInstance();

    //  Sets the memory block's size if it wasn't already set.
    void setPool(size_t size);
};

#endif //CPP_EX3_V3_MEMPOOL_H
