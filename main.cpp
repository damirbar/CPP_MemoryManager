#include <iostream>
#include "hdr/FreeList.h"
#include "hdr/MemPool.h"
#include "hdr/MemoryManager.h"

using namespace std;

//static MemoryManager manager(15, true);
static MemoryManager manager;

void *operator new(size_t size) {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's new operator was called");
        return manager.getMemoryBlock(size);
    }

    return malloc(size);
}

void operator delete(void *p) {
//    manager.returnMemoryBlock(static_cast<char *>(p), 8);
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's delete operator was called");
        manager.searchSizeAndReturnMemory((char *) p);
        return;
    }
    free(p);
}

void *operator new[](size_t size) {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's new[] operator was called");
        return manager.getMemoryBlock(size);
    }
    return malloc(size);
}

void operator delete[](void *p) {
    if (manager.isReady()) {
        puts("\t\t\t\t\t\t\tMemoryManager's delete[] operator was called");
        manager.searchSizeAndReturnMemory((char *) p);
        return;
    }
    free(p);
}

int main(int argc, char *argv[]) {

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
        char *argOne = argv[1];
//        string argOneString = argOne;
        if ((string)argOne == "-s") {
            size = static_cast<size_t>(stoll((string) argv[2]));
        }
        else {
            cerr << "Wrong arguments" << endl;
            return 1;
        }
    }
    else if (argc == 4) {
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





    double *d = new double;
    *d = 11;

    manager.printCurrMemoryState();


    cout << "d = " << static_cast<void*>(d) << " and *d = " << *d << endl;

//    manager.returnMemoryBlock((char*)d, sizeof(double));

    delete d;

    manager.printCurrMemoryState();

    int *ints = new int[4];

    ints[0] = 4;
    ints[1] = 2;
    ints[2] = 0;
    ints[3] = 1;

    for (int i = 0; i < 4; ++i) {
        cout << "ints[" << i << "] = " << ints[i] << endl;
    }


    delete[] ints;


//    manager.printCurrMemoryState();


    std::cout << "\n\n\n\n----------------Main ended----------------\n\n\n\n" << std::endl;
    return 0;
}