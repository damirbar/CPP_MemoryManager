//
// Created by damir on 10-Jan-18.
//

#include <cmath>
#include "../hdr/MemPool.h"

MemPool::MemPool() : _pool(nullptr) {
    std::cout << "MemPool ctor called" << std::endl;
}

void MemPool::setPool(size_t size) {
    if (!_pool) {
        size_t next = (size_t )pow(2, ceil(log(size)/log(2)));
//        _pool = new char[next];
        _pool = (char*)malloc(sizeof(char) * next);
        std::cout << "A pool of " << next << " bytes was set at address ";
        printf("%p", _pool);
        std::cout << std::endl;
    }
    else {
        std::cerr << "Can't set the pool twice!" << std::endl;
    }
}

MemPool::~MemPool() {
    std::cout << "MemPool dtor called" << std::endl;
    if (_pool) {
        delete[] _pool;
//        free(_pool);
        _pool = nullptr;
        std::cout << "Pool deleted" << std::endl;
    }
}

MemPool & MemPool::getInstance() {
    static MemPool instance;
    return instance;
}

char *MemPool::getPool() {
    return _pool;
}