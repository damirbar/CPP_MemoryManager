#include <iostream>
#include "hdr/FreeList.h"
#include "hdr/MemPool.h"
#include "hdr/MemoryManager.h"

using namespace std;

static MemoryManager manager(15, true);

void *operator new (size_t size) {
    return manager.getMemoryBlock(size);
}


int main() {

//    static MemoryManager manager(15, true);

//    FreeList list;
//    MemPool &mmpl = MemPool::getInstance();
//    mmpl.setPool(32);
//
//    char *block = new char;
//    char *block2 = new char;
//
//    list.addBlock(block, sizeof(char));
//    list.addBlock(block2, sizeof(char));
//
//    list.removeNodeOfBlock(block2);

    manager.printCurrMemoryState();

//    double *d = (double *)(manager.getMemoryBlock(sizeof(double)));
    double *d = new double;
    *d = 11;

    manager.printCurrMemoryState();

    cout << "d = ";
    printf("%p", d);
    cout << "and *d = " << *d << endl;

//    manager.returnMemoryBlock((char*)d, sizeof(double));

    manager.printCurrMemoryState();


    std::cout << "\n\n\n\n----------------Main ended----------------\n\n\n\n" << std::endl;
    return 0;
}