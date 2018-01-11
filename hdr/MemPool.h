//
// Created by damir on 10-Jan-18.
//

#ifndef CPP_EX3_V3_MEMPOOL_H
#define CPP_EX3_V3_MEMPOOL_H

#include <iostream>


/**
 *  Singleton class
 */
class MemPool {

    friend class FreeList;
    friend class MemoryManager;

    char *_pool;

    MemPool();

    MemPool(const MemPool &other) = delete;

    void operator=(const MemPool &other) = delete;

    char *getPool();

public:

    ~MemPool();

    static MemPool &getInstance();

    void setPool(size_t size);

//    char *getChunk(size_t size);
};

#endif //CPP_EX3_V3_MEMPOOL_H
