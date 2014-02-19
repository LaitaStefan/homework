//Papa Florin, 334CA

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<vector>
using namespace std;

int count = 0;

typedef enum {CHR, STAR, QUESTION, ALTER, CONCAT, PLUS} node_type;

class tree_node {
public:
	char val;
	node_type type;
	tree_node *left;
	tree_node *right;
public:
	tree_node(node_type t, char v, tree_node *l, tree_node *r) {
		val = v;
		type = t;
		left = l;
		right = r;
	}	
};

class NFA_Official {
public:
	vector<string> states;
	vector<string> from; //tranzitie de la
	vector<string> symbol; //tranzitie prin
	vector<string> to; //tranzitie la
	vector<string> final;
	string initial; //stari initial si final

public:
	NFA_Official() {}
};

class NFA {
public:
	vector<int> states;
	vector<int> from; //tranzitie de la
	vector<string> symbol; //tranzitie prin
	vector<int> to; //tranzitie la
	vector<int> final;
	int initial; //stari initial si final

public:
	NFA() {}
};

//intoarce indexul unui element dintr-un vector, -1 daca nu exista
int indexOf(vector<string> v, string elem) {
	for(int i = 0; i < v.size(); i++) {
		if(v[i].compare(elem) == 0) return i;
	}
	return -1;
}

//creez NFA intern din NFA in format string, expandand tranzitiile pe mai mult
//de un caracter
NFA* official_to_intern(NFA_Official *n) {
	NFA *result = new NFA();
	
	for(int i = 0; i < n->states.size(); i++) {
		result->states.push_back(i);
	}
	
	for(int i = 0; i < n->from.size(); i++) {
		int from = indexOf(n->states, n->from[i]);
		int to = indexOf(n->states, n->to[i]);
		result->from.push_back(from);
		result->symbol.push_back(n->symbol[i]);
		result->to.push_back(to);
	}
	
	for(int i = 0; i < n->final.size(); i++) {
		result->final.push_back(indexOf(n->states, n->final[i]));
	}
	
	result->initial = indexOf(n->states, n->initial);	
	
	//eliminare tranzitii pe stringuri
	int state_nr = result->states.size();
	
	for(int i = 0; i < result->symbol.size(); i++) {
		string aux(result->symbol[i]);
		
		if(aux.size() > 1) {
			int old_to = result->to[i];
			
			
			string c(aux.substr(0,1));
			result->symbol[i] = c;
			result->to[i] = state_nr;
			state_nr++;
			
			
			//introduc o noua tranzitie
			string s(aux.substr(1, aux.length() - 1));
			result->from.push_back(result->to[i]);
			result->symbol.push_back(s);
			result->to.push_back(old_to);
		}
	}
			 
	return result;
}  

//copiaza un vector in alt vector
void copy_states(vector<int> &dest, vector<int> &source) {
	for(int i = 0; i < source.size(); i++) {
		dest.push_back(source[i]);
	}
}

void copy_symbols(vector<string> &dest, vector<string> &source) {
	for(int i = 0; i < source.size(); i++) {
		dest.push_back(source[i]);
		//cout << dest[dest.size() - 1];
	}
}

//inlocuieste valoarea de la un anumit index cu o valoare data
void replace(vector<int> &v, vector<int> index, int newVal) {
	for(int i = 0; i < index.size(); i++) {
		v[index[i]] = newVal;
	}
}

//intoarce indexul unui element dintr-un vector, -1 daca nu exista
vector<int> indexOf(vector<int> v, int elem) {
	vector<int> index;
	for(int i = 0; i < v.size(); i++) {
		if(v[i] == elem) index.push_back(i);
	}
	return index;
}	

//construieste NFA dintr-un simbol
NFA* build_simple_NFA(char c) {
	NFA* my_nfa = new NFA();
	
	my_nfa->initial = count;
	my_nfa->states.push_back(count);
	count++;
	
	my_nfa->final.push_back(count);
	my_nfa->states.push_back(count);
	count++;
	
	if(c != 'O') {
		my_nfa->from.push_back(count - 2);
		string s;
		s.append(1, c);
		my_nfa->symbol.push_back(s);
		my_nfa->to.push_back(count - 1);
	}
	
	return my_nfa;
}

//construieste NFA alternand 2 NFA-uri
NFA* build_alter_NFA(NFA *left, NFA *right) {

	int old_left_initial = left->initial;
	int old_left_final = left->final[0];

	left->initial = count;
	count++;
	left->final[0] = count;
	count++;

	//new initial and final states
	left->states.push_back(left->initial);
	left->states.push_back(left->final[0]);

	//new transitions to left NFA
	left->from.push_back(left->initial);
	left->symbol.push_back("e");
	left->to.push_back(old_left_initial);

	left->from.push_back(old_left_final);
	left->symbol.push_back("e");
	left->to.push_back(left->final[0]);

	//copy states from right NFA to left NFA
	copy_states(left->states, right->states);
	copy_states(left->from, right->from);
	copy_symbols(left->symbol, right->symbol);
	copy_states(left->to, right->to);

	//new transitions to right NFA
	left->from.push_back(left->initial);
	left->symbol.push_back("e");
	left->to.push_back(right->initial);

	left->from.push_back(right->final[0]);
	left->symbol.push_back("e");
	left->to.push_back(left->final[0]);
	
	return left;
}

//construieste NFA concatenand 2 NFA-uri
NFA* build_concat_NFA(NFA *left, NFA *right) {
	 
	 vector<int> index = indexOf(right->from, right->initial);
	 
	 replace(right->from, index, left->final[0]);
	 
	 index = indexOf(right->states, right->initial);
	 right->states.erase(right->states.begin() + index[0]);
	 
	 copy_states(left->states, right->states);
	 copy_states(left->from, right->from);
	 copy_symbols(left->symbol, right->symbol);
	 copy_states(left->to, right->to);
	 
	 //modifica stare finala
	 left->final[0] = right->final[0];
	 
	 return left;
}

//construieste NFA aplicand Kleene Star pe un NFA dat
NFA* build_star_NFA(NFA *n) {
	int new_initial = count;
	count++;
	int new_final = count;
	count++;
	
	n->states.push_back(new_initial);
	n->states.push_back(new_final);
	
	//from initial to final
	n->from.push_back(new_initial);
	n->symbol.push_back("e");
	n->to.push_back(new_final);
	
	//from old final to old initial
	n->from.push_back(n->final[0]);
	n->symbol.push_back("e");
	n->to.push_back(n->initial);
	
	//from new initial to old initial
	n->from.push_back(new_initial);
	n->symbol.push_back("e");
	n->to.push_back(n->initial);
	
	//from old final to new final
	n->from.push_back(n->final[0]);
	n->symbol.push_back("e");
	n->to.push_back(new_final);
	
	//new initial & new final
	n->initial = new_initial;
	n->final[0] = new_final;
	
	return n;
}

//construieste NFA aplicand "+" pe un NFA dat
NFA* build_plus_NFA(NFA *n) {
	int new_initial = count;
	count++;
	int new_final = count;
	count++;
	
	n->states.push_back(new_initial);
	n->states.push_back(new_final);
	
	//from old final to old initial
	n->from.push_back(n->final[0]);
	n->symbol.push_back("e");
	n->to.push_back(n->initial);
	
	//from new initial to old initial
	n->from.push_back(new_initial);
	n->symbol.push_back("e");
	n->to.push_back(n->initial);
	
	//from old final to new final
	n->from.push_back(n->final[0]);
	n->symbol.push_back("e");
	n->to.push_back(new_final);
	
	//new initial & new final
	n->initial = new_initial;
	n->final[0] = new_final;
	
	return n;
}

//constuieste NFA aplicand "?" pe un NFA dat
NFA* build_question_NFA(NFA *n) {
	int new_initial = count;
	count++;
	int new_final = count;
	count++;
	
	n->states.push_back(new_initial);
	n->states.push_back(new_final);
	
	//from initial to final
	n->from.push_back(new_initial);
	n->symbol.push_back("e");
	n->to.push_back(new_final);
	
	//from new initial to old initial
	n->from.push_back(new_initial);
	n->symbol.push_back("e");
	n->to.push_back(n->initial);
	
	//from old final to new final
	n->from.push_back(n->final[0]);
	n->symbol.push_back("e");
	n->to.push_back(new_final);
	
	//new initial & new final
	n->initial = new_initial;
	n->final[0] = new_final;
	
	return n;
}

//construieste NFA din arbore de parsare
NFA* build_NFA(tree_node* n) {
	
	if(n->type == CHR) {
 		return build_simple_NFA(n->val);
	}	
	
	if(n->type == ALTER) {
		return build_alter_NFA(build_NFA(n->left), build_NFA(n->right));
	}
	
	if(n->type == CONCAT) {
		return build_concat_NFA(build_NFA(n->left), build_NFA(n->right));
	}
	
	if(n->type == STAR) {
		return build_star_NFA(build_NFA(n->left));
	}
	
	if(n->type == PLUS) {
		return build_plus_NFA(build_NFA(n->left));
	}
	
	if(n->type == QUESTION) {
		return build_question_NFA(build_NFA(n->left));
	}
	
	return NULL;
}

//afiseaza NFA in format numeric
void print_NFA(NFA *n, vector<char> alphabet) {
	//print states
	cout << "({";
	for(int i = 0; i < n->states.size(); i++) {
		if(i > 0) cout << ",";
		cout << "S" << n->states[i];
	}
	cout << "},{";
	
	//print alphabet
	for(int i = 0; i < alphabet.size(); i++) {
		if(i > 0) cout << ",";
		cout << alphabet[i];
	}
	cout << "},";
	
	//print transitions
	if(n->from.size() == 0) {
		cout << "O,";
	}
	else {	
		cout << "{";
		for(int i = 0; i < n->from.size(); i++) {
			if(i > 0) cout << ",";
			cout << "(S" << n->from[i] << ",";
			cout << n->symbol[i] << ",";
			cout << "S" << n->to[i] << ")";
		}
		cout << "},";
	}
	
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

//afiseaza NFA in format string
void print_NFA_Official(NFA_Official *n) {
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
