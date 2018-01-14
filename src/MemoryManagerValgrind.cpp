//
// Created by damir on 14-Jan-18.
//

#include <cmath>
#include "../hdr/MemoryManagerValgrind.h"


MemoryManagerValgrind::MemoryManagerValgrind(size_t size, bool valgrind)
        : MemoryManager(size, valgrind) {}


MemoryManagerValgrind::MemoryManagerValgrind()
        : MemoryManager() {}


void MemoryManagerValgrind::setValgrind(bool is) {
    valgrindFlag = is;
}


MemoryManagerValgrind::~MemoryManagerValgrind() {
    if (valgrindFlag) {
        int bytesForgotten = 0;
        int placesForgotten = 0;
        for (int i = 0; i < 11; ++i) {
            if (allocated[i].size() > 0) {
                std::cout << "Lost " << allocated[i].size() << " blocks of size " << pow(2, i) << std::endl;
                bytesForgotten += allocated[i].size() * pow(2, i);
                placesForgotten += allocated[i].size();
            }
        }
        if (bytesForgotten > 0) {
            std::cout << "\t\t\t\tTotal: lost " << bytesForgotten << " bytes in " << placesForgotten
                      << " places\n\n*****Showing the locations of the lost blocks of memory:******\n"
                      << std::endl;

            printCurrAllocatedMemoryState();
        } else {
            std::cout << "No memory leaks at all" << std::endl;
        }
    }
}



