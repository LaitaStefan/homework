//Papa Florin, 334CA

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<vector>
#include<set>
#include<stack>
using namespace std;

class DFA_Official {
public:
	vector<string> states;
	vector<string> from; //tranzitie de la
	vector<string> symbol; //tranzitie prin
	vector<string> to; //tranzitie la
	vector<string> final;
	string initial; //stari initial si final

public:
	DFA_Official() {}
};

class DFA {
public:
	vector< set<int> > states;
	vector<int> from; //tranzitie de la
	vector<char> symbol; //tranzitie prin
	vector<int> to; //tranzitie la
	vector<int> final;
	int initial; //stari initial si final

public:
	DFA() {}
};

//convertesc DFA_Official la DFA
DFA* official_to_intern(DFA_Official *off) {
	DFA *d = new DFA();

	//stari
	for(int i = 0; i < off->states.size(); i++) {
		set<int> s;
		d->states.push_back(s);
	}
	
	for(int i = 0; i < off->from.size(); i++) {
		int from = indexOf(off->states, off->from[i]);
		int to = indexOf(off->states, off->to[i]);
		d->from.push_back(from);
		d->symbol.push_back(off->symbol[i][0]);
		d->to.push_back(to);
	}
	
	for(int i = 0; i < off->final.size(); i++) {
		d->final.push_back(indexOf(off->states, off->final[i]));
	}
	
	d->initial = indexOf(off->states, off->initial);

	return d;
}

//starile in care se poate ajunge prin tranzitii pe "e"
set<int> eps_closure(set<int> states, NFA *n) {
	
	set<int> closure = states;
	stack<int> st;
	set<int>::iterator it;
	
	for(it = states.begin(); it != states.end(); it++) {
		st.push(*it);
	}
	  
	while(!st.empty()) {
		int t = st.top();
		st.pop();
		for(int i = 0; i < n->from.size(); i++) {
			string s("e");
			it = closure.find(n->to[i]);
			if(t == n->from[i] && n->symbol[i].compare(s) == 0 && it == closure.end()) {
				closure.insert(n->to[i]);
				st.push(n->to[i]);
			}
		}
	}
	
	return closure;
} 

//intoarce o stare nemarcata(pentru subset construct), -1 daca nu exista
int has_unmarked(DFA *d, vector<int> v) {
	for(int i = 0; i < d->states.size(); i++) {
		if(v[i] == 0) return i;
	}
	return -1;
} 

//decide daca un NFA are o anumita stare finala
int has_final_state(NFA *n, int val) {
	for(int i = 0; i < n->final.size(); i++) {
		if(n->final[i] == val) return 1;
	}
	return 0;
}

//decide daca un DFA are o anumita stare finala
int has_final_state(DFA *d, int val) {
	for(int i = 0; i < d->final.size(); i++) {
		if(d->final[i] == val) return 1;
	}
	return 0;
}

//intoarce indexul unui element dintr-un vector
int indexOf(vector<set<int> > v, set<int> s) {
	for(int i = 0; i < v.size(); i++) {
		if(v[i] == s) return i;
	}
	return -1;
}

//intoarce indexul unui element dintr-un vector
int getIndex1(vector<int> v, int elem) {
	for(int i = 0; i < v.size(); i++) {
		if(v[i] == elem) return i;
	}
	return -1;
}

//intoarce un set de stari in care se ajunge pe o anumita tranzitie
set<int> move(NFA *n, set<int> s, char c) {
	set<int>::iterator it;
	set<int> rez;
	for(it = s.begin(); it != s.end(); it++) {
		for(int j = 0; j < n->from.size(); j++) {
			if(n->from[j] == *it){
				string s;
				s.append(1, c);
				if(n->symbol[j].compare(s) == 0) rez.insert(n->to[j]);
			}
		}
	}
	
	return rez;
}

//construieste DFA din NFA - algoritmul subset construct
DFA* build_DFA(NFA *n, vector<char> alphabet) {
	DFA *d = new DFA();
	vector<int> marked;
	set<int>::iterator it;
	
	set<int> s;
	s.insert(n->initial);
	
	d->states.push_back(eps_closure(s, n));
	marked.push_back(0);
	d->initial = 0;
	
	while(has_unmarked(d, marked) >= 0) {
		int index = has_unmarked(d, marked); 
		set<int> t = d->states[index];
		marked[index] = 1;
		
		for(it = t.begin(); it != t.end(); it++) {
			if(has_final_state(n, *it) && !has_final_state(d, index)) {
				d->final.push_back(index);
				break;
			}
		}
		
		for(int i = 0; i < alphabet.size(); i++) {
			set<int> moved = move(n, t, alphabet[i]);
			set<int> u = eps_closure(moved, n);
			
			if(indexOf(d->states, u) == -1) {
				d->states.push_back(u);
				marked.push_back(0);
			}
			
			d->from.push_back(index);
			d->symbol.push_back(alphabet[i]);
			d->to.push_back(indexOf(d->states, u));
		}
	}
	
	return d;
}

//valideaza o intrare
int isValid(char* a, DFA *d) {
	int stare = d->initial;
	int l = strlen(a);
	
	if(strcmp(a, "e") == 0 && getIndex1(d->final, d->initial) != -1) return 1;
	
	for(int i = 0; i < l; i++) {
		int found = 0;

		for(int j = 0; j < d->from.size(); j++) {
			if(d->from[j] == stare && d->symbol[j] == a[i]) {
				//trec in starea urmatoare
				stare = d->to[j];
				found = 1;
				break;
			}
		}
		
		if(found == 0) return 0;
	}
	
	for(int i = 0; i < d->final.size(); i++) {
		if(d->final[i] == stare) return 1;
	}
	
	return 0;
} 
 
//afiseaza DFA in format string  
void print_DFA_Official(DFA_Official *n) {
	printf("States: ");
	for(int i = 0; i < n->states.size(); i++) {
		cout<< n->states[i] << " ";
	}
	printf("\n");
	
	for(int i = 0; i < n->from.size(); i++) {
		cout<< "from " << n->from[i] << " via ";
		cout << n->symbol[i];
		cout << " to " << n->to[i] << "\n";
	}
	
	cout << "Initial " << n->initial << "\n";
	
	printf("Final ");
	for(int i = 0; i < n->final.size(); i++) {
		cout << n->final[i] << " ";
	}
	printf("\n");
}

//afiseaza DFA in format numeric
void print_DFA(DFA *n, vector<char> alphabet) {
	//print states
	cout << "({";
	for(int i = 0; i < n->states.size(); i++) {
		if(i > 0) cout << ",";
		cout << "S" << i;
	}
	cout << "},{";
	
	//print alphabet
	for(int i = 0; i < alphabet.size(); i++) {
		if(i > 0) cout << ",";
		cout << alphabet[i];
	}
	cout << "},";
	
	//print transitions	
	cout << "{";
	for(int i = 0; i < n->from.size(); i++) {
		if(i > 0) cout << ",";
		cout << "d(S" << n->from[i] << ",";
		cout << n->symbol[i] << ")=";
		cout << "S" << n->to[i];
	}
	cout << "},";
	
	//print initial
	cout << "S" << n->initial << ",";
	
	//print final
	cout << "{";
	for(int i = 0; i < n->final.size(); i++) {
		if(i > 0) cout << ",";
		cout << "S" << n->final[i];
	}
	cout <<  "})\n";
}
