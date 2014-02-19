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

	tree_node() {}	
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

//afiseaza NFA in format string
void print_NFA_Official(NFA_Official *n, vector<char> alphabet) {
	//print states
	cout << "({";
	for(int i = 0; i < n->states.size(); i++) {
		if(i > 0) cout << ",";
		cout << n->states[i];
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
			cout << "(" << n->from[i] << ",";
			cout << n->symbol[i] << ",";
			cout << n->to[i] << ")";
		}
		cout << "},";
	}
	
	//print initial
	cout << n->initial << ",";
	
	//print final
	cout << "{";
	for(int i = 0; i < n->final.size(); i++) {
		if(i > 0) cout << ",";
		cout << n->final[i];
	}
	cout <<  "})\n\n";
}	

//afiseaza NFA in format XML
void print_NFA_XML(NFA_Official *n, vector<char> alphabet) {
	cout << "<NFA>\n";

	//print states
	cout << "\t<states>\n";
	for(int i = 0; i < n->states.size(); i++) {
		cout << "\t\t<state>" << n->states[i] << "</state>\n";
	}
	cout << "\t</states>\n";
	
	//print alphabet
	cout << "\t<alphabet>\n";
	for(int i = 0; i < alphabet.size(); i++) {
		cout << "\t\t<symbol>" << alphabet[i] << "</symbol>\n";
	}
	cout << "\t</alphabet>\n";
	
	//print transitions
	cout << "\t<Delta>\n";
	for(int i = 0; i < n->from.size(); i++) {
		cout << "\t\t<relation>\n";

		cout << "\t\t\t<source>\n";
		cout << "\t\t\t\t<state>" << n->from[i] << "</state>\n";
		cout << "\t\t\t</source>\n";
	
		if(n->symbol[i][0] == 'e') {
			cout << "\t\t\t<word>" << "e" << "</word>\n";
		}
		else {
			cout << "\t\t\t<word>\n";
			for(int j = 0; j < n->symbol[i].size(); j++) {
				cout << "\t\t\t\t<symbol>" << n->symbol[i][j] << "</symbol>\n";
			}
			cout << "\t\t\t</word>\n";
		}

		cout << "\t\t\t<destination>\n";
		cout << "\t\t\t\t<state>" << n->to[i] << "</state>\n";
		cout << "\t\t\t</destination>\n";

		cout << "\t\t</relation>\n";
	}
	cout << "\t</Delta>\n";
	
	//print initial
	cout << "\t<initial>\n";
	cout << "\t\t<state>" << n->initial << "</state>\n";
	cout << "\t</initial>\n";
	
	//print final
	cout << "\t<final>\n";
	for(int i = 0; i < n->final.size(); i++) {
		cout << "\t\t<state>" << n->final[i] << "</state>\n";
	}
	cout <<  "\t</final>\n";

	cout << "</NFA>\n\n";
}	
