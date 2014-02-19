//Papa Florin, 334CA

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<vector>
#include<set>
using namespace std;

//intoarce indexul unui element dintr-un vector, -1 daca nu exista
int getIndex(vector<int> v, int elem) {
	for(int i = 0; i < v.size(); i++) {
		if(v[i] == elem) return i;
	}
	return -1;
}

//decide daca un DFA are o tranzitie data
int hasTransition(DFA *d, int from, char sym, int to) {
	for(int i = 0; i < d->from.size(); i++) {
		if(d->from[i] == from && d->symbol[i] == sym && d->to[i] == to) {
			return 1;
		}
	}
	return 0;
}

//intoarce concatenarea a 2 simboluri
string concatenate(string a, string b) {
	if(a[0] == 'O') {
		string s("O");
		return s;
	}
	else if(b[0] == 'O') {
		string s("O");
		return s;
	}
	else if(a[0] == 'e' && a.length() == 1) return b;
	else if(b[0] == 'e' && b.length() == 1) return a;
	
	else {
		string s;
		
		s += "(" + a + ")(" + b + ")";
				 
		return s;
	}
}

//intoarce rzultatul aplicarii Kleene Star pe un simbol
string star(string a) {
	string s;
	
	if(a[0] == 'O') {
		s += "e";
	}
	else {
		s += "(" + a + ")*";
	}
	
	return s;
}

//intoarce alternarea a 2 simboluri
string alter(string a, string b) {
	if(a[0] == 'O') return b;
	else if(b[0] == 'O') return a;
	else {
		string s("(" + a + ")");
		s += "|(" + b + ")";
		return s;
	}
}	

//algoritm de construire a expresiei regulate - Brzozowski
string buildER(DFA *d, vector<char> alpha) { 
	vector<string> B;
	vector< vector<string> > A;
	
	//initializari
	for(int i = 0; i < d->states.size(); i++) {
		if(getIndex(d->final, i) != -1) {
			string s("e");
			B.push_back(s);
		}
		else {
			string s("O");
			B.push_back(s);
		}
	}	
	
	for(int i = 0; i < d->states.size(); i++) {
		vector<string> v;
		for(int j = 0; j < d->states.size(); j++) {
			v.push_back("O");
			for(int k = 0; k < alpha.size(); k++) {
				if(hasTransition(d, i, alpha[k], j)) {
					string s;
					s.append(1, alpha[k]);
					v[j] = alter(v[j], s);
				}
			}
			
		}
		
		A.push_back(v);
	} 
	
	//construire ER
	for(int n = d->states.size() - 1; n >= 0; n--) {
		B[n] = concatenate(star(A[n][n]), B[n]);
		
		for(int j = 0; j < n; j++) {
			 A[n][j] = concatenate(star(A[n][n]), A[n][j]);
		}
		
		for(int i = 0; i < n; i++) {
			B[i] = alter(B[i], concatenate(A[i][n], B[n]));
			for(int j = 0; j < n; j++) {
				A[i][j] = alter(A[i][j], concatenate(A[i][n], A[n][j]));
			}
		}
		
		for(int i = 0; i < n; i++) {
      		A[i][n] = "e";
  		}
	}
	
	//daca starea initiala este si finala, se accepta sirul vid
	if(getIndex(d->final, d->initial) != -1) B[0] = alter(B[0], "e");
	
	return B[0];
}
