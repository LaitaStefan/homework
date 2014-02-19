//Papa Florin, 334CA

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<vector>
#include<set>
using namespace std;
int align = 0;
int poz = 0;

void print_tabs() {
	for(int i = 0; i < align; i++) {
		cout << "\t";
	}
}

void print_XML(tree_node *n) {
	align++;
	print_tabs();

	if(n->type == CHR) {
		if(n->val != 'e') {
	 		cout << "<basic>\n";

	 		align++;
	 		print_tabs();
	 		cout << "<symbol>" << n->val << "</symbol>\n";

	 		align--;
	 		print_tabs();
	 		cout << "</basic>\n";
 		}
 		else {
 			cout << "<basic>" << n->val << "</basic>\n";
 		}
	}	
	
	if(n->type == ALTER) {
		cout << "<reunion>\n";
		print_XML(n->left);
		print_XML(n->right);
		print_tabs();
		cout << "</reunion>\n";
	}
	
	if(n->type == CONCAT) {
		cout << "<concatenation>\n";
		print_XML(n->left);
		print_XML(n->right);
		print_tabs();
		cout << "</concatenation>\n";
	}
	
	if(n->type == STAR) {
		cout << "<star>\n";
		print_XML(n->left);
		print_tabs();
		cout << "</star>\n";
	}
	
	if(n->type == PLUS) {
		cout << "<plus>\n";
		print_XML(n->left);
		print_tabs();
		cout << "</plus>\n";
	}
	
	if(n->type == QUESTION) {
		cout << "<optional>\n";
		print_XML(n->left);
		print_tabs();
		cout << "</optional>\n";
	}
	
	align--;
}

//afiseaza expresie regulata in format XML
void print_RE_XML(tree_node* n, vector<char> alphabet) {
	
	cout << "<RE>\n";

	//print alphabet
	cout << "\t<alphabet>\n";
	for(int i = 0; i < alphabet.size(); i++) {
		cout << "\t\t<symbol>" << alphabet[i] << "</symbol>\n";
	}
	cout << "\t</alphabet>\n";

	print_XML(n);
	cout << "</RE>\n\n";
}