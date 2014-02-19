#include "scheduler.h"

Scheduler::Scheduler(int n, int nrproc, FILE *f, FILE *g)
{
	N=n;
	nrProc=nrproc;
	in=f;
	txt=g;
}

Scheduler::~Scheduler()
{
}

int Scheduler::test4number(char *a)
{
	int n=strlen(a);
	int count=0,i;
	for(i=0;i<n;i++)
	{
		if('0'<=a[i] && a[i]<='9') 	count++;
	}	
	return count;
}

void Scheduler::readNodeFile(Graph *g)	//citire vecini
{
	int k,i;
	char a[550],*p;

	for(i=0;i<N;i++)
	{
		fgets(a,550,in);
		if(test4number(a)!=0)
		{
			p=strtok(a," ");
			k=atoi(p);
			g->addEdge(i,k-1); 
			while(p!=NULL)
			{
				p=strtok(NULL," ");
				if(p==NULL) break;
				else
				{
					k=atoi(p);
					g->addEdge(i,k-1);
				}	
			}
		}	

		if(feof(in))
		{
			break;
		}
	}	
}

void Scheduler::readTimeFile(Graph *g)	//citire durata
{
	int k,i;
	char a[10];

	for(i=0;i<N;i++)
	{
		fgets(a,20,txt);
		k=atoi(a);
		g->addNodeDurata(k,i);
	}	
}

void Scheduler::PlanProcess(Graph *g)
{
	int *v,*proc,*plan,*typeProc,cont=0,i,j,T=0;

	proc=new int[nrProc];
	plan=new int[nrProc];
	typeProc=new int[nrProc];

	for(i=0;i<nrProc;i++)
	{
		proc[i]=-1;
	}

	while(g->isEmpty() != 1)
	{
		v=g->GetVectReady(nrProc);
		
		cont=0;
		for(i=0;i<nrProc;i++)
		{
			if(proc[i]==-1)
			{
				for(j=0;j<nrProc;j++)
				{
					if(v[j]!=-1 && g->getRunning(v[j])!=1)
					{
						proc[i]=v[j];	//planificare proces
						g->decreaseDurata(v[j]);	
						g->setRunning(v[j]);
						if(g->getDurata(v[j])==0) proc[i]=-1;
						plan[cont]=v[j]+1;	//stocare elemente output
						typeProc[cont]=i+1;
						cont++;
						break;
					}
				}	
			}
			else
			{
				g->decreaseDurata(proc[i]);
				if(g->getDurata(proc[i])==0) proc[i]=-1;
			}	
		}

		if(cont!=0)		//afisare
		{
			printf("T %d\n",T);
			for(i=0;i<cont;i++)
			{
				printf("%d %d\n",typeProc[i],plan[i]);
			}
		}
		delete[] v;
		T++;	
	}
	delete[] proc;
	delete[] plan;
	delete[] typeProc;
}