//
// Created by damir on 11-Jan-18.
//

#ifndef CPP_EX3_V3_MEMORYMANAGER_H
#define CPP_EX3_V3_MEMORYMANAGER_H



#include <cstdlib>
#include "MemPool.h"
#include "FreeList.h"


/** The custom memory manager.
 *  This class uses every other class of this project.
 *  The memory manager has a total of 22 FreeLists:
 *  -   11 lists of free memory blocks of 11 powers of 2:
 *      +   A list for blocks of size 2^0.
 *      +   A list for blocks of size 2^1.
 *      +   A list for blocks of size 2^2.
 *      +   A list for blocks of size 2^3.
 *      +   A list for blocks of size 2^4.
 *      +   A list for blocks of size 2^5.
 *      +   A list for blocks of size 2^6.
 *      +   A list for blocks of size 2^7.
 *      +   A list for blocks of size 2^8.
 *      +   A list for blocks of size 2^9.
 *      +   A list for blocks of size greater than 2^9.
 *
 *  -   11 lists of allocated memory blocks of 11 powers of 2:
 *      +   Same like the lists of free memory.
 *
 *  The manager also has a reference to the singleton memory pool.
 *  The manager has other fields:
 *  -   poolSize         - self explanatory.
 *  -   currAlloc        - The number of bytes allocated.
 *  -   valgrindFlag     - determines if the user wants a full output of memory leaks.
 *  -   counters[]       - an array which tells the manager how many nodes are in which list
 *      of the free memory.
 *  -   readyToGo        - determines if all fields were initialized correctly.
 *  -   elaboratedOutput - determines if the user wants a more elaborated output of the manager's actions.
 */
class MemoryManager {

    FreeList *map;
    FreeList *allocated;
    MemPool &_mmpl;
    size_t _poolSize;
    size_t _currAlloc;

    bool valgrindFlag;

    int counters[11];

    bool _readyToGo;

    bool _elaboratedOutput;

    /*  Returns which power of 2 n is.
     *  On failure: returns -1;
     *  If the power is greater than 9: returns 9.
     *  Else: returns the correct power.
     */
    int whichPowerOfTwo(size_t n);
public:

    //  Ctor. Receives pool size and a valgrind flag.
    explicit MemoryManager(size_t size, bool valgrind=false);

    //  Default ctor.
    MemoryManager();

    //  Sets the size of the memory manager which actually sets the pool size.
    void setSize(size_t size);

    //  Sets the valgrind flag.
    void setValgrind(bool is);

    //  Sets the elaborated output flag.
    void setElaboratedOutput(bool is);

    //  Tells if the manager is ready for action.
    bool isReady();

    /*  Calls the dtor of the free lists and freeing their memory.
     *  If the valgrind flag was set, a full output will be given.
     */
    ~MemoryManager();

    /*  Allocates a memory block of the given size.
     *  What it actually does:
     *  -   Determines which power of two is "size" (lets call the power i).
     *  -   Removes a block from "map[i]" and saves it.
     *  -   For every 0 <= j <= 11, removes this address from every map[j].
     *  -   Returns a pointer to the memory block.
     */
    char *getMemoryBlock(size_t size);


    /*  Returns a memory block to the pool.
     *  What it actually does:
     *  -   Determines which power of two is "size" (lets call the power i).
     *  -   Removes the correct block from "allocated[i]" and puts it in "map[i]"
     *  -   Adds blocks to all of the j's which correspond 0 < j < i.
     *  -   Checks if the returned block completes consecutive pairs to
     *      let a user get a bigger block, e.g:
     *          current blocks of 2 bytes:
     *              [0x00, 0x06]
     *          current blocks of 4 bytes:
     *              []
     *          # Returning a the block 0x2 of size 2 #
     *
     *          current blocks of 2 bytes:
     *              [0x00, 0x02, 0x06]
     *          current blocks of 4 bytes:
     *              [0x00]
     */
    void returnMemoryBlock(char *f, size_t size);

    //  Searches for the size of the memory block given and calls 
    void searchSizeAndReturnMemory(char *block);

    void printCurrMemoryState();

    void printCurrAllocatedMemoryState();

    void init();
};


#endif //CPP_EX3_V3_MEMORYMANAGER_H
