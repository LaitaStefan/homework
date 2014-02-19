#include<stdio.h>
#include<stdlib.h>

class Graph{
private:
		int **A;
		int N;
		int *durata;
		int *running;

public:
		Graph(int n);
		~Graph();

		void addEdge(int i,int j);

		void addNodeDurata(int durata,int ind);

		void decreaseDurata(int ind);

		void removeNode(int ind);

		int* GetVectReady(int n);

		int getDurata(int ind);

		void setRunning(int ind);

		int getRunning(int ind);

		int isEmpty();
};




