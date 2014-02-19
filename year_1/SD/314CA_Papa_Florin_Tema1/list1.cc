#include "list1.h"

template <typename T>
LinkedList1<T>::LinkedList1()
{
	pFirst=NULL;
	pLast=NULL;
}

template <typename T>
LinkedList1<T>::~LinkedList1()
{
	while(pFirst!=NULL)
	{
		removeFirst();
	}
}
 

template <typename T>
void LinkedList1<T>::addFirst(T timp,T coada)
{
	Node1<T> *node;
	node=new Node1<T>;
	node->timp=timp;
	node->coada=coada;
	if(pFirst==NULL)
	{
		pFirst=node;
		pLast=node;	
		node->next=node;
		node->prev=node;
	}
	else
	{
		pLast->next=node;
		node->next=pFirst;
		node->prev=pLast;
		pFirst->prev=node;
		pFirst=node;
	}
}
 

template <typename T>
void LinkedList1<T>::addLast(T timp,T coada)
{		
	Node1<T> *node;
	node=new Node1<T>;
	node->timp=timp;
	node->coada=coada;
	if(pLast==NULL)
	{
		pFirst=node;
		pLast=node;	
		node->next=node;
		node->prev=node;
	}
	else
	{
		pFirst->prev=node;
		node->next=pFirst;
		node->prev=pLast;
		pLast->next=node;
		pLast=node;
	}
}

template <typename T>
T LinkedList1<T>::removeFirst()
{
	Node1<T> *aux=pFirst;
	T val;
	if(pFirst->prev==pFirst)
	{
		pFirst=NULL;
		pLast=NULL;
		val=aux->coada;
		delete aux;
		return val;
	}
	else
	{
		val=pFirst->coada;
		pFirst=pFirst->next;
		pLast->next=pFirst;
		if(pLast==pFirst)
		{
			pLast->prev=pLast;
		}
		delete aux;
		return val;
	}
	return -1;
}

template <typename T>
T LinkedList1<T>::removeLast()
{
	Node1<T> *aux=pLast;
	T val;
	if(pLast->next==pLast)
	{
		pFirst=NULL;
		pLast=NULL;
		val=aux->coada;
		delete aux;
		return val;
	}
	else
	{
		val=aux->coada;
		pLast=pLast->prev;
		pFirst->prev=pLast;
		if(pFirst==pLast)
		{
			pFirst->next=pFirst;
		}
		delete aux;
		return val;
	}
	return -1;
}
 
template <typename T>
Node1<T>* LinkedList1<T>::getFirst()
{
	return pFirst;
}

template <typename T>
int  LinkedList1<T>::isEmpty()
{
	if(pFirst==NULL || pLast==NULL)
	{
		return 1;
	}
	return 0;
}

template class LinkedList1<int>;
