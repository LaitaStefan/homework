#include "graph.h"

Graph::Graph(int n)
{
	int i;

	N=n;

	durata=new int[N];
	A=new int*[N];
	running=new int[N];

	for(i=0;i<N;i++)
	{
		A[i]=new int[N];
	}	
}

Graph::~Graph()
{
	int i;
	
	for(i=0;i<N;i++)
	{	
		delete[] A[i];
	}

	delete[] durata;
	delete[] A;	
	delete[] running;
}

void Graph::addEdge(int i,int j)
{
	if(i!=j) A[i][j]=1;
}

void Graph::addNodeDurata(int dur,int ind)
{
	durata[ind]=dur;
}

void Graph::removeNode(int ind)
{
	int i;
	for(i=0;i<N;i++)
	{
		A[ind][i]=0;		//nodul nu mai pointeaza catre nimeni
	}	
	A[ind][ind]=-1;			//marcare ca si inactiv
	durata[ind]=0;
}

void Graph::decreaseDurata(int ind)
{
	durata[ind]--;
	if(durata[ind]==0)
	{
		removeNode(ind);
	}	
}

int* Graph::GetVectReady(int n)
{
	int *v,cont,ind=0,i,j;
	v=new int[n];
	for(i=0;i<n;i++)
	{
		v[i]=-1; //initializare ca empty
	}	

	for(i=0;i<N;i++)
	{
		cont=0;
		if(A[i][i]==-1 || running[i]==1) continue;
		for(j=0;j<N;j++)
		{
			if(A[j][i]!=0) cont++;
		}
		if(cont==0)
		{
			v[ind]=i;
			ind++;
			if(ind==n) break;
		}	
	}	
	return v;
}

int Graph::isEmpty()
{
	int i;
	for(i=0;i<N;i++)
	{
		if(A[i][i]!=-1) return 0;
	}	
	return 1;
}

int Graph::getDurata(int ind)
{
	return durata[ind];
}

void Graph::setRunning(int ind)
{
	running[ind]=1;
}

int Graph::getRunning(int ind)
{
	return running[ind];
}
