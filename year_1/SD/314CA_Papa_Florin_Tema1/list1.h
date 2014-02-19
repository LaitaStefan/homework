#include<stdlib.h>
#include<stdio.h>

template <typename T> struct Node1 {
 
public:
 
	T timp;
	T coada;
	Node1<T> *next;
	Node1<T> *prev;
};
 
template <typename T> class LinkedList1{
private:
 
	Node1<T> *pFirst, *pLast;
 
public:
 
	
	LinkedList1();


	~LinkedList1();
 
	
	void addFirst(T timp,T coada);
 
	
	void addLast(T timp,T coada);
	
	
	T removeFirst();
	

	T removeLast();
	

	Node1<T>* getFirst();
 
	
	int  isEmpty();
	
};
