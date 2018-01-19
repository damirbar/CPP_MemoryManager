//
// Created by damir on 11-Jan-18.
//

#include "../hdr/MemoryManager.h"

#include <cmath>

MemoryManager::MemoryManager(size_t size, bool valgrind)
        : _mmpl(MemPool::getInstance()),
          _poolSize((size_t) pow(2, ceil(log(size) / log(2)))),
          _currAlloc(0), valgrindFlag(valgrind),
          _readyToGo(true),
          _elaboratedOutput(false) {

    void *mapMem = (FreeList *) malloc(sizeof(FreeList) * MAX_POW_OF_TWO);
    if (!mapMem) {
        std::cerr << "FreeList map was not created" << std::endl;
        return;
    }
    void *allocatedMem = (FreeList *) malloc(sizeof(FreeList) * MAX_POW_OF_TWO);
    if (!allocatedMem) {
        std::cerr << "FreeList allocated was not created" << std::endl;
    }

    map = new(mapMem) FreeList[MAX_POW_OF_TWO];
    allocated = new(allocatedMem) FreeList[MAX_POW_OF_TWO];

    _mmpl.setPool(size);
    init();
}

MemoryManager::MemoryManager()
        : _mmpl(MemPool::getInstance()), _readyToGo(false) {}


void MemoryManager::setSize(size_t size) {
    if (_readyToGo) {
        std::cout << "Can't set the manager's size more than once!" << std::endl;
    }

    _poolSize = (size_t) pow(2, ceil(log(size) / log(2)));
    _currAlloc = 0;
    valgrindFlag = false;

    void *mapMem = (FreeList *) malloc(sizeof(FreeList) * MAX_POW_OF_TWO);
    if (!mapMem) {
        std::cerr << "FreeList map was not created" << std::endl;
        return;
    }
    void *allocatedMem = (FreeList *) malloc(sizeof(FreeList) * MAX_POW_OF_TWO);
    if (!allocatedMem) {
        std::cerr << "FreeList allocated was not created" << std::endl;
    }

    map = new(mapMem) FreeList[MAX_POW_OF_TWO];
    allocated = new(allocatedMem) FreeList[MAX_POW_OF_TWO];

    _mmpl.setPool(size);
    init();
    _readyToGo = true;
}


int MemoryManager::whichPowerOfTwo(size_t n) {
    int counter = 0;

    while (n % 2 == 0) {
        counter++;
        if (counter > 10)
            break;
        n /= 2;
    }
    if (n != 1 && n % 2 != 0) {
        return -1;
    }
    return counter;
}


MemoryManager::~MemoryManager() {

    if (!map) {
        return;
    }
    for (int i = 0; i < MAX_POW_OF_TWO; ++i) {
        map[i].~FreeList();
    }
    free(map);

    if (!allocated) {
        return;
    }
    for (int i = 0; i < MAX_POW_OF_TWO; ++i) {
        allocated[i].~FreeList();
    }
    free(allocated);
}

void MemoryManager::printCurrMemoryState() {

    for (int i = 0; i < MAX_POW_OF_TWO; ++i) {

        FreeNode *it = map[i].getHead();

        if (it) {
            if (_elaboratedOutput) {
                std::cout << "This list has " << map[i].pairs() << " consecutive pairs" << std::endl;
            }
            std::cout << "Showing state for blocks of size " << pow(2, i) << ":" << std::endl;
            while (it != map[i].tail) {
                std::cout << *it << " <-- ";
                it = it->next;
            }
            if (it == map[i].tail) {
                std::cout << *it;
            }
            std::cout << "\n" << std::endl;
        }
    }
}

void MemoryManager::init() {

    char *pool = _mmpl.getPool();
    size_t size = _poolSize;

    for (int i = 0; i < MAX_POW_OF_TWO; ++i) {
        size_t counter = (size_t) pow(2, i);

        counters[i] = 0;
        if (counter <= size) {
            for (long j = 0, index = 0; j < size; j += counter, ++index) {

                char *filler = &pool[index * counter];
                void *memToAlloc = (FreeNode *) malloc(sizeof(FreeNode));
                if (!memToAlloc) {
                    std::cerr << "Couldn't allocate memory for block of size " << size << std::endl;
                    return;
                }
                map[i].add(new(memToAlloc) FreeNode(filler, counter));
                ++counters[i];
            }
        }
    }
}

void MemoryManager::printCurrAllocatedMemoryState() {
    for (int i = 0; i < MAX_POW_OF_TWO; ++i) {

        FreeNode *it = allocated[i].getHead();

        if (it) {
            std::cout << "Showing state for blocks of size " << pow(2, i) << ":" << std::endl;
            while (it != allocated[i].tail) {
                std::cout << *it << "  |  ";
                it = it->next;
            }
            if (it == allocated[i].tail) {
                std::cout << *it;
            }
            std::cout << "\n" << std::endl;
        }
    }
}

char *MemoryManager::getMemoryBlock(size_t size) {

    if (_elaboratedOutput) {
        std::cout << "Got a memory request of size " << size << ", will return memory of size "
                  << (int) pow(2, ceil(log(size) / log(2))) << std::endl;
    }

    FreeNode *ret = nullptr;

    size = (size_t) pow(2, ceil(log(size) / log(2)));
    bool wasAllocated = false;

    int check = whichPowerOfTwo(size);
    if (check != -1) {

        if (map[check].size() == 0) {
            std::cerr << "Not enough memory!" << std::endl;
            return nullptr;
        }

        int nextPow = (int) pow(2, ceil(log(size) / log(2)));
        for (int i = 0; i < MAX_POW_OF_TWO; ++i) {

            while (counters[i] * pow(2, i) > _poolSize - (!wasAllocated ? _currAlloc + size : _currAlloc)) {
                if (_elaboratedOutput) {
                    std::cout << "Popping a block of size " << pow(2, i) << std::endl;
                }
                if (i == check) {
                    ret = map[i].allocNode();
                    allocated[i].addNode(ret);

                    if (!wasAllocated) {
                        _currAlloc += size;
                        wasAllocated = true;
                    }
                } else {
                    FreeNode *toDel = map[i].allocNode();
                    toDel->~FreeNode();
                    free(toDel);
                }
                --counters[i];
            }
        }
        if (!ret) {
            std::cout << "Returning a nullptr. Not enough memory" << std::endl;
            return nullptr;
        } else {
            if (_elaboratedOutput) {
                std::cout << "Returning to the user a block of size " << (int) pow(2, check) << std::endl;
            }
            return ret->getBlock();
        }
    }

    return nullptr;

}


void MemoryManager::returnMemoryBlock(char *f, size_t size) {


    if (!f) {
        std::cout << "Tried to delete a block, but it's actually nullptr" << std::endl;
        return;
    }

    if (_elaboratedOutput) {
        std::cout << "Attempting to return a memory of size " << size << std::endl;
    }
    int check = whichPowerOfTwo(size);

    if (check == -1) {
        std::cerr << "Error returning memory" << std::endl;
    } else {
        char *block = f;
        _currAlloc -= size;
        bool returned = false;

        for (int i = 0; i < MAX_POW_OF_TWO; ++i) {
            size_t counter = (size_t) pow(2, i);

            if (counter <= _poolSize) {

                long j = 0, index = 0;

                while ((counters[i] * pow(2, i)) + pow(2, i) <= _poolSize - _currAlloc) {
                    char *filler;
                    if (_elaboratedOutput) {
                        std::cout << "Adding to the heap a block of size " << pow(2, i) << std::endl;
                    }
                    if (counter > size) {

                        if (map[i - 1].pairs() > map[i].size()) {
                            FreeNode *n = map[i - 1].getHead();
                            map[i].clear();
                            while (n && n->next) {
                                if (n->next->getBlock() - n->getBlock() == pow(2, i - 1)) {
                                    void *memToAlloc = (FreeNode *) malloc(sizeof(FreeNode));
                                    if (!memToAlloc) {
                                        std::cerr << "Couldn't allocate memory for block of size " << size << std::endl;
                                        return;
                                    }
                                    map[i].add(new(memToAlloc) FreeNode(n->getBlock(), counter));
                                    n = n->next;
                                }
                                n = n->next;
                            }
                            counters[i] = (int)map[i].size();
                            break;
                        } else {
                            filler = nullptr;
                        }
                    } else {
                        filler = &block[index * counter + (counter - (int) (pow(2, i)))];

                    }

                    if (!returned) {

                        if (pow(2, i) == size) {
                            returned = true;
                            FreeList &list = allocated[whichPowerOfTwo(size)];
                            map[whichPowerOfTwo(size)].addBlock(list.removeNodeOfBlock(f), size);
                        } else {
                            void *memToAlloc = (FreeNode *) malloc(sizeof(FreeNode));
                            if (!memToAlloc) {
                                std::cerr << "Couldn't allocate memory for block of size " << size << std::endl;
                                return;
                            }
                            map[i].add(new(memToAlloc) FreeNode(filler, counter));
                        }
                    }
                    ++counters[i];
                    j += counter, ++index;
                }

            }
        }
    }


}

void MemoryManager::searchSizeAndReturnMemory(char *block) {
    for (int i = 0; i < MAX_POW_OF_TWO; ++i) {
        int size = allocated[i].searchBlockSize(block);
        if (size != -1) {
            if (_elaboratedOutput) {
                std::cout << "\t\t\t\t\t\t\t\t\t\t\t\tThe node removed is of size " << size << std::endl;
            }
            returnMemoryBlock(block, (size_t) size);
        }
    }
}

bool MemoryManager::isReady() {
    return _readyToGo;
}

void MemoryManager::setElaboratedOutput(bool is) {
    _elaboratedOutput = is;
}







