//
// Created by damir on 10-Jan-18.
//

#include "../hdr/FreeList.h"


#include <iostream>
#include <cmath>


FreeList::FreeList()
        : _size(0), _numOfPairs(0), head(nullptr), tail(nullptr) {}


FreeList::~FreeList() {
    if (head) {
        while (head) {
            FreeNode *curr = head;
            head = head->next;
            curr->~FreeNode();
            free(curr);
//            delete curr;
        }
        tail = nullptr;
        head = nullptr;
//        head->~FreeNode();
        free(head);
//        delete head;
    }
}

size_t FreeList::size() const {
    return _size;
}

//  Like LinkedList's remove
char *FreeList::alloc() {
//    FreeNode *it = head;

    if (!head) {
        return nullptr;
    }
    FreeNode *ret = head;

    head = head->next;
//    char *blockRet = ret->getBlock();
//    delete ret;

//    return ret->getBlock();

//    while (it && !it->_free) {
//        it = it->next;
//    }
//    if (it) {
//        it->_free = false;
//        it->prev->next = it->next;
//        it->next->prev = it->prev;
//        return it->getBlock();
//    }
    return ret->getBlock();
}


void FreeList::add(FreeNode *f) {
    if (!head) {
        head = tail = f;
    } else if (size() == 1) {
        if (f->getBlock() > head->getBlock()) {
            tail->next = f;
            f->prev = tail;
            tail = f;
            tail->next = nullptr;
        } else {
            head->prev = f;
            f->next = head;
            head = f;
        }
    } else {
        FreeNode *tmp = head;
//        while ((tmp != tail) && f->getBlock() > tmp->getBlock()) {
//            tmp = tmp->next;
//        }
        if (tmp->next == head) {
            if (head->getBlock() > f->getBlock()) {
                head->prev = f;
                f->next = head;
                head = f;
            } else {
                head->next = f;
                f->prev = head;
                tail = f;
            }
            return;
        }
        while (tmp && tmp->next && f->getBlock() > tmp->getBlock()) {
            tmp = tmp->next;
        }
        if (tmp == tail) {
            if (f->getBlock() > tail->getBlock()) {
                tail->next = f;
                f->prev = tail;
                tail = f;
                tail->next = nullptr;
            } else {
                tail->prev->next = f;
                f->prev = tail->prev;
                tail->prev = f;
                f->next = tail;
            }
        } else {
            if (tmp == head) {
                head->prev = f;
                f->next = head;
                head = f;
            } else {
//            if (tmp->next) {
                f->prev = tmp->prev;
                f->next = tmp;
                tmp->prev = f;
                f->prev->next = f;
            }
//            }
//            else{
//                tail->next=f;
//                tail=f;
//            }
//            f->prev = tmp;

        }
//        f->next = head;
//        head = f;
//        tail->next = f;
//        tail = f;
    }

    //updatePairs();
    ++_size;
}

FreeNode *FreeList::allocNode() {
    if (!head) {
        return nullptr;
    }

    FreeNode *ret = head;

    if (_size == 1) {
        head = tail = nullptr;
    } else {
        head = head->next;
    }

    //updatePairs();
    --_size;
    return ret;
}

FreeNode *FreeList::getHead() {
    return head;
}

FreeNode *FreeList::removeNode(FreeNode *f) {
    FreeNode *node = head;
    if (!head) {
        return nullptr;
    }
    if (node->getBlock() == f->getBlock()) {
        std::cout << "Removed the head node with id = " << node->_id << std::endl;
        FreeNode *node = head;
        head = head->next;
//        node = node->next;
        node->next = node->prev = nullptr;
        --_size;
        return node;
    }

    while (node->getBlock() != f->getBlock() && node->next != nullptr) {
        node = node->next;
    }
    if (node->getBlock() == f->getBlock()) {
        std::cout << "Removed the node with id = " << node->_id << std::endl;
        node->prev->next = node->next;
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
        node->next = node->prev = nullptr;
        --_size;
        return node;
    }
    //  updatePairs();
}

void FreeList::addNode(FreeNode *f) {
//    FreeNode *substitutor = new FreeNode(f->getBlock(), f->getBlockSize());
    f->next = f->prev = nullptr;
    FreeNode *substitutor = f;
    if (!head) {
        head = tail = substitutor;
    } else {
        head->prev = substitutor;
        substitutor->next = head;
        head = substitutor;
//        tail->next = substitutor;
//        tail = substitutor;
    }
    //   updatePairs();
    ++_size;
}

size_t FreeList::pairs() {
    updatePairs();
    return _numOfPairs;
}

void FreeList::updatePairs() {
    _numOfPairs = 0;
    FreeNode *node = head;
    while (node != tail && node->next) {
//        printf("node next block=%p node block=%p\n",node->next->getBlock(),node->getBlock());
//        //std::cout<<"node next block="<<node->next->getBlock()<<"node block="<< node->getBlock()<<std::endl;
//        std::cout<<"Diff="<<node->next->getBlock() - node->getBlock()<<std::endl;

        if (node->next->getBlock() - node->getBlock() == node->getBlockSize()) {
            _numOfPairs++;

            if (!node->next->next) {
                break;
            }
            node = node->next->next;
        } else {
            node = node->next;
        }
    }
}


char *FreeList::getPairAt(char *block) {
    FreeNode *node = head;

    while (node) {
        if (node->getBlock() == block && node->next &&
            node->next->getBlock() - node->getBlock() == node->getBlockSize()) {
            return node->getBlock();
        } else if (node->getBlock() == block && node->prev &&
                   node->getBlock() - node->prev->getBlock() == node->getBlockSize()) {
            return node->prev->getBlock();
        } else {
            node = node->next;
        }
    }
}


//bool
//FreeList::getPairAt(char *block, FreeList *map, int i, size_t counter, FreeNode *f, size_t size,
//                    FreeList *allocated, bool test) {
////    map[i].remove();
//    FreeNode *node = head;
//    while (node && node->next) {
//        if (node->next->getBlock() - block == node->getBlockSize() && !map[i].contains(block)) {
//            if (test) {
//                map[i].add(new FreeNode(block, counter));
//
//
//            } else if (pow(2, i) == size) {
//                test = true;
//                FreeList &list = allocated[whichPowerOfTwo(f->getBlockSize())];
//                map[whichPowerOfTwo(f->getBlockSize())].add(list.removeNode(f));
//            } else {
//                map[i].add(new FreeNode(block, counter));
//            }
//            node = node->next;
//        }
//        node = node->next;
//    }
//    FreeNode *node2 = map[i + 1].getHead();
//    map[i + 1].remove();
//    FreeNode *n = head;
//    while (n && n->next) {
//        if (n->next->getBlock() - n->getBlock() == counter) {
//            if (test) {
//                map[i + 1].add(new FreeNode(n->getBlock(), counter*2 ));
//            } else if (pow(2, i + 1) == size) {
//                test = true;
//                FreeList &list = allocated[whichPowerOfTwo(f->getBlockSize())];
//                map[whichPowerOfTwo(f->getBlockSize())].add(list.removeNode(f));
//            } else {
//                map[i + 1].add(new FreeNode(block, counter));
//            }
//            n = n->next;
//        }
//        n = n->next;
//    }
//    return test;
//}
//
//int FreeList::whichPowerOfTwo(size_t n) {
//    int counter = 0;
//
//    while (n % 2 == 0) {
//        counter++;
//        if (counter > 10)
//            break;
//        n /= 2;
//    }
//    if (n != 1 && n % 2 != 0) {
//        return -1;
//    }
//    return counter;
//}
//
//
//bool FreeList::contains(char *block) {
//    FreeNode *node = head;
//    while (node) {
//        if (node->getBlock() == block) {
//            return true;
//        }
//        node = node->next;
//    }
//    return false;
//}
//
void FreeList::remove() {
    if (head) {
        while (head != tail) {
            FreeNode *curr = head;
            head = head->next;
            curr->~FreeNode();
            free(curr);
//            delete curr;
        }
        head->~FreeNode();
        free(head);
//        delete head;
    }
    head = tail = nullptr;
    _size = 0;
}


char *FreeList::removeNodeOfBlock(char *block) {

    char *ret = nullptr;
    FreeNode *node = head;
    if (!head) {
        return nullptr;
    }
    if (node->getBlock() == block) {
//        std::cout << "Removed the head node with id = " << node->_id << std::endl;
        FreeNode *node = head;
        head = head->next;
//        node = node->next;
        node->next = node->prev = nullptr;
        --_size;
        ret = node->getBlock();
        node->~FreeNode();
        free(node);
//        delete node;
        return ret;
    }

    while (node->getBlock() != block && node->next != nullptr) {
        node = node->next;
    }
    if (node->getBlock() == block) {
//        std::cout << "Removed the node with id = " << node->_id << std::endl;
        node->prev->next = node->next;
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
        if (node == tail && node->prev) {
            tail = node->prev;
        }
        node->next = node->prev = nullptr;
        --_size;
        ret = node->getBlock();
        node->~FreeNode();
        free(node);
//        delete node;
        return ret;
    }
}


void FreeList::addBlock(char *block, size_t size) {
    void *memToAlloc = (FreeNode *) malloc(sizeof(FreeNode));
    if (!memToAlloc) {
        std::cerr << "Couldn't allocate memory for block of size " << size << std::endl;
        return;
    }
    FreeNode *toAlloc = new(memToAlloc) FreeNode(block, size);

    if (!head) {
        head = tail = toAlloc;
    } else if (_size == 1) {
        if (block > head->getBlock()) {
            tail->next = toAlloc;
            tail->next->prev = tail;
            tail = tail->next;
            tail->next = nullptr;
        } else {
            head->prev = toAlloc;;
            head->prev->next = head;
            head = head->prev;
        }
    } else {
        FreeNode *tmp = head;

        if (tmp->next == head) {
            if (head->getBlock() > block) {
                head->prev = toAlloc;
                head->prev->next = head;
                head = head->prev;
            } else {
                head->next = toAlloc;
                head->next->prev = head;
                tail = head->next;
            }
            return;
        }
        while (tmp && tmp->next && block > tmp->getBlock()) {
            tmp = tmp->next;
        }
        if (tmp == tail) {
            if (block > tail->getBlock()) {
                tail->next = toAlloc;
                tail->next->prev = tail;
                tail = tail->next;
                tail->next = nullptr;
            } else {
                tail->prev->next = toAlloc;
                tail->prev->next->prev = tail->prev;
                tail->prev = tail->prev->next;
                tail->prev->next->next = tail;
            }
        } else {
            if (tmp == head) {
                head->prev = toAlloc;
                head->prev->next = head;
                head = head->prev;
            } else {
                FreeNode *f = toAlloc;
                f->prev = tmp->prev;
                f->next = tmp;
                tmp->prev = f;
                f->prev->next = f;
            }

        }

    }
    ++_size;
}



int FreeList::searchBlockSize(char *block) {
    char *ret = nullptr;
    FreeNode *node = head;
    if (!head) {
        return -1;
    }

    while (node->getBlock() != block && node->next) {
        node = node->next;
    }
    if (node->getBlock() == block) {
        return node->getBlockSize();
    }
    else {
        return -1;
    }
}
