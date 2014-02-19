#include "queue.h"

template <typename T>
Queue<T>::Queue()
{
}

template <typename T>
Queue<T>::~Queue()
{
}

template <typename T>
void Queue<T>::enqueue(T timp,T durata,T pid,T prior)
{
	queueList.addLast(timp,durata,pid,prior);
}

template <typename T>
void Queue<T>::addBetween(T timp,T durata,T pid,T prior)
{
	queueList.addBetween(timp,durata,pid,prior);
}

template <typename T>
T Queue<T>::dequeue()
{
	return queueList.removeFirst();
}

template <typename T>
T Queue<T>::getPid()
{
	Node<T> *q;
	q=queueList.getFirst();
	return q->pid;
}

template <typename T>
T Queue<T>::getTime()
{
	Node<T> *q;
	q=queueList.getFirst();
	return q->timp;
}

template <typename T>
T Queue<T>::getDurata()
{
	Node<T> *q;
	q=queueList.getFirst();
	return q->durata;
}

template <typename T>
T Queue<T>::getPrior()
{
	Node<T> *q;
	q=queueList.getFirst();
	return q->prior;
}

template <typename T>
void Queue<T>::decreaseDurata()
{
	queueList.decreaseDurata();
}

template <typename T>
T Queue<T>::increaseTime()
{
	return queueList.increaseTime();
}

template <typename T>
T Queue<T>::modifyPrior()
{
	return queueList.modifyPrior();
}

template <typename T>
int Queue<T>::isEmpty()
{	
	return queueList.isEmpty();
}

template class Queue<int>;
