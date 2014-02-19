#include "list1.h"

template <typename T>
class Queue1 {
private:
	//De ce nu mai este nevoie sa retinem head, tail si size?
	LinkedList1<T> queueList;
 
public:
	// Constructor
	Queue1();
 
	// Destructor
	~Queue1();
 
	void enqueue(T timp,T coada);
	T dequeue();
	//T front();
	T getTime();
	T getCoada();
	int isEmpty();
};
