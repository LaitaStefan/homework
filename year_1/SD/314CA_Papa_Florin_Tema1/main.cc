//Papa Florin, 314CA

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "queue.h"
#include "queue1.h"

int t=0,tr=0,pid=1,prevpid=0,aux;

Queue<int> q[5],w[3];
Queue1<int> wait,sign;


void newproc(int timp,int durata,int prior)
{
	q[prior].enqueue(timp+1,durata,pid,prior);
	pid++;
}

void functionwait(int timp,int coada)
{
	wait.enqueue(timp+1,coada);
}

void functsignal(int timp,int coada)
{
	sign.enqueue(timp+1,coada);
}

int first()
{
	int aux1,i;
	aux1=-1;
	for(i=0;i<=4;i++)
	{
		if(q[i].isEmpty()!=1)
		{
			aux1=i;
			break;
		}
	}
	for(i=0;i<=4;i++)
	{
		if(q[i].isEmpty()!=1)
		{
			if(q[i].getTime()<q[aux1].getTime() && q[aux1].getTime()>t) { aux1=i; }
		}
	}
	return aux1;
}

void finalize()
{
	int p,coada,cont,a1,a2,a3;
	while(1)
	{
		//cazul in care exista semnale ce urmeaza a fi aplicate
		if(sign.isEmpty()!=1)
		{
			if(sign.getTime()==t)
			{
				coada=sign.getCoada();
				while(w[coada].isEmpty()!=1)															
				{
					if(w[coada].getPrior()>0) { p=w[coada].modifyPrior(); }
					else { p=w[coada].getPrior(); }
					q[p].addBetween(t,w[coada].getDurata(),w[coada].getPid(),p);
					w[coada].dequeue();
				}
				sign.dequeue();
			}
		}
		//cazul in care exista deja un proces care ruleaza
		if(tr>0 && tr<3)
		{	
			cont=0;
			if(wait.isEmpty()!=1)
			{
				if(wait.getTime()==t)
				{
					coada=wait.getCoada();
					w[coada].enqueue(q[aux].getTime(),q[aux].getDurata(),q[aux].getPid(),q[aux].getPrior());
					q[aux].dequeue();
					wait.dequeue();
					aux=first();
					tr=0;
					prevpid=0;
					cont++;							
					if(aux==-1)
					{
						if(sign.isEmpty()==1)	{ exit(0); } 
						else 
						{
							printf("[%d] NOP\n",t); 
							t++; 
							continue;
						}
					}
				}
			}
			//daca asupra procesului nu se aplica un semnal wait, continua executia
			if(cont==0)
			{
				if(q[aux].getDurata()==1)
				{
					printf("[%d] PID %d\n",t,q[aux].getPid());
					q[aux].dequeue();
					tr=0;
					prevpid=0;
				}
				else
				{
					printf("[%d] PID %d\n",t,prevpid);
					tr++;
					q[aux].decreaseDurata();	//decrementarea duratei ramase de rulare a procesului
					q[aux].increaseTime(); //cresterea timpului aferent procesului
				}
			}
			//daca asupra procesului curent s-a aplicat un semnal wait, cautam un alt proces care sa ruleze
			else
			{
				if(q[aux].getTime()<=t)
				{
					if(q[aux].getDurata()==1)
					{
						printf("[%d] PID %d\n",t,q[aux].getPid());
						q[aux].dequeue();
						tr=0;
						prevpid=0;
					}
					else
					{
						printf("[%d] PID %d\n",t,q[aux].getPid());
						tr++;
						prevpid=q[aux].getPid();
						q[aux].decreaseDurata();
						q[aux].increaseTime();
					}
				}
				//daca nu exista niciun proces care sa aiba timpul adecvat la care ar trebui sa inceapa rularea, se marcheaza cuanta de timp cu NOP
				else
				{
					printf("[%d] NOP\n",t);
					tr=0;	
					prevpid=0;
				}
			}
		}
		//cazul in care nu exista un proces care ruleaza(sau daca exista, trebuie oprit pentru ca a atins limita de timp) si trebuie sa cautam unul nou
		else
		{	
			if(tr==3)
			{
				tr=0;
				prevpid=0;
				a1=q[aux].getDurata();
				a2=q[aux].getPid();			//parametrii procesului trebuiesc salvati deoarece daca am face addBetween la un proces si acesta ar fi pus 
				a3=q[aux].getPrior();		//la inceputul cozii, la pasul urmator i-am face chiar acelui proces dequeue
				q[aux].dequeue();
				q[aux].addBetween(t,a1,a2,a3);	//addBetween adauga procesul intr-un loc corespunzator in coada in functie de timpul la care trebuie sa
			}									//ruleze				
			aux=first();
			if(aux!=-1)
			{
				if(q[aux].getTime()<=t)
				{
					cont=0;
					if(wait.isEmpty()!=1)
					{
						if(wait.getTime()==t)
						{
							coada=wait.getCoada();
							w[coada].enqueue(q[aux].getTime(),q[aux].getDurata(),q[aux].getPid(),q[aux].getPrior());
							q[aux].dequeue();
							wait.dequeue();
							aux=first();
							cont++;
							if(aux==-1)
							{
								if(sign.isEmpty()==1)	{ exit(0); } 
								else { printf("[%d] NOP\n",t); }	
							}
						}
					}
					if(cont==0)
					{	
						printf("[%d] PID %d\n",t,q[aux].getPid());
						if(q[aux].getDurata()==1)
						{
							tr=0;
							prevpid=0;
							q[aux].dequeue();						
						}
						else
						{
							tr++;
							prevpid=q[aux].getPid();
							q[aux].decreaseDurata();
							q[aux].increaseTime();
						}
					}		
					else
					{
						if(q[aux].getTime()<=t)
						{
							printf("[%d] PID %d\n",t,q[aux].getPid());
							if(q[aux].getDurata()==1)
							{
								tr=0;
								prevpid=0;
								q[aux].dequeue();							
							}
							else
							{
								tr++;
								prevpid=q[aux].getPid();
								q[aux].decreaseDurata();
								q[aux].increaseTime();
							}
						}
					}
				}
				else
				{
					printf("[%d] NOP\n",t);
				}
			}
			else 
			{ 
				if(sign.isEmpty()==1)	{ exit(0); } 
				else { printf("[%d] NOP\n",t); } 
			}
		}
		t++;
	}
}

int main()
{
	char *s,*p,newp[10]="NEW",fin[10]="FINALIZE",wait[10]="WAIT",signal[10]="SIGNAL";
	int n1,n2,n3;
	s=(char*)malloc(100*sizeof(char));
	while (1)
	{
		gets(s);
		p=strtok(s," ,.-");
		if(strcmp(p,newp)==0)
		{
			p=strtok(NULL," .,-");
			n1=atoi(p);
			p=strtok(NULL," .,-");
			n2=atoi(p);
			p=strtok(NULL," .,-");
			n3=atoi(p);
			newproc(n1,n2,n3); 
		}
		if(strcmp(p,fin)==0) { finalize(); }
		if(strcmp(p,wait)==0) 
		{ 
			p=strtok(NULL," .,-");
			n1=atoi(p);
			p=strtok(NULL," .,-");
			n2=atoi(p);
			functionwait(n1,n2);
		}
		if(strcmp(p,signal)==0) 
		{
			p=strtok(NULL," .,-");
			n1=atoi(p);
			p=strtok(NULL," .,-");
			n2=atoi(p); 
			functsignal(n1,n2);
		}
	}
	return 0;
}
