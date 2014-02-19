//Papa Florin, 314CA

#include "skip.h"
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

using namespace std;

#define pas 10

int ind=0,dim=1; 		//dim retine cu cat trebuie realocata memorie pentru structura cheie, iar ind retine dimensiunea vectorului de "struct cheie"
struct cheie
{
	char str[20];
};

int getInd(char *s,struct cheie *key)
{
	int i;
	for(i=0;i<ind;i++)
	{
		if(strcmp(s,key[i].str)==0) { return i; }	 //returneaza indicele la care se afla un anumit tag. In cazul in care nu exista, returneaza -1
	}	
	return -1;
}

void turn2char(string a, char c[])
{
	int i,n=a.size();
	for(i=0;i<n;i++)
	{
		c[i]=a[i];				//transforma o variabila de tip string in char
	}	
	c[n]='\0';
}

int main(int argc,char **argv)
{
	SkipList *sl;
	sl=new SkipList [40];
	struct cheie *key;
	string a;
	Node **x;
	char *cuvant,*cheie,**token_cautat,**parte_vorbire,**num,numar[10],b[20],d[20],*c,**comanda;
	int N,i,j,nr,n,k;
	key=(struct cheie*)malloc(pas*sizeof(struct cheie));

	ifstream f;
	f.open(argv[1]);

	ifstream g;
	getline(f,a);
	turn2char(a,b);		//citire nume fisier corpus
	g.open(b);

	ofstream out;
	getline(f,a);
	turn2char(a,d);		//citire nume fisier output
	out.open(d);

	getline(f,a); 		//citire numar comenzi
	turn2char(a,numar);
	N=atoi(numar);
	
	//parsare text
	while(g.good())
	{
		getline(g,a);
		n=a.size();
		c=new char[n+1];
		turn2char(a,c);
		
		cuvant=strtok(c,"/");
		n=strlen(cuvant);
		for(i=0;i<n;i++)
		{
			if(isupper(cuvant[i])) { cuvant[i]=tolower(cuvant[i]); }		//verific daca primul cuvant de pe rand contine litere mari si le 
		}																	//transform in litere mici
		cheie=strtok(NULL," ");

		while(cheie!=NULL)
		{
				
			if(strlen(cheie)>=2 && isalpha(cheie[0]))
			{
				i=getInd(cheie,key);
				if(i==-1)
				{
					if(ind%10==0 && ind!=0)
					{
						dim++;
						key=(struct cheie*)realloc(key,pas*dim*sizeof(struct cheie));
					}	
					strcpy(key[ind].str,cheie);
					sl[ind].insert(cuvant);
					ind++;
				}	
				else
				{
					sl[i].insert(cuvant);
				}
			}	
			cuvant=strtok(NULL,"/");
			cheie=strtok(NULL," ");
		}	
		delete[] c;
	}	

	//interpretare comenzi si producere output
	if(N>0)
	{	
		comanda=(char**)malloc(N*sizeof(char*));
		token_cautat=(char**)malloc(N*sizeof(char*));
		parte_vorbire=(char**)malloc(N*sizeof(char*));
		num=(char**)malloc(N*sizeof(char*));

		for(i=0;i<N;i++)
		{
			f>>a;
			n=a.size();
			comanda[i]=(char*)malloc((n+1)*sizeof(char));
			turn2char(a,comanda[i]);
			if(strcmp(comanda[i],"-s")==0)
			{
				f>>a;
				n=a.size();
				token_cautat[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,token_cautat[i]);

				f>>a;
				n=a.size();
				parte_vorbire[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,parte_vorbire[i]);

				j=getInd(parte_vorbire[i],key);

				if(j==-1) { out<<"Tokenul cautat nu exista\n\n"; }
				else
				{
					if(sl[j].getFrecv(token_cautat[i])==0) { out<<"Tokenul cautat nu exista\n\n"; }
					else 
					{ 
						out<<"CAUTARE"<<endl;
						out<<"*PARTE_DE_VORBIRE : "<<parte_vorbire[i]<<"\n";
						out<<"**TOKEN : "<<token_cautat[i]<<endl;
						out<<"***FRECVENTA : "<<sl[j].getFrecv(token_cautat[i])<<endl<<endl;
					}
				}
			}
			if(strcmp(comanda[i],"-d")==0)
			{
				f>>a;
				n=a.size();
				token_cautat[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,token_cautat[i]);
			

				f>>a;
				n=a.size();
				parte_vorbire[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,parte_vorbire[i]);

				f>>a;
				n=a.size();
				num[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,num[i]);

				nr=atoi(num[i]);
				j=getInd(parte_vorbire[i],key);
				if(j!=-1) { sl[j].decreaseFrecv(token_cautat[i],nr); }
			}	
			if(strcmp(comanda[i],"-p")==0)
			{
				f>>a;
				n=a.size();
				parte_vorbire[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,parte_vorbire[i]);

				j=getInd(parte_vorbire[i],key);
				out<<"AFISARE"<<endl;
				if(j!=-1 && sl[j].getSize()>0) 
				{ 
					out<<"*PARTE_DE_VORBIRE : "<<parte_vorbire[i]<<endl;
				
					x=sl[j].getData();		
			
					k=0;
					while(1)
					{
						out<<"**(TOKEN : "<<x[k]->s<<", FRECVENTA : "<<x[k]->frecv<<")"<<endl;
						if(x[k]->link[0]==NULL) { break; }
						k++;
					} 
					delete[] x;
					out<<endl;
				}
				else
				{
					out<<"*PARTE_DE_VORBIRE : "<<parte_vorbire[i]<<endl;
					out<<"**UNKNOWN\n\n";
				}	
			}
			if(strcmp(comanda[i],"-a")==0)
			{
				f>>a;
				n=a.size();
				token_cautat[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,token_cautat[i]);
				f>>a;
				n=a.size();
				parte_vorbire[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,parte_vorbire[i]);
				f>>a;
				n=a.size();
				num[i]=(char*)malloc((n+1)*sizeof(char));
				turn2char(a,num[i]);
				nr=atoi(num[i]);
				j=getInd(parte_vorbire[i],key);
				if(nr>=0)
				{	
					if(j==-1)
					{
						if(nr>0)
						{	
							if( strlen(parte_vorbire[i])>=2 && isalpha(parte_vorbire[i][0]) )
							{	
								if(ind%10==0 && ind!=0)
								{
									dim++;
									key=(struct cheie*)realloc(key,pas*dim*sizeof(struct cheie));
								}	
								strcpy(key[ind].str,parte_vorbire[i]);
								sl[ind].insert(token_cautat[i]);
								sl[ind].setFrecv(nr,token_cautat[i]);
								ind++;
							}	
						}	
					}	
					else 
					{
						if(sl[j].search(token_cautat[i])==NULL)
						{
							if(n>0)
							{	
								sl[j].insert(token_cautat[i]);
								sl[j].setFrecv(nr,token_cautat[i]);
							}	
						}	
						else { sl[j].setFrecv(nr+sl[j].getFrecv(token_cautat[i]),token_cautat[i]);}
					}
				}
			}	
		}	

		delete[] parte_vorbire;
		delete[] token_cautat;
		delete[] num;
	}	
	f.close();
	g.close();
	out.close();
	delete[] key;	
	delete[] sl;
	return 0;
}