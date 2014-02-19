//Papa Florin, 334CA 

#include "NFABuilder.h"
#include "DFABuilder.h"
#include "ER.h"
using namespace std;

vector<char> alphabet;
char preprocess[1000];
int offset = 0, position = 0;
char allowed[20] = "!#$%&-./:;<>=@[]^`~";

//verifica daca un simbol este operator sau caracter permis
int isAllowedCharacter(char c) {
	if(isalnum(c)) return 1;
	
	for(int i = 0; i < 19; i++) {
		if(c == allowed[i]) return 1;
	}
	
	return 0;
}

//construieste arbore din caracter
tree_node* chr()
{
    char data = preprocess[position];

    if (isAllowedCharacter(data) || data == 0)
    {
    	position++;
        return new tree_node(CHR, preprocess[position-1], 0, 0);
    }

    exit(1); //eroare
}

tree_node* expr();

// atom = 	chr
//      |   '(' expr ')'
tree_node* atom()
{
    tree_node* atom_node;

    if (preprocess[position] == '(')
    {
        position++;
        atom_node = expr();

        if (preprocess[position++] != ')')
        {
            cerr << "Eroare: se astepta ')'" << endl;
            exit(1);
        }
    }
    else
    {
        atom_node = chr();
    }

    return atom_node;
}


// rep  = 	atom '*'
//      |   atom '?'
//      |   atom '+'
//		| 	atom
tree_node* rep()
{
    tree_node* atom_node = atom();

    if (preprocess[position] == '*')
    {
        position++;

        tree_node* rep_node = new tree_node(STAR, 0, atom_node, 0);
        return rep_node;
    }
    else if (preprocess[position] == '?')
    {
        position++;

        tree_node* rep_node = new tree_node(QUESTION, 0, atom_node, 0);
        return rep_node;
    }
    else if (preprocess[position] == '+')
    {
        position++;

        tree_node* rep_node = new tree_node(PLUS, 0, atom_node, 0);
        return rep_node;
    }
    else
    {
        return atom_node;
    }
}


// concat   = 	rep . concat
//          |   rep
tree_node* concat()
{
    tree_node* left = rep();

    if (preprocess[position] == 'C')
    {
        position++;
        tree_node* right = concat();

        tree_node* concat_node = new tree_node(CONCAT, 0, left, right);
        return concat_node;
    }
    else
    {
        return left;
    }
}

// expr = concat '|' expr
//      |   concat
tree_node* expr()
{
    tree_node* left = concat();

    if (preprocess[position] == '|')
    {
        position++;
        tree_node* right = expr();

        tree_node* expr_node = new tree_node(ALTER, 0, left, right);
        return expr_node;
    }
    else
    {
        return left;
    }
}
