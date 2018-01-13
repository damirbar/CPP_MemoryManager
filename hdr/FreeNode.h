//
// Created by damir on 10-Jan-18.
//

#ifndef CPP_EX3_V3_FREENODE_H
#define CPP_EX3_V3_FREENODE_H

#include <cstdlib>
#include <iostream>


/** A class for the FreeList's node.
 *  It has the information of its block, such as address and size.
 *  It has a unique ID and a pointer to his next and previous node.
 */
class FreeNode {

    size_t _blockSize;
    char *_block;

public:

    static int id;
    const int _id;

    FreeNode *next;
    FreeNode *prev;

    //  Explicit ctor which receives a memory block and its size.
    explicit FreeNode(char *block, size_t size);

    //  Default ctor
    FreeNode();

    //  Set the node's memory block.
    void setBlock(char *block);

    //  Set the node's memory block's size.
    void setSize(size_t size);

    //  Dtor does nothing.
    ~FreeNode();

    //  Returns the memory block's size.
    size_t getBlockSize() const;

    //  Returns the memory block itself.
    char *getBlock();

    //  Output operator for printing purposes.
    friend std::ostream &operator<<(std::ostream &os, FreeNode &node);
};




#endif //CPP_EX3_V3_FREENODE_H
