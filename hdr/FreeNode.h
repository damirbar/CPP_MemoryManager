//
// Created by damir on 10-Jan-18.
//

#ifndef CPP_EX3_V3_FREENODE_H
#define CPP_EX3_V3_FREENODE_H

#include <cstdlib>
#include <iostream>

class FreeNode {

    size_t _blockSize;
    char *_block;

public:

    static int id;
    const int _id;

    FreeNode *next;
    FreeNode *prev;

    explicit FreeNode(char *block, size_t size);

    FreeNode();

    void setBlock(char *block);

    void setSize(size_t size);

    ~FreeNode();

    size_t getBlockSize() const;

    char *getBlock();

    friend std::ostream &operator<<(std::ostream &os, FreeNode &node);
};




#endif //CPP_EX3_V3_FREENODE_H
