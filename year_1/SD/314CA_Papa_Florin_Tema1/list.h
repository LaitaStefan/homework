#include<stdlib.h>
#include<stdio.h>

template <typename T> struct Node {
 
public:
 
	T timp;
	T durata;
	T pid;
	T prior;
	Node<T> *next;
	Node<T> *prev;
};
 
template <typename T> class LinkedList{
private:
 
	Node<T> *pFirst, *pLast;
 
public:
 
	
	LinkedList();


	~LinkedList();
 
	
	void addFirst(T timp,T durata,T pid,T prior);
 
	
	void addLast(T timp,T durata,T pid,T prior);
	void addBetween(T timp,T durata,T pid,T prior);//verif
	
	
	T removeFirst();
	

	T removeLast();
	
	T decreaseDurata();

	T modifyPrior();

	T increaseTime();

	Node<T>* getFirst();
 	
	int  isEmpty();
	
};
