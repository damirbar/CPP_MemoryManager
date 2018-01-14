#include <iostream>
#include "hdr/Point.h"
#include "hdr/Operators.h"

using namespace std;


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
        if ((string) argOne == "-s") {
            size = static_cast<size_t>(stoll((string) argv[2]));
        } else {
            cerr << "Wrong arguments" << endl;
            return 1;
        }
    } else if (argc == 4) {
        if ((string) argv[1] == "-valgrind") {
            valgrind = true;

            if ((string) argv[2] == "-s") {
                size = static_cast<size_t>(stoll((string) argv[3]));
            } else {
                cerr << "Wrong arguments" << endl;
                return 1;
            }
        } else if ((string) argv[3] == "-valgrind") {
            valgrind = true;

            if ((string) argv[1] == "-s") {
                size = static_cast<size_t>(stoll((string) argv[2]));
            } else {
                cerr << "Wrong arguments" << endl;
                return 1;
            }
        }
    }


    /*  Setting the heap size of the memory manager according to the size given
     *  after the '-s' flag. Also setting the valgrind flag to true if the flag
     *  '-valgrind' was given. If the valgrind flag was given, at the end of the
     *  program a minimal report will be given for how much bytes of memory was
     *  lost and in how many places.
     */
    manager.setSize(size);
    manager.setValgrind(valgrind);
    //manager.setElaboratedOutput(true);  //  Optional, for more output.


    /*  This method will print the current state of the heap. It will, at this
     *  point, show the free memory blocks for each size for each power of 2.
     */
    manager.printCurrMemoryState();

    /*  Creating a new pointer-to-double by using the overloaded global new operator.
     *  The memory will be provided by the custom memory manager, and it will be
     *  located on its "heap", which is the manager's memory pool.
     */
    double *d = new double;
    *d = 11;

    /*  At this point, we will be able to see that 8 bytes of memory were allocated,
     *  which means there will be 8 bytes less to show.
     */
    manager.printCurrMemoryState();

    /*  Printing the address of the double, which will correspond to the missing
     *  bytes on the manager's heap, and the value of the double.
     */
    cout << "d = " << static_cast<void *>(d) << " and *d = " << *d << endl;

    /*  Deleting the pointer by using the overloaded global delete operator.
     *  The memory will be returned to the custom memory manager.
     */
    delete d;

    /*  At this point, we will be able to see that the 8 bytes that were missing,
     *  after allocating the memory for the double, were returned to the memory
     *  mamanger's heap in the correct order.
     */
    manager.printCurrMemoryState();


    /*  Allocating memory for array of ints by using the overloaded global new[]
     *  operator. The memory will be provided by the custom memory manager, and
     *  it will be located on its "heap", which is the manager's memory pool.
     */
    int *ints = new int[4];

    /*  Assigning values to the array of ints. */
    ints[0] = 4;
    ints[1] = 2;
    ints[2] = 0;
    ints[3] = 1;

    for (int i = 0; i < 4; ++i) {
        cout << "The address is " << static_cast<void *>(ints + i) << " and ints[" << i << "] = " << ints[i] << endl;
    }


    /*  Deleting the pointer by using the overloaded global delete[] operator.
     *  The memory will be returned to the custom memory manager.
     */
    delete[] ints;


    /*  At this point, we will be able to see that the heap is as it was at
     *  the beginning. All of the memory was restored
     */
    manager.printCurrMemoryState();


    /*  This is the end of a correct allocate-deallocate program. after this I
     *  will allocate memory and won't free it, just to demonstrate how my
     *  valgrind works.
     */



    /*  Total allocations:
     *  -   1 int (of size 4)
     *  -   3 shorts (of size 2)
     *  -   2 chars as an array (2 bytes total)
     *  -   1 Point object (of size 16)
     */
    int *noDeallocateInt       = new int;
    short *noDeallocateShort1  = new short;
    short *noDeallocateShort2  = new short;
    short *noDeallocateShort3  = new short;
    char *noDeallocateChar1    = new char[2];
    Point *p = new Point();


    std::cout << "\n\n\n\n----------------Main function ended----------------\n\n\n\n" << std::endl;
    return 0;
}