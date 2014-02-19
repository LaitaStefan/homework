#include "list.h"

template <typename T>
LinkedList<T>::LinkedList()
{
	pFirst=NULL;
	pLast=NULL;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
	while(pFirst!=NULL)
	{
		removeFirst();
	}
}
 

template <typename T>
void LinkedList<T>::addFirst(T timp,T durata,T pid,T prior)
{
	Node<T> *node;
	node=new Node<T>;
	node->timp=timp;
	node->durata=durata;
	node->pid=pid;
	node->prior=prior;
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
void LinkedList<T>::addLast(T timp,T durata,T pid,T prior)
{		
	Node<T> *node;
	node=new Node<T>;
	node->timp=timp;
	node->durata=durata;
	node->pid=pid;
	node->prior=prior;
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
void LinkedList<T>::addBetween(T timp,T durata,T pid,T prior)
{
	Node<T> *node,*aux=pFirst;
	if(pFirst==NULL) { addFirst(timp,durata,pid,prior); }
	else
	{
		if(aux==pLast)
		{
			if(timp<aux->timp) { addFirst(timp,durata,pid,prior); }
			else { addLast(timp,durata,pid,prior); }
		}
		else
		{
			node=new Node<T>;
			node->timp=timp;
			node->durata=durata;
			node->pid=pid;
			node->prior=prior;
			while(aux!=pLast)
			{
				if(timp<aux->timp)
				{
					node->next=aux;
					node->prev=aux->prev;
					aux->prev->next=node;
					aux->prev=node;
					if(aux==pFirst) { node=pFirst; }
					break;
				}
				aux=aux->next;
			}
			if(aux==pLast)
			{
				if(timp<aux->timp)
				{
					node->next=aux;
					node->prev=aux->prev;
					aux->prev->next=node;
					aux->prev=node;
				}
				else
				{
					addLast(timp,durata,pid,prior);
					delete node;
				}	
			}
		}
	}
}
					

template <typename T>
T LinkedList<T>::removeFirst()
{
	Node<T> *aux=pFirst;
	T val;
	if(pFirst->prev==pFirst)
	{
		pFirst=NULL;
		pLast=NULL;
		val=aux->pid;
		delete aux;
		return val;
	}
	else
	{
		val=pFirst->pid;
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
T LinkedList<T>::removeLast()
{
	Node<T> *aux=pLast;
	T val;
	if(pLast->next==pLast)
	{
		pFirst=NULL;
		pLast=NULL;
		val=aux->pid;
		delete aux;
		return val;
	}
	else
	{
		val=aux->pid;
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
Node<T>* LinkedList<T>::getFirst()
{
	return pFirst;
}

template <typename T>
T LinkedList<T>::decreaseDurata()
{
	pFirst->durata--;
	return pFirst->durata;
}

template <typename T>
T LinkedList<T>::increaseTime()
{
	pFirst->timp++;
	return pFirst->timp;
}

template <typename T>
T LinkedList<T>::modifyPrior()
{
	pFirst->prior--;
	return pFirst->prior;
}

template <typename T>
int  LinkedList<T>::isEmpty()
{
	if(pFirst==NULL || pLast==NULL)
	{
		return 1;
	}
	return 0;
}

template class LinkedList<int>;
