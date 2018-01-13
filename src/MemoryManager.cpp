//
// Created by damir on 11-Jan-18.
//

#include "../hdr/MemoryManager.h"

#include <cmath>

MemoryManager::MemoryManager(size_t size, bool valgrind)
        : /*map(new FreeList[11]), allocated(new FreeList[11]),*/ _mmpl(MemPool::getInstance()),
                                                                  _poolSize((size_t) pow(2, ceil(log(size) / log(2)))),
                                                                  _currAlloc(0), valgrindFlag(valgrind),
                                                                  _readyToGo(true) {

    void *mapMem = (FreeList *) malloc(sizeof(FreeList) * 11);
    if (!mapMem) {
        std::cerr << "FreeList map was not created" << std::endl;
        return;
    }
    void *allocatedMem = (FreeList *) malloc(sizeof(FreeList) * 11);
    if (!allocatedMem) {
        std::cerr << "FreeList allocated was not created" << std::endl;
    }

    map = new(mapMem) FreeList[11];
    allocated = new(allocatedMem) FreeList[11];

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

    void *mapMem = (FreeList *) malloc(sizeof(FreeList) * 11);
    if (!mapMem) {
        std::cerr << "FreeList map was not created" << std::endl;
        return;
    }
    void *allocatedMem = (FreeList *) malloc(sizeof(FreeList) * 11);
    if (!allocatedMem) {
        std::cerr << "FreeList allocated was not created" << std::endl;
    }

    map = new(mapMem) FreeList[11];
    allocated = new(allocatedMem) FreeList[11];

    _mmpl.setPool(size);
    init();
    _readyToGo = true;
}

void MemoryManager::setValgrind(bool is) {
    valgrindFlag = is;
}

//FreeNode *MemoryManager::getMemory(size_t size) {
//
//    std::cout << "Got a memory request of size " << size << ", but will return memory of size "
//              << (int) pow(2, ceil(log(size) / log(2))) << std::endl;
//
//    FreeNode *ret = nullptr;
//
//    size = (size_t) pow(2, ceil(log(size) / log(2)));
//    bool wasAllocated = false;
//
//    int check = whichPowerOfTwo(size);
//    if (check != -1) {
//
//        int nextPow = (int) pow(2, ceil(log(size) / log(2)));
//        for (int i = 0; i < 11; ++i) {
//
////            if (i != check) {
////                if (map[i].size() != 0) {
////                    for (int j = 0; j == 0 || j < nextPow; ++j) {
////                        std::cout << "Popping a block of size " << pow(2, i) << std::endl;
////                        delete map[i].allocNode();
////                        --counters[i];
////                    }
////                }
////                // TODO else
////            }
//
////            nextPow /= 2;
//
//            while (counters[i] * pow(2, i) > _poolSize - (!wasAllocated ? _currAlloc + size : _currAlloc)) {
//                std::cout << "Popping a block of size " << pow(2, i) << std::endl;
//                if (i == check) {
//                    ret = map[i].allocNode();
//                    allocated[i].addNode(ret);
//
//                    if (!wasAllocated) {
//                        _currAlloc += size;
//                        wasAllocated = true;
//                    }
//
////                    allocated[i].add(ret);
//                } else {
////                    delete map[i].allocNode();
//                    FreeNode *toDel = map[i].allocNode();
//                    toDel->~FreeNode();
//                    free(toDel);
//                }
//
//                --counters[i];
//
//            }
//
//        }
//
////        --counters[check];
////        return map[check].allocNode();
//        if (!ret) {
//            std::cout << "Returning a nullptr. Not enough memory" << std::endl;
//        } else {
//            std::cout << "Returning a block of size " << (int) pow(2, check) << std::endl;
//        }
//        return ret;
//    }
//
//    return nullptr;
//}


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

//void MemoryManager::returnMemory(FreeNode *f) {
//
//    if (!f) {
//        std::cout << "Tried to delete a node, but it's actually nullptr" << std::endl;
//        return;
//    }
//
//    std::cout << "Attempting to return a memory of size " << f->getBlockSize() << std::endl;
//    int check = whichPowerOfTwo(f->getBlockSize());
//
//    if (check == -1) {
//        std::cerr << "Error returning memory" << std::endl;
//    } else {
//        char *block = f->getBlock();
//        size_t size = f->getBlockSize();
//        _currAlloc -= f->getBlockSize();
//        bool returned = false;
//
//        for (int i = 0; i < 11; ++i) {
//            size_t counter = (size_t) pow(2, i);
//
//            if (counter <= _poolSize) {
//
//                long j = 0, index = 0;
//
//                while ((counters[i] * pow(2, i)) + pow(2, i) <= _poolSize - _currAlloc) {
//                    char *filler;
//                    std::cout << "Adding a block of size " << pow(2, i) << std::endl;
//                    if (counter > size) {
//
//                        if (map[i - 1].pairs() > map[i].size()) {
//                            FreeNode* n = map[i-1].getHead();
//                            map[i].remove();
//                            while(n && n->next){
//                                if(n->next->getBlock()-n->getBlock() == pow(2,i-1)){
//                                    void *memToAlloc = (FreeNode *) malloc(sizeof(FreeNode));
//                                    if (!memToAlloc) {
//                                        std::cerr << "Couldn't allocate memory for block of size " << size << std::endl;
//                                        return;
//                                    }
//                                    map[i].add(new(memToAlloc) FreeNode(n->getBlock(), counter));
//                                    n=n->next;
//                                }
//                                n=n->next;
//                            }
//                            //filler = map[i - 1].getPairAt(block);
//                            // returned =map[i].getPairAt(block,map,i,counter,f,size,allocated,returned);
////                            filler = &block[(index * counter) - (size)];
//                            break;
//                        } else {
//                            filler = nullptr;
//                        }
//                    } else {
//                        filler = &block[index * counter + (counter - (int) (pow(2, i)))];
////                        if (!filler) {
////                            break;
////                        }
////                        if (returned) {
////                            map[i].add(new FreeNode(filler, counter));
////
////                        } else if (pow(2, i) == size) {
////                            returned = true;
////                            FreeList &list = allocated[whichPowerOfTwo(f->getBlockSize())];
////                            map[whichPowerOfTwo(f->getBlockSize())].add(list.removeNode(f));
////                        } else {
////                            map[i].add(new FreeNode(filler, counter));
////                        }
//                    }
////                    FreeNode* toAdd = (FreeNode*)malloc(sizeof(FreeNode));
////                    toAdd = new (toAdd) FreeNode(filler, counter);
////                    map[i].add(toAdd);
//
////                    if (pow(2, i) != size && filler) {
////                        map[i].add(new FreeNode(filler, counter));
////                    }
////                    else if(filler) {
////                        FreeList &list = allocated[whichPowerOfTwo(f->getBlockSize())];
////                        map[whichPowerOfTwo(f->getBlockSize())].add(list.removeNode(f));
////                        ++counters[i];
////                        break;
////                    }
////
////                    if(filler) {
////                        ++counters[i];
////                    }
////                    else{
////                        break;
////                    }
////                    if (!filler) {
////                        break;
////                    }
//                    if (!returned) {
//                        //map[i].add(new FreeNode(filler, counter));
//
//                        if (pow(2, i) == size) {
//                            returned = true;
//                            FreeList &list = allocated[whichPowerOfTwo(f->getBlockSize())];
//                            map[whichPowerOfTwo(f->getBlockSize())].add(list.removeNode(f));
//                        } else {
//                            void *memToAlloc = (FreeNode *) malloc(sizeof(FreeNode));
//                            if (!memToAlloc) {
//                                std::cerr << "Couldn't allocate memory for block of size " << size << std::endl;
//                                return;
//                            }
//                            map[i].add(new(memToAlloc) FreeNode(filler, counter));
//                        }
//                    }
//                    ++counters[i];
//                    j += counter, ++index;
//                }
//
//            }
//        }
//    }
//
//    /* Removing specific node from the freelist */
////    FreeList &list = allocated[whichPowerOfTwo(f->getBlockSize())];
////    map[whichPowerOfTwo(f->getBlockSize())].add(list.removeNode(f));
//
//    //delete f;
//}

MemoryManager::~MemoryManager() {
//    delete[] map;

    if (!map) {
        return;
    }
    for (int i = 0; i < 11; ++i) {
        map[i].~FreeList();
    }
    free(map);

    if (valgrindFlag) {

        int bytesForgotten = 0;
        int placesForgotten = 0;
        for (int i = 0; i < 11; ++i) {
            if (allocated[i].size() > 0) {
                std::cout << "Lost " << allocated[i].size() << " blocks of size " << pow(2, i) << std::endl;
                bytesForgotten += allocated[i].size() * pow(2, i);
                placesForgotten += allocated[i].size();

//            delete (allocated[i]);
            }
        }
        if (bytesForgotten > 0) {
            std::cout << "\t\t\t\tTotal: lost " << bytesForgotten << " bytes in " << placesForgotten << " places"
                      << std::endl;
        } else {
            std::cout << "No memory leaks at all" << std::endl;
        }
    }

    for (int i = 0; i < 11; ++i) {
        allocated[i].~FreeList();
    }
    free(allocated);

//    delete[] allocated;
}

void MemoryManager::printCurrMemoryState() {

    for (int i = 0; i < 11; ++i) {

        FreeNode *it = map[i].getHead();

        if (it) {
//            std::cout << "This list has " << map[i].pairs() << " pairs" << std::endl;
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

void MemoryManager::printCurrMemoryStateShortly() {
    for (int i = 0; i < 11; ++i) {
        std::cout << "There are " << counters[i] << " free blocks of size " << pow(2, i) << std::endl;
    }
}

void MemoryManager::init() {

    char *pool = _mmpl.getPool();
    size_t size = _poolSize;

    for (int i = 0; i < 11; ++i) {
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
    for (int i = 0; i < 11; ++i) {

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

//    std::cout << "Got a memory request of size " << size << ", but will return memory of size "
//              << (int) pow(2, ceil(log(size) / log(2))) << std::endl;

    FreeNode *ret = nullptr;

    size = (size_t) pow(2, ceil(log(size) / log(2)));
    bool wasAllocated = false;

    int check = whichPowerOfTwo(size);
    if (check != -1) {

        int nextPow = (int) pow(2, ceil(log(size) / log(2)));
        for (int i = 0; i < 11; ++i) {

//            if (i != check) {
//                if (map[i].size() != 0) {
//                    for (int j = 0; j == 0 || j < nextPow; ++j) {
//                        std::cout << "Popping a block of size " << pow(2, i) << std::endl;
//                        delete map[i].allocNode();
//                        --counters[i];
//                    }
//                }
//                // TODO else
//            }

//            nextPow /= 2;

            while (counters[i] * pow(2, i) > _poolSize - (!wasAllocated ? _currAlloc + size : _currAlloc)) {
//                std::cout << "Popping a block of size " << pow(2, i) << std::endl;

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
//            std::cout << "Returning a block of size " << (int) pow(2, check) << std::endl;
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

//    std::cout << "Attempting to return a memory of size " << size << std::endl;
    int check = whichPowerOfTwo(size);

    if (check == -1) {
        std::cerr << "Error returning memory" << std::endl;
    } else {
        char *block = f;
//        size_t size = size;
        _currAlloc -= size;
        bool returned = false;

        for (int i = 0; i < 11; ++i) {
            size_t counter = (size_t) pow(2, i);

            if (counter <= _poolSize) {

                long j = 0, index = 0;

                while ((counters[i] * pow(2, i)) + pow(2, i) <= _poolSize - _currAlloc) {
                    char *filler;
//                    std::cout << "Adding a block of size " << pow(2, i) << std::endl;
                    if (counter > size) {

                        if (map[i - 1].pairs() > map[i].size()) {
                            FreeNode *n = map[i - 1].getHead();
                            map[i].remove();
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
                            counters[i] = map[i].size();
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

    for (int i = 0; i < 11; ++i) {
        int size = allocated[i].searchBlockSize(block);
        if (size != -1) {
//            std::cout << "\t\t\t\t\t\t\t\t\t\t\t\tThe node removed is of size " << size << std::endl;
            returnMemoryBlock(block, (size_t) size);
        }
    }
}

bool MemoryManager::isReady() {
    return _readyToGo;
}







