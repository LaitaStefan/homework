#include "heap.h"
#include<stdint.h>

template <typename T>
Heap<T>::Heap()
{
	capVect=256;
	dimVect=0;
}

template <typename T>
Heap<T>::~Heap()
{
}

template <typename T>
int Heap<T>::parent(int poz)
{
	int p;
	if(poz>dimVect || poz==0) { return -1; }
	p=(poz-1)/2;
	if(p>dimVect) { return -1; }
	return p;
}

template <typename T>
int Heap<T>::leftSubtree(int poz)
{
	int p;
	if(poz>dimVect) { return -1; }
	p = 2*poz + 1;
	if(p>dimVect) { return -1; }
	return p;
}
	
template <typename T>
int Heap<T>::rightSubtree(int poz)
{
	int p;
	if(poz>dimVect) { return -1; }
	p = 2*poz + 2;
	if(p>dimVect) { return -1; }
	return p;
}

template <typename T>
void Heap<T>::pushUp(int l)
{
	Node<T> aux;
	if(l>dimVect) { return; }
	if(parent(l)!=-1)
	{	
		if(nodes[l].frecv < nodes[parent(l)].frecv)
		{	
			aux=nodes[l];
			nodes[l]=nodes[parent(l)];
			nodes[parent(l)]=aux;
			pushUp(parent(l));
		}	
	}
}

template <typename T>
void Heap<T>::pushDown(int l)
{
	Node<T> aux;
	if(l>dimVect) { return; }
	if(leftSubtree(l)!=-1 && rightSubtree(l)!=-1)
	{	
		if(nodes[leftSubtree(l)].frecv <= nodes[l].frecv && nodes[leftSubtree(l)].frecv <= nodes[rightSubtree(l)].frecv)
		{
			aux=nodes[l];
			nodes[l]=nodes[leftSubtree(l)];
			nodes[leftSubtree(l)]=aux;
			pushDown(leftSubtree(l));
		}
		else
		{
			if(nodes[rightSubtree(l)].frecv <= nodes[l].frecv && nodes[rightSubtree(l)].frecv <= nodes[leftSubtree(l)].frecv)
			{
				aux=nodes[l];
				nodes[l]=nodes[rightSubtree(l)];
				nodes[rightSubtree(l)]=aux;
				pushDown(rightSubtree(l));
			}	
		}
	}	
	else
	{
		if(rightSubtree(l)!=-1)
		{
			if(nodes[rightSubtree(l)].frecv < nodes[l].frecv)
			{
				aux=nodes[l];
				nodes[l]=nodes[rightSubtree(l)];
				nodes[rightSubtree(l)]=aux;
				pushDown(rightSubtree(l));
			}
		}
		if(leftSubtree(l)!=-1)
		{
			if(nodes[leftSubtree(l)].frecv < nodes[l].frecv)
			{
				aux=nodes[l];
				nodes[l]=nodes[leftSubtree(l)];
				nodes[leftSubtree(l)]=aux;
				pushDown(rightSubtree(l));
			}
		}
	}	
}

template <typename T>
void Heap<T>::insertElement(T symbol, T frecv, T address)
{
	nodes[dimVect].symbol=symbol;
	nodes[dimVect].frecv=frecv;
	nodes[dimVect].address=address;
	dimVect++;
	pushUp(dimVect-1);
}

template <typename T>
Node<T> Heap<T>::peek()
{
	return nodes[0];
}

template <typename T>
Node<T> Heap<T>::extractMin()
{
	Node<T> aux;
	aux=nodes[0];
	nodes[0]=nodes[dimVect-1];
	dimVect--;
	pushDown(0);
	return aux;
}

template <typename T>
int Heap<T>::getDim()
{
	return dimVect;
}

template <typename T>
void Heap<T>::getHeap()
{
	int i;
	for(i=0;i<dimVect;i++)
	{
		printf("%d ",nodes[i].frecv);
	}	
	printf("\n");
}

/*template <typename T>
void Heap<T>::sortHeap()
{
	int i;
	int d=dimVect;
	for(i=0;i<d;i++)
	{
		printf("%d ",extractMin());
	}	
	printf("\n");
}*/

template class Heap<int>;	
template class Heap<uint8_t>;
		
		
		
		
		
