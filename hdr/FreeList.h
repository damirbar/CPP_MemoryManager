//
// Created by damir on 10-Jan-18.
//

#ifndef CPP_EX3_V3_FREELIST_H
#define CPP_EX3_V3_FREELIST_H

#include <cstdlib>
#include "FreeNode.h"


/** This is the FreeList. it has FreeNodes and various methods.
 *  For more information about FreeNodes, go to "FreeNode.h".
 *
 *  The list has a pointer to its head and tail. It has a field size
 *  which is the number of nodes within the list and a field numOfPairs
 *  which is the number of the consecutive pairs (in memory address
 *  matters) of nodes within the list.
 */
class FreeList {

    friend class MemoryManager;

    FreeNode *head;
    FreeNode *tail;

    size_t _size;
    size_t _numOfPairs;

    //  Returns the head of the list.
    FreeNode *getHead();

    //  Updates the number of consecutive pairs of nodes within this list.
    void updatePairs();
public:

    //  Initializes the list.
    FreeList();

    //  Frees all of the memory which was allocated for the nodes.
    ~FreeList();

    //  Returns the current number of nodes within this list.
    size_t size() const;

    //  Updates and returns the number of consecutive pairs of nodes within this list.
    size_t pairs();

    //  Adds a node to the list (sorted by memory address).
    void add(FreeNode *f);

    //  Returns the head node and removes it from the list.
    FreeNode *allocNode();

    //  Returns the block of the head node and removes it from the list.
    char *alloc();

    //  Removes a specific node from the list.
    FreeNode *removeNode(FreeNode *f);

    //  Adds a node to the beginning of the list.
    void addNode(FreeNode *f);

    //  Returns the first block of a corresponding pair of nodes.
    char *getPairAt(char *string);

    //  Clears the list by removing all nodes.
    void clear();

    //  Returns a block of a specific node, searching it by its memory block.
    char* removeNodeOfBlock(char *block);

    //  Adds a new node by giving it a memory block and size.
    void addBlock(char *block, size_t size);

    //  Returns the size of a specific memory block by iterating the list.
    int searchBlockSize(char *block);
};


#endif //CPP_EX3_V3_FREELIST_H
