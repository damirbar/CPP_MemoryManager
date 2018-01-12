#include <iostream>
#include "hdr/FreeList.h"
#include "hdr/MemPool.h"
#include "hdr/MemoryManager.h"

using namespace std;

//static MemoryManager manager(15, true);
static MemoryManager manager;

void *operator new(size_t size) {
    return manager.getMemoryBlock(size);
}

void operator delete(void *p) {
//    manager.returnMemoryBlock(static_cast<char *>(p), 8);
    manager.searchSizeAndReturnMemory((char *) p);
}


int main(int argc, char *argv[]) {

    for (int i = 0; i < argc; ++i) {
        cout << "argv[" << i <<"] = " << argv[i] << endl;
    }

    size_t size = 0;
    bool valgrind = false;

    if (argc == 1) {
        cerr << "Need more arguments" << endl;
        return 1;
    }
    if (argc > 4) {
        cerr << "Too many arguments" << endl;
        return 1;
    }
    if (argc == 3) {
        string argOne = argv[1];
        if (argOne == "-s") {
            size = static_cast<size_t>(stoll((string) argv[2]));
        }
        else {
            cerr << "Wrong arguments" << endl;
            return 1;
        }
    }
    if (argc == 4) {
        if ((string) argv[1] == "-valgrind") {
            valgrind = true;

            if ((string) argv[2] == "-s") {
                size = static_cast<size_t>(stoll((string) argv[3]));
            }
            else {
                cerr << "Wrong arguments" << endl;
                return 1;
            }
        }
        else if ((string) argv[3] == "-valgrind") {
            valgrind = true;

            if ((string) argv[1] == "-s") {
                size = static_cast<size_t>(stoll((string) argv[2]));
            }
            else {
                cerr << "Wrong arguments" << endl;
                return 1;
            }
        }
    }


    manager.setSize(size);
    manager.setValgrind(valgrind);
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

    delete d;

    manager.printCurrMemoryState();


    std::cout << "\n\n\n\n----------------Main ended----------------\n\n\n\n" << std::endl;
    return 0;
}