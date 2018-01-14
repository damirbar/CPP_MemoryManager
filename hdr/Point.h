//
// Created by damir on 14-Jan-18.
//

#ifndef CPP_EX3_V3_POINT_H
#define CPP_EX3_V3_POINT_H

/*  This is a regular point class. It was created to demonstrate the allocation
 *  of a custom object on the memory manager's heap.
 */
class Point {

    double _x, _y;
public:

    Point(double x, double y);

    Point();

    ~Point();

};


#endif //CPP_EX3_V3_POINT_H
