#ifndef __HEAP_H
#define __HEAP_H
 
#include <cstdio>
#include <cstdlib>
#include "node.h"

template <typename T>
class Heap
{
public:
    Heap();
    ~Heap();
 
    int parent(int poz);
 
    int leftSubtree(int poz);
 
    int rightSubtree(int poz);
 
    void pushUp(int l);
 
    void pushDown(int l);
 
    void insertElement(T symbol,T frecv,T address);
 
    Node<T> peek();
 
    Node<T> extractMin();

    int getDim();

    void getHeap();

private:
    Node<T> nodes[256];
    int dimVect;
    int capVect;
};
 
#endif // __HEAP_H
