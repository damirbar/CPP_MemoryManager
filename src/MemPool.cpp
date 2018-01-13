//
// Created by damir on 10-Jan-18.
//

#include <cmath>
#include "../hdr/MemPool.h"

MemPool::MemPool() : _pool(nullptr) {}

void MemPool::setPool(size_t size) {
    if (!_pool) {
        size_t next = (size_t) pow(2, ceil(log(size) / log(2)));
        _pool = (char *) malloc(sizeof(char) * next);
    } else {
        std::cerr << "Can't set the pool twice!" << std::endl;
    }
}

MemPool::~MemPool() {
    if (_pool) {
        free(_pool);
        _pool = nullptr;
    }
}

MemPool &MemPool::getInstance() {
    static MemPool instance;
    return instance;
}

char *MemPool::getPool() {
    return _pool;
}