#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "graph.h"

class Scheduler {
private:
	FILE *in,*txt;
	int N,nrProc;
public:

	Scheduler(int n, int nrproc, FILE *f, FILE *g);

	~Scheduler();

	int test4number(char *a);

	void readNodeFile(Graph *g);

	void readTimeFile(Graph *g);

	void PlanProcess(Graph *g);	
};