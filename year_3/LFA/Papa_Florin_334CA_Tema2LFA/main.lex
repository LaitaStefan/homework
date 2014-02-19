/* Papa Florin, 334CA */

%{
	#include "utils.h"
	
	char prev = -1;
	int count_st = 0;
	int state = 1;	//1 REGEX, 2 AFN, 3 AFD
	int tranz = 0;	//folosit la parsarea NFA, DFA si RE
	string curr_word("");
	NFA_Official *my_nfa;
	DFA_Official *my_dfa;
	stack<string> regex_stack;
%}       

digit   	[0-9]
letter		[a-z]|"O"
capitals	[A-Z]
specials	"!"|"#"|"$"|"%"|"&"|"-"|"."|"/"|":"|";"|"<"|">"|"="|"@"|"["|"]"|"^"|"`"|"~"
specials_XML	"!"|"#"|"$"|"%"|"&"|"-"|"."|"/"|":"|";"|"="|"@"|"["|"]"|"^"|"`"|"~"
operator 	[|*?+()]
space		[ \n\t\r\f]*
expression	({digit}|{letter}|{specials}|{operator}){1,1}
token		({digit}|{letter}|{specials}|{capitals}|"_"){1,1}
token_XML	({digit}|{letter}|{specials_XML}|{capitals}|"_"){1,1}

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
%x 	NFA_XML
%x 	NFA_XML_Delta
%x 	NFA_XML_alpha
%x 	NFA_XML_states
%x 	NFA_XML_fin
%x 	NFA_XML_init
%x 	DFA_XML
%x 	DFA_XML_Delta
%x 	DFA_XML_alpha
%x 	DFA_XML_states
%x 	DFA_XML_fin
%x 	DFA_XML_init
%x 	RE_XML
%x 	RE_XML_alpha

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

"<DFA>" {
	BEGIN(DFA_XML);
	my_dfa = new DFA_Official();
}

"<NFA>" {
	BEGIN(NFA_XML);
	my_nfa = new NFA_Official();
}

"<RE>" {
	BEGIN(RE_XML_alpha);
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


<NFA_XML>{space} {}
<NFA_XML>"<states>" {
	BEGIN(NFA_XML_states);
}		

<NFA_XML_states>{space} {}
<NFA_XML_states>"<state>" {}
<NFA_XML_states>"</state>" {}
<NFA_XML_states>"</states>" {
	BEGIN(NFA_XML_alpha);
}
<NFA_XML_states>{token_XML}* {
	string s(yytext);
	my_nfa->states.push_back(s);
}	

<NFA_XML_alpha>{space} {}
<NFA_XML_alpha>"<alphabet>" {}
<NFA_XML_alpha>"<symbol>" {}
<NFA_XML_alpha>"</symbol>" {}
<NFA_XML_alpha>"</alphabet>" {
	BEGIN(NFA_XML_Delta);
}
<NFA_XML_alpha>{token_XML} {
	alphabet.push_back(yytext[0]);
}

<NFA_XML_Delta>{space} {}
<NFA_XML_Delta>"<Delta>" {}
<NFA_XML_Delta>"</Delta>" {
	BEGIN(NFA_XML_init);
}
<NFA_XML_Delta>"<relation>" {}
<NFA_XML_Delta>"</relation>" {}
<NFA_XML_Delta>"<source>" {}
<NFA_XML_Delta>"</source>" {}
<NFA_XML_Delta>"<destination>" {}
<NFA_XML_Delta>"</destination>" {}
<NFA_XML_Delta>"<symbol>" {}
<NFA_XML_Delta>"</symbol>" {}
<NFA_XML_Delta>"<state>" {}
<NFA_XML_Delta>"</state>" {}
<NFA_XML_Delta>"<word>" { tranz = 1; };
<NFA_XML_Delta>"</word>" { 
	tranz = 2;
	my_nfa->symbol.push_back(curr_word);
	curr_word = ""; 
};
<NFA_XML_Delta>{token_XML}* {
	string s(yytext);
	if(tranz == 0) my_nfa->from.push_back(s);
	else if(tranz == 1) curr_word.append(1, yytext[0]);
	else if(tranz == 2) {
		my_nfa->to.push_back(s);
		tranz = 0;
	}
}

<NFA_XML_init>{space} {}
<NFA_XML_init>"<initial>" {}
<NFA_XML_init>"<state>" {}
<NFA_XML_init>"</state>" {}
<NFA_XML_init>"</initial>" {
	BEGIN(NFA_XML_fin);
}
<NFA_XML_init>{token_XML}* {
	string s(yytext);
	my_nfa->initial = s;
}

<NFA_XML_fin>{space} {}
<NFA_XML_fin>"<final>" {}
<NFA_XML_fin>"</final>" {}
<NFA_XML_fin>"<state>" {}
<NFA_XML_fin>"</state>" {}
<NFA_XML_fin>"</NFA>" {}
<NFA_XML_fin>{token_XML}* {
	string s(yytext);
	my_nfa->final.push_back(s);
}


<DFA_XML>{space} {}
<DFA_XML>"<states>" {
	BEGIN(DFA_XML_states);
}		

<DFA_XML_states>{space} {}
<DFA_XML_states>"<state>" {}
<DFA_XML_states>"</state>" {}
<DFA_XML_states>"</states>" {
	BEGIN(DFA_XML_alpha);
}
<DFA_XML_states>{token_XML}* {
	string s(yytext);
	my_dfa->states.push_back(s);
}	

<DFA_XML_alpha>{space} {}
<DFA_XML_alpha>"<alphabet>" {}
<DFA_XML_alpha>"<symbol>" {}
<DFA_XML_alpha>"</symbol>" {}
<DFA_XML_alpha>"</alphabet>" {
	BEGIN(DFA_XML_Delta);
}
<DFA_XML_alpha>{token_XML} {
	alphabet.push_back(yytext[0]);
}

<DFA_XML_Delta>{space} {}
<DFA_XML_Delta>"<delta>" {}
<DFA_XML_Delta>"</delta>" {
	BEGIN(DFA_XML_init);
}
<DFA_XML_Delta>"<transition>" {}
<DFA_XML_Delta>"</transition>" {}
<DFA_XML_Delta>"<source>" {}
<DFA_XML_Delta>"</source>" {}
<DFA_XML_Delta>"<destination>" {}
<DFA_XML_Delta>"</destination>" {}
<DFA_XML_Delta>"<symbol>" {}
<DFA_XML_Delta>"</symbol>" {}
<DFA_XML_Delta>"<state>" {}
<DFA_XML_Delta>"</state>" {}
<DFA_XML_Delta>{token_XML}* {
	string s(yytext);
	if(tranz == 0) {
		my_dfa->from.push_back(s);
		tranz = 1;
	}
	else if(tranz == 1) {
		my_dfa->symbol.push_back(s);
		tranz = 2;
	}
	else if(tranz == 2) {
		my_dfa->to.push_back(s);
		tranz = 0;
	}
}

<DFA_XML_init>{space} {}
<DFA_XML_init>"<initial>" {}
<DFA_XML_init>"<state>" {}
<DFA_XML_init>"</state>" {}
<DFA_XML_init>"</initial>" {
	BEGIN(DFA_XML_fin);
}
<DFA_XML_init>{token_XML}* {
	string s(yytext);
	my_dfa->initial = s;
}

<DFA_XML_fin>{space} {}
<DFA_XML_fin>"<final>" {}
<DFA_XML_fin>"</final>" {}
<DFA_XML_fin>"<state>" {}
<DFA_XML_fin>"</state>" {}
<DFA_XML_fin>"</DFA>" {}
<DFA_XML_fin>{token_XML}* {
	string s(yytext);
	my_dfa->final.push_back(s);
}

<RE_XML_alpha>{space} {}
<RE_XML_alpha>"<alphabet>" {
	cout << "{";
}
<RE_XML_alpha>"<symbol>" {}
<RE_XML_alpha>"</symbol>" {}
<RE_XML_alpha>"</alphabet>" {
	BEGIN(RE_XML);
	cout << "}:";
}
<RE_XML_alpha>{token_XML} {
	if(alphabet.size() > 0) cout << ",";
	cout << yytext[0];
	alphabet.push_back(yytext[0]);
}

<RE_XML>{space} {}
<RE_XML>"<basic>" {}
<RE_XML>"</basic>" {}
<RE_XML>"<symbol>" {}
<RE_XML>"</symbol>" {}
<RE_XML>"<reunion>" {
	string s("reunion");

	if(!regex_stack.empty() && regex_stack.top() == "reunion") {
		if(tranz > 0) cout << "|";
		else tranz++;
	}
	regex_stack.push(s);

	tranz = 0;

	cout << "(";
}
<RE_XML>"</reunion>" {
	cout << ")";
	regex_stack.pop();
}
<RE_XML>"<concatenation>" {
	string s("concat");

	if(!regex_stack.empty() && regex_stack.top() == "reunion") {
		if(tranz > 0) cout << "|";
		else tranz++;
	}
	regex_stack.push(s);

	cout << "(";
}
<RE_XML>"</concatenation>" {
	cout << ")";

	regex_stack.pop();
}
<RE_XML>"<star>" {
	string s("star");

	if(!regex_stack.empty() && regex_stack.top() == "reunion") {
		if(tranz > 0) cout << "|";
		else tranz++;
	}
	regex_stack.push(s);

	cout << "(";
}
<RE_XML>"</star>" {
	cout << ")*";

	regex_stack.pop();
}
<RE_XML>"<optional>" {
	string s("optional");

	if(!regex_stack.empty() && regex_stack.top() == "reunion") {
		if(tranz > 0) cout << "|";
		else tranz++;
	}
	regex_stack.push(s);

	cout << "(";
}
<RE_XML>"</optional>" {
	cout << ")?";

	regex_stack.pop();
}
<RE_XML>"<plus>" {
	string s("plus");

	if(!regex_stack.empty() && regex_stack.top() == "reunion") {
		if(tranz > 0) cout << "|";
		else tranz++;
	}
	regex_stack.push(s);

	cout << "(";
}
<RE_XML>"</plus>" {
	cout << ")+";

	regex_stack.pop();
}
<RE_XML>"</RE>" { 
	cout << "\n\n"; 
}
<RE_XML>{token_XML}* {
	string s(yytext);
	
	if(!regex_stack.empty() && regex_stack.top() == "reunion") {
		if(tranz > 0) cout << "|";
		else tranz++;
	}

	cout << s;
}

%%

int main(int argc, char** argv) {
	char in[20000];
	char aux[1000];
	int offset = 0;
	
	if(strcmp(argv[2], "--from-XML") != 0) {
		fgets(in, 3000, stdin);
	}
	else {
		while(1) {
			memset(aux, 0, 1000);
			fgets(aux, 1000, stdin);
			if(strlen(aux) == 0) break;
			memcpy(in + offset, aux, strlen(aux));
			offset += strlen(aux);
		}
	}

	
	if(strcmp(argv[1], "--from-RE") == 0) state = 0;
	if(strcmp(argv[1], "--from-NFA") == 0) state = 1;
	if(strcmp(argv[1], "--from-DFA") == 0) state = 2;
	
    yy_scan_string(in);
    yylex();
    yy_delete_buffer(YY_CURRENT_BUFFER);

	if(strcmp(argv[2], "--to-XML") == 0) {
		if(state == 0) {	//RE
			tree_node *n = expr();
			print_RE_XML(n, alphabet);
		}	
		else if(state == 1) {	//NFA
			print_NFA_XML(my_nfa, alphabet);
		}
		else if(state == 2) {	//DFA
			print_DFA_XML(my_dfa, alphabet);
		}
	}
	else if(strcmp(argv[2], "--from-XML") == 0) {
		if(state == 1) {
			print_NFA_Official(my_nfa, alphabet);
		}
		else if(state == 2) {
			print_DFA_Official(my_dfa, alphabet);
		}
	}	
    
    return 0;
}
