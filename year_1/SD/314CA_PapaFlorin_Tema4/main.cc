#include "scheduler.h"

int main(int argc,char **argv)
{
	FILE *f,*g;
	char a[10];
	int n,nrProc;

	f=fopen(argv[1],"r");
	g=fopen(argv[2],"r");

	if(f==NULL || g==NULL) 
	{
		printf("Eroare citire fisier input\n");
		return 0;
	}	
	fgets(a,20,f);	//citire numar de noduri
	n=atoi(a);

	fgets(a,20,g);	//citire numar de procesoare
	nrProc=atoi(a);

	Graph graf(n);

	Scheduler sch(n,nrProc,f,g);

	sch.readNodeFile(&graf);	

	sch.readTimeFile(&graf);

	sch.PlanProcess(&graf);

	fclose(f);
	fclose(g);

	return 0;
}