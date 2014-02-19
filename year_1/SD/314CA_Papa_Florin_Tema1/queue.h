#include "list.h"

template <typename T>
class Queue {
private:
	//De ce nu mai este nevoie sa retinem head, tail si size?
	LinkedList<T> queueList;
 
public:
	// Constructor
	Queue();
 
	// Destructor
	~Queue();
 
	void enqueue(T timp,T durata,T pid,T prior);
	T dequeue();
	void addBetween(T timp,T durata,T pid,T prior);
	T front();
	void decreaseDurata();
	T modifyPrior();
	T increaseTime();
	T getTime();
	T getDurata();
	T getPid();
	T getPrior();
	int isEmpty();
};


