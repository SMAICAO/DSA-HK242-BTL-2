#ifndef SAMPLEFUNC_H
#define SAMPLEFUNC_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

#include "util/Point.h"
#include "util/ArrayLib.h"



inline int minHeapComparator(int& a, int& b){
    if (a < b) return -1;
    else if(a > b) return +1;
    else return 0;
}
inline int maxHeapComparator(int& a, int& b){
    if (a < b) return +1;
    else if(a > b) return -1;
    else return 0;
}

inline int minHeapComparator(Point& a, Point& b){
    float ra = sqrt(a.getX()*a.getX() + a.getY()*a.getY());
    float rb = sqrt(b.getX()*b.getX() + b.getY()*b.getY());
    if (ra < rb) return -1;
    else if(ra > rb) return +1;
    else return 0;
}
inline int maxHeapComparator(Point& a, Point& b){
    float ra = sqrt(a.getX()*a.getX() + a.getY()*a.getY());
    float rb = sqrt(b.getX()*b.getX() + b.getY()*b.getY());
    if (ra < rb) return +1;
    else if(ra > rb) return -1;
    else return 0;
}
inline int minHeapComparator(Point*& a, Point*& b){
    float ra = sqrt(a->getX()*a->getX() + a->getY()*a->getY());
    float rb = sqrt(b->getX()*b->getX() + b->getY()*b->getY());
    if (ra < rb) return -1;
    else if(ra > rb) return +1;
    else return 0;
}
inline int maxHeapComparator(Point*& a, Point*& b){
    float ra = sqrt(a->getX()*a->getX() + a->getY()*a->getY());
    float rb = sqrt(b->getX()*b->getX() + b->getY()*b->getY());
    if (ra < rb) return +1;
    else if(ra > rb) return -1;
    else return 0;
}
////////////////////////////////////////////////////////////
inline string point2str(Point& point){
    stringstream os;
    os  << "("  << point.getX() 
                << ", "
                << point.getY()
        << ")";
    return os.str();
}
inline string point2str(Point*& point){
    stringstream os;
    os  << "("  << point->getX() 
                << ", "
                << point->getY()
        << ")";
    return os.str();
}

inline string int2str(int& value){
    stringstream os;
    os << value;
    return os.str();
}
inline string int2str(int*& value){
    stringstream os;
    os << *value;
    return os.str();
}

#endif /* SAMPLEFUNC_H */