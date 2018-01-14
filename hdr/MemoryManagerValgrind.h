//
// Created by damir on 14-Jan-18.
//

#ifndef CPP_EX3_V3_MEMORYMANAGERVALGRIND_H
#define CPP_EX3_V3_MEMORYMANAGERVALGRIND_H


#include "MemoryManager.h"


/*  This is a concrete class of the MemoryManager.
 *  This kind of memory manager has capability of setting the valgrind flag on to show
 *  the possible memory leaks.
 *
 *  This memory manager's fields:
 *
 *  -   valgrindFlag     - determines if the user wants a full output of memory leaks.
 */
class MemoryManagerValgrind : public MemoryManager {

public:

    //  Ctor. Receives pool size and a valgrind flag.
    explicit MemoryManagerValgrind(size_t size, bool valgrind=false);

    //  Default ctor.
    MemoryManagerValgrind();

    //  Sets the valgrind flag.
    void setValgrind(bool is);

    /*  Calls the dtor of the free lists and freeing their memory.
     *  If the valgrind flag was set, a full memory leaks output will be provided.
     */
    virtual ~MemoryManagerValgrind();

};


#endif //CPP_EX3_V3_MEMORYMANAGERVALGRIND_H
