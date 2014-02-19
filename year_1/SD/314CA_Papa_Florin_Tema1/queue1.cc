#include "queue1.h"

template <typename T>
Queue1<T>::Queue1()
{
}

template <typename T>
Queue1<T>::~Queue1()
{
}

template <typename T>
void Queue1<T>::enqueue(T timp,T coada)
{
	queueList.addLast(timp,coada);
}

template <typename T>
T Queue1<T>::dequeue()
{
	return queueList.removeFirst();
}

template <typename T>
T Queue1<T>::getCoada()
{
	Node1<T> *q;
	q=queueList.getFirst();
	return q->coada;
}

template <typename T>
T Queue1<T>::getTime()
{
	Node1<T> *q;
	q=queueList.getFirst();
	return q->timp;
}

template <typename T>
int Queue1<T>::isEmpty()
{	
	return queueList.isEmpty();
}

template class Queue1<int>;
