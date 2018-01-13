//
// Created by damir on 10-Jan-18.
//

#include "../hdr/FreeList.h"
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
        }
        tail = nullptr;
        head = nullptr;
        free(head);
    }
}

size_t FreeList::size() const {
    return _size;
}

//  Like LinkedList's clear
char *FreeList::alloc() {
    if (!head) {
        return nullptr;
    }
    FreeNode *ret = head;

    head = head->next;
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
                f->prev = tmp->prev;
                f->next = tmp;
                tmp->prev = f;
                f->prev->next = f;
            }
        }
    }
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
}

void FreeList::addNode(FreeNode *f) {
    f->next = f->prev = nullptr;
    FreeNode *substitutor = f;
    if (!head) {
        head = tail = substitutor;
    } else {
        head->prev = substitutor;
        substitutor->next = head;
        head = substitutor;
    }
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


void FreeList::clear() {
    if (head) {
        while (head != tail) {
            FreeNode *curr = head;
            head = head->next;
            curr->~FreeNode();
            free(curr);
        }
        head->~FreeNode();
        free(head);
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
        FreeNode *node = head;
        head = head->next;
        node->next = node->prev = nullptr;
        --_size;
        ret = node->getBlock();
        node->~FreeNode();
        free(node);
        return ret;
    }

    while (node->getBlock() != block && node->next != nullptr) {
        node = node->next;
    }
    if (node->getBlock() == block) {
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
    } else {
        return -1;
    }
}
