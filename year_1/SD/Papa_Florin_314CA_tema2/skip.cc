//Papa Florin, 314CA

#include "skip.h"

SkipList::SkipList()
{
	int i;
	Node *nod=new Node;
	pFirst=nod;
	nod->link=(Node**)malloc(MAX*sizeof(Node*));
	for(i=0;i<MAX;i++)
	{
		nod->link[i]=NULL;
	}	
	dim=0;
}

SkipList::~SkipList()
{
	Node *x=pFirst,*y;
	while(x!=NULL)
	{
		y=x;
		x=x->link[0];
		delete[] y->link;
		delete y;
	}	
}

Node* SkipList::search(char *string)
{
	Node *x=pFirst;
	int grad=MAX-1;

	while(grad>=0)
	{
		while((x->link[grad]!=NULL) && strcmp(x->link[grad]->s,string)<0 )
		{
			x=x->link[grad];
		}	
		grad--;
	}	
	if(x->link[0]!=NULL)
	{
		if(strcmp(x->link[0]->s,string)==0) { return x; }
	}	
	return NULL;
}

void SkipList::insert(char *string)
{
	Node *nod=new Node;
	Node *x=pFirst;
	Node *temp[MAX];
	int grad=MAX-1,i,h=1;

	for(i=0;i<MAX;i++)
	{
		temp[i]=pFirst;
	}	

	while(grad>=0)
	{
		while((x->link[grad]!=NULL) && strcmp(x->link[grad]->s,string)<0 )
		{
			x=x->link[grad];
			for(i=0;i<=grad;i++)
			{
				temp[i]=x;
			}	
		}	
		grad--;
	}	

	if(x->link[0]!=NULL)
	{
		if(strcmp(x->link[0]->s,string)==0) 
		{ 
			x->link[0]->frecv++;
			delete[] nod;
			return; 
		}
	}	
	while(h<MAX && rand()%2==0)
	{
		h++;
	}
	dim++;	
	nod->height=h;
	nod->link=(Node**)malloc(h*sizeof(Node*));
	for(i=0;i<h;i++)
	{
		nod->link[i]=temp[i]->link[i];
		temp[i]->link[i]=nod;
	}	
	strcpy(nod->s,string);
	nod->frecv=1;
}

void SkipList::sterg(char *string)
{
	Node *x=pFirst;
	Node *temp[MAX];
	int grad=MAX-1,i,d;

	for(i=0;i<MAX;i++)
	{
		temp[i]=pFirst;
	}	
	while(grad>=0)
	{
		while((x->link[grad]!=NULL) && strcmp(x->link[grad]->s,string)<0 )
		{
			x=x->link[grad];
			for(i=0;i<=grad;i++)
			{
				temp[i]=x;
			}	
		}	
		grad--;
	}	
	
	if(x->link[0]!=NULL)
	{
		if(strcmp(x->link[0]->s,string)==0) 
		{ 
			d=x->link[0]->height;
			x=temp[0]->link[0];
			for(i=0;i<d;i++)
			{
				temp[i]->link[i]=temp[i]->link[i]->link[i];
			}	
			delete[] x->link;
			delete[] x;
			dim--;
			return; 
		}
	}	
}

int SkipList::getFrecv(char *string)
{
	Node *x;
	x=search(string);
	if(x!=NULL)
	{
		return x->link[0]->frecv; 
	}
	return 0;
}

void SkipList::decreaseFrecv(char *string,int n)
{
	Node *x;
	x=search(string);
	
	if(x!=NULL)
	{
		if(x->link[0]->frecv<=n) { sterg(string); }
		else { x->link[0]->frecv-=n; }	
	}	
}

Node** SkipList::getData()
{
	Node **x,*y=pFirst;
	int i;
	x=new Node*[dim];
	for(i=1;i<=dim;i++)
	{
		x[i-1]=y->link[0];
		y=y->link[0];
	}	
	return x;
}

void SkipList::setFrecv(int n, char *string)
{
	Node *x=search(string);
	if(strcmp(x->link[0]->s,string)==0)
	{
		x->link[0]->frecv=n;
	}
}

int SkipList::getSize()
{
	return dim;
}