/* Papa Florin, 334CA */

%{
	#include "utils.h"
	
	char prev = -1;
	int count_st = 0;
	int state = 1;	//1 REGEX, 2 AFN, 3 AFD
	NFA_Official *my_nfa;
	DFA_Official *my_dfa;
		
%}       

digit   	[0-9]
letter		[a-z]|"O"
capitals	[A-Z]
specials	"!"|"#"|"$"|"%"|"&"|"-"|"."|"/"|":"|";"|"<"|">"|"="|"@"|"["|"]"|"^"|"`"|"~"
operator 	[|*?+()]
space		[ \n\t\r\f]*
expression	({digit}|{letter}|{specials}|{operator}){1,1}
token		({digit}|{letter}|{specials}|{capitals}|"_"){1,1}

%x 	ALPHA_ER
%x 	REGEX
%x	AFN_states
%x 	AFN_alpha
%x 	AFN_tranz
%x 	AFN_init
%x 	AFN_fin	
%x	AFD_states
%x 	AFD_alpha
%x 	AFD_tranz
%x 	AFD_init
%x 	AFD_fin

%%
"{" {
	BEGIN(ALPHA_ER);
}

"({" {
	if(state == 1) {
		BEGIN(AFN_states);
		my_nfa = new NFA_Official();
	}
	else if(state == 2) {
		BEGIN(AFD_states);
		my_dfa = new DFA_Official();
	}
	else BEGIN(INITIAL);
}

<ALPHA_ER>{expression} {
	char curr = yytext[0];
	alphabet.push_back(curr);
}

<ALPHA_ER>{space} {}
<ALPHA_ER>","* {}

<ALPHA_ER>"}:" {
	BEGIN(REGEX);
}

<REGEX>{expression} {
	char curr = yytext[0];
	
	if(prev != -1) {		
		if(prev != '(' && prev != '|' && curr != ')' && curr != '|' && curr != '*' && curr != '?' && curr != '+') {
			preprocess[offset] = 'C';	
			offset++;
		}
		prev = curr;
		preprocess[offset] = curr;
		offset++;
	}
	else {
		preprocess[offset] = curr;
		offset++;
		prev = curr;
	}
}

<REGEX>{space}* {}

<AFN_states>","* {}

<AFN_states>{space} {}

<AFN_states>{token}* {
	string s(yytext);
	my_nfa->states.push_back(s);

}

<AFN_states>"},{" {
	BEGIN(AFN_alpha);
}

<AFN_alpha>","* {}

<AFN_alpha>{space} {}

<AFN_alpha>{token} {
	char c = yytext[0];
	alphabet.push_back(c);
}

<AFN_alpha>"}," {
	BEGIN(AFN_tranz);
}

<AFN_tranz>[,(){]* {}

<AFN_tranz>{space} {}

<AFN_tranz>"O" { BEGIN(AFN_init); }

<AFN_tranz>{token}* {
	if(count_st == 0) {
		string s(yytext);
		my_nfa->from.push_back(s);
		count_st++;
	}
	else if(count_st == 1) {
		string s(yytext);
		my_nfa->symbol.push_back(s);
		count_st++;
	}
	else if(count_st == 2) {
		string s(yytext);
		my_nfa->to.push_back(s);
		count_st = 0;
	}
}

<AFN_tranz>"}," {
	BEGIN(AFN_init);
}

<AFN_init>{space} {}

<AFN_init>{token}* {
	string s(yytext);
	my_nfa->initial = s;
}

<AFN_init>"," {
	BEGIN(AFN_fin);
}

<AFN_fin>{space} {}

<AFN_fin>[,O{]* {}

<AFN_fin>{token}* {
	string s(yytext);
	my_nfa->final.push_back(s);
}

<AFN_fin>"}" {}

<AFN_fin>")" {}

<AFD_states>","* {}

<AFD_states>{space} {}

<AFD_states>{token}* {
	string s(yytext);
	my_dfa->states.push_back(s);
}

<AFD_states>"},{" {
	BEGIN(AFD_alpha);
}

<AFD_alpha>","* {}

<AFD_alpha>{space} {}

<AFD_alpha>{token} {
	char c = yytext[0];
	alphabet.push_back(c);
}

<AFD_alpha>"},{" {
	BEGIN(AFD_tranz);
}

<AFD_tranz>","* {}

<AFD_tranz>"d(" {}

<AFD_tranz>")=" {}

<AFD_tranz>{space} {}

<AFD_tranz>{token}* {
	if(count_st == 0) {
		string s(yytext);
		my_dfa->from.push_back(s); 
		count_st++;
	}
	else if(count_st == 1) {
		string s(yytext);
		my_dfa->symbol.push_back(s);
		count_st++;
	}
	else if(count_st == 2) {
		string s(yytext);
		my_dfa->to.push_back(s);
		count_st = 0;
	}
}

<AFD_tranz>"}," {
	BEGIN(AFD_init);
}

<AFD_init>{space} {}

<AFD_init>{token}* {
	string s(yytext);
	my_dfa->initial = s;
}

<AFD_init>"," {
	BEGIN(AFD_fin);
}

<AFD_fin>{space}* {}

<AFD_fin>[,O{]* {}

<AFD_fin>{token}* {
	string s(yytext);
	my_dfa->final.push_back(s);
}

<AFD_fin>"}" {}
<AFD_fin>")" {}
			
%%

int main(int argc, char** argv) {
	char in[3000];
	
	fgets(in, 3000, stdin);
	
	if(strcmp(argv[1], "--from-RE") == 0) state = 0;
	if(strcmp(argv[1], "--from-NFA") == 0) state = 1;
	if(strcmp(argv[1], "--from-DFA") == 0) state = 2;
	
    yy_scan_string(in);
    yylex();
    yy_delete_buffer(YY_CURRENT_BUFFER);

	if(strcmp(argv[2], "--to-RE") == 0) {
		if(state == 0) printf("%s", in);
		else if(state == 1) {			//from NFA to RE
			NFA *n = official_to_intern(my_nfa);
			DFA *d = build_DFA(n, alphabet);
			
			cout << "{";
			for(int i = 0; i < alphabet.size(); i++) {
				if(i == 0) cout << alphabet[i];
				else cout << "," << alphabet[i];
			}
			cout << "}:";
			 
			cout << buildER(d, alphabet) << "\n";
		}
		else if(state == 2) {			//from DFA to RE
			DFA *d = official_to_intern(my_dfa); 
			
			cout << "{";
			for(int i = 0; i < alphabet.size(); i++) {
				if(i == 0) cout << alphabet[i];
				else cout << "," << alphabet[i];
			}
			cout << "}:";

			cout << buildER(d, alphabet) << "\n";
		}
	}
	else if(strcmp(argv[2], "--to-NFA") == 0) {
		if(state == 0) {
			tree_node *n = expr();
			NFA *nfa = build_NFA(n);
			
			print_NFA(nfa, alphabet);
		}
		else if(state == 1) printf("%s", in);
		else if(state == 2) {
			char in2[3000];
			DFA *dfa = official_to_intern(my_dfa);

			strcpy(in2, buildER(dfa, alphabet).c_str());
						
			yy_scan_string(in2);
			BEGIN(REGEX);
			yylex();
			yy_delete_buffer(YY_CURRENT_BUFFER);
					
			tree_node *n = expr();
			NFA *nfa = build_NFA(n);
			
			print_NFA(nfa, alphabet);
		}
	}
	else if(strcmp(argv[2], "--to-DFA") == 0) {
		if(state == 0) {
			tree_node *n = expr();
			NFA *nfa = build_NFA(n);
			DFA *dfa = build_DFA(nfa, alphabet);
			print_DFA(dfa, alphabet);
		}
		else if(state == 1) {
			NFA *nfa = official_to_intern(my_nfa);
			DFA *dfa = build_DFA(nfa, alphabet);
			print_DFA(dfa, alphabet);
		}
		else if(state == 2) {
			printf("%s", in);
		}
	}
	else if(strcmp(argv[2], "--contains") == 0) {
		DFA *dfa;
		
		if(state == 0) {
			tree_node *n = expr();
			NFA *nfa = build_NFA(n);
			dfa = build_DFA(nfa, alphabet);
		}
		else if(state == 1) {
			NFA *nfa = official_to_intern(my_nfa);
			dfa = build_DFA(nfa, alphabet);
		}
		else if(state == 2) {
			dfa = official_to_intern(my_dfa);
		}
		
		for(int i = 3; i < argc; i++) {
			if(isValid(argv[i], dfa)) {
				printf("True\n");
			}
			else printf("False\n");
		}
	}		
    
    return 0;
}
