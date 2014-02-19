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

//afiseaza DFA in format string
void print_DFA_Official(DFA_Official *n, vector<char> alphabet) {
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
	cout << "{";
	for(int i = 0; i < n->from.size(); i++) {
		if(i > 0) cout << ",";
		cout << "d(" << n->from[i] << ",";
		cout << n->symbol[i] << ")=";
		cout  << n->to[i];
	}
	cout << "},";
	
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

//afiseaza DFA in format XML
void print_DFA_XML(DFA_Official *n, vector<char> alphabet) {

	cout << "<DFA>\n";

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
	cout << "\t<delta>\n";
	for(int i = 0; i < n->from.size(); i++) {
		cout << "\t\t<transition>\n";

		cout << "\t\t\t<source>\n";
		cout << "\t\t\t\t<state>" << n->from[i] << "</state>\n";
		cout << "\t\t\t</source>\n";

		cout << "\t\t\t<symbol>" << n->symbol[i] << "</symbol>\n";
	
		cout << "\t\t\t<destination>\n";
		cout << "\t\t\t\t<state>" << n->to[i] << "</state>\n";
		cout << "\t\t\t</destination>\n";

		cout << "\t\t</transition>\n";
	}
	cout << "\t</delta>\n";
	
	//print initial
	cout << "\t<initial>\n";
	cout << "\t\t<state>" << n->initial << "</state>\n";
	cout << "\t</initial>\n";
	
	//print final
	cout << "\t<final>\n";
	for(int i = 0; i < n->final.size(); i++) {
		cout << "\t\t<state>" << n->final[i] << "</state>\n";
	}
	cout << "\t</final>\n";

	cout << "</DFA>\n\n";
}
