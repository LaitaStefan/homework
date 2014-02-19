//Papa Florin, 314CA

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<iostream>
#include<fstream>
#include<string>
#define MAX 16

using namespace std;

struct Node
{
	int height;
	int frecv;
	char s[35];
	Node **link;
};

class SkipList{
	
private:
	Node *pFirst;
	int dim;
public:
	SkipList();
	~SkipList();
	void insert(char *s);
	Node* search(char *s);
	void sterg(char*s);

	void decreaseFrecv(char *s,int n);
	int getFrecv(char *s);
	void setFrecv(int n,char *s);	
	Node** getData();
	int getSize();
};