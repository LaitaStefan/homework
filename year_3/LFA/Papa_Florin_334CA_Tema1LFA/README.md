Nume: Florin Papa
Grupa: 334CA
Asistent: MD
Comanda: java Checker shaorma
Multime vida : O
Executabil: shaorma

This project implements conversion methods between representation forms of
automata: regular expressions(RE), non-deterministic finite automaton(NFA) and 
deterministic finite automaton(DFA). The parsing of the input is made using
FLEX and its state machine. The program can convert to various representation
forms or it can check whether a word is accepted by an automaton(if the 
representation form is other than DFA, it is converted to DFA in order to 
decide if the word is accepted by the machine).


Limbajul ales de mine contine totalitatea cuvintelor care se termina cu litera
"a" si foloseste alfabetul {a,b}.

Tema a fost scrisa in C++. Fisierul Makefile obtine executabilul shaorma. Tema a 
fost testata local folosind acest executabil si checker-ul v2.1, lansat in 
executie prin comanda "java Checker shaorma". Local obtin 96/96 puncte.

Arhiva contine urmatoarele fisiere:
	- README
	- Makefile
	- my-afd - AFD - ul aferent limbajului descris mai sus
	- my-afn - AFN - ul aferent limbajului descris mai sus
	- my-er - expresia regulata aferenta limbajului descris mai sus
	- main.lex - aici se realizeaza parsarea input-ului si se produce output-ul
	- DFABuilder.h - metode necesare obtinerii unui AFD dintr-un AFN
	- ER.h - metode necesare obtinerii unei expresii regulate dintr-un AFD
	- NFABuilder.h - metode necesare obtinerii unui AFN dintr-o expresie 
					regulata
	- utils.h - metode necesare obtinerii arborelui de parsare aferent unei
				expresii regulate citite
				
Fisierul main.lex
	Aici se face parsarea datelor de intrare folosind flex. In cazul in care 
datele incep cu "{" inseamna ca avem la intrare o expresie regulata. Se trece in
starea "ALPHA-ER", ceea ce inseamna ca se citeste alfabetul atasat expresiei 
regulate. Toate simbolurile cu exceptia celor permise sunt ignorate. Cand se 
intalneste "}:" inseamna ca s-a terminat alfabetul si incepe expresia regulata.
Aceasta se citeste caracter cu caracter, retinandu-se mereu caracterul anterior 
pentru a determina daca s-a produs o concatenare. Daca simbolul anterior nu a
fost un operator, inseamna ca s-a produs o concatenare, marcata cu litera C.
Expresia procesata(cu simboluri pentru concatenari) este salvata intr-un string.
	Daca datele incep cu "({" inseamna ca avem AFD sau AFN. In functie de para-
metrul din linia de comanda, se trece la citirea unui AFN sau AFD. Acestea sunt
tratate similar datorita formatului asemanator. Exista o stare diferita pentru
fiecare categorie de date citite(stari, alfabet, tranzitii, stare initiala, 
stari finale). Toate spatiile, virgulele, parantezele sun ignorate si se retin 
numai datele importante. Acestea sunt stocate intr-o clasa NFAOfficial sau 
DFAOfficial, urmand a fi convertite intr-un format local pentru procesare, daca
este nevoie.
	Dupa parsare, in functie de informatiile cerute in linia de comanda, se
efectueaza transformarile necesare si se produce output-ul.

Fisierul utils.h
	Prima transformare abordata a fost expresie regulata -> NFA. Pentru aceasta,
trebuie construit arborele de parsare al expresiei. Pentru construirea arborelui
de parsare, an construit clasa tree_node, care contine campurile tip, fiu stang,
fiu drept si valoare. Construirea se face recursiv, parcurgand string-ul care 
contine expresia regulata caracter cu caracter si construind nodurile aferente. 
Prioritatea pentru operatori este, in ordine descrescatoare, concatenare, "*",
"+", "?", alternare, prioritatea crescand daca se pun paranteze.
	Dupa construirea arborelui de parsare, se poate construi NFA-ul. 
	
Fisierul NFABuilder.h
	NFA-ul se construieste recursiv, pe baza arborelui de parsare. In functie de
tipul nodului curent, se construieste NFA-ul aferent. Daca nodul este o frunza,
se construieste un NFA format din 2 stari, una initiala, una finala, avand o 
tranzitie pe simbolul din nod(functia "build_simple_NFA"). Daca tipul este 
alternare, se construieste un NFA prin alternarea NFA-urilor obtinute din fiii
nodului. Alternarea este construita adaugand o stare initiala si una finala si 
tranzitii pe "e" din starea initiala in starile initiale ale NFA-urilor vechi si
din starile finale ale acestora in noua stare finala(functia build_alter_NFA). 
Pentru concatenare, noul NFA va fi compus din fiul stanga, a carui stare finala
devine fosta stare initiala a fiului drept, noua stare initiala find cea a 
fiului stang, iar cea finala, starea finala a fiului drept(functia 
build_concat_NFA). Pentru Kleene Star, se foloseste numai fiul stang(operator 
unar). Se adauga o noua stare initiala si finala, tranzitie pe "e" intre 
acestea, intre noua stare finala si vechea stare initiala a fiului, intre vechea
stare finala a fiului si noua stare finala si intre vechea stare finala a fiului
si vechea stare initiala a acestuia(functia build_star_NFA). Pentru "?" si "+" 
constructia este similara cu Kleene Star, in cazul "?" lipsind tranzitia pe "e" 
intre vechea stare finala si vechea stare initiala(functia build_question_NFA), 
iar in cazul "+" lipsind tranzitia pe "e" intre noua stare initiala si noua 
stare finala(functia build_plus_NFA). Algoritmul este cel descris anterior, insa
fisierul mai contine functii auxiliare, a caror functionalitate este descrisa
in comentariile atasate in cod.

Fisierul DFABuilder.h
	Pentru a obtine un DFA dintr-un NFA, am folosit algoritmul subset construct.
Acesta are nevoie de o functie care primeste un set de stari din NFA si intoarce
un set de stari in care se poate ajunge din cele primite pe simbolul vid(functia
eps-closure). Starea initiala a DFA-ului este reprezentata de setul eps-closure
al starii initiale a NFA-ului. Aceasta este introdusa in DFA cu indicele 0, fara
a fi insa marcata ca si vizitata. Cat timp noul DFA are stari nevizitate, se
marcheaza o stare ca fiind vizitata, dupa care se verifica daca aceasta contine
stari finale ale NFA-ului. Daca da, starea curenta este marcata ca si stare 
finala. Pentru fiecare simbol din alfabet, se construieste un set prin mutarea
starilor NFA-ului din cadrul starii curente a DFA-ului si se determina multimea
eps-closure pentru noul set. Daca aceasta multime nu exista printre starile 
DFA-ului, este adaugata ca si nevizitata. Indiferent daca multimea exista deja 
sau nu, se adauga o tranzitie de la starea curenta la multimea eps-closure pe 
caracterul curent.
	Functia "move" primeste ca parametru un NFA, un set si un caracter si 
intoarce un set de stari in care se poate ajunge prin tranzitii din starile 
primite pe caracterul primit.
	Functia "isValid" primeste un DFA si un string si determina daca acesta este
continut de limbajul descris de DFA. Pentru fiecare caracter al string-ului, se
efectueaza tranzitia in starile DFA-ului. La final, se verifica daca starea in 
care am ajuns este una finala.
	Celelalte functii definite in DFABuilder.h sunt functii ajutatoare, iar 
comportamentul lor este descris in comentariile din cod.

Fisierul ER.h
	Pentru a obtine o expresie regulata dintr-ul DFA, am folosit algoritmul 
Brzozowski. In cadrul acestui algoritm, se foloseste un vector de string-uri si
o matrice de stringuri. Fiecare element al vectorului este initializat cu 
valoarea "O" daca indexul curent nu este stare finala si cu "e" in caz contrar.
Matricea este initializata cu "O", iar pentru fiecare tranzitie intre 2 stari 
se face alternare intre simbolul pe care se face tranzitia si valoarea curenta 
de la linia "from" si coloana "to", from si to fiind starile intre care se face
tranzitia pe caracterul respectiv. In continuare, se aplica algoritmul, care 
obtine expresii regulate folosind numai concatenari, alternari si Kleene Star.
Rezultatul este depus la indicele 0 al vectorului de string-uri. In cazul conca-
tenarii intre un simbol si "O" se obtine "O", in cazul concatenarii intre "e" si
un simbol se obtine simbolul, iar in cazul concatenarii a 2 alte simboluri se
concateneaza acele simboluri inconjurate fiecare de paranteze(pentru 
corectitudine) - functia concatenate. In cazul alternarii unui simbol cu "O", se 
obtine simbolul, iar in cazul alternarii a doua alte simboluri, se imbraca 
fiecare simbol intre paranteze si se pune "|" intre ele - functia alter. In 
cazul Kleene Star,daca avem "O" se obtine "e", in caz contrar imbracandu-se 
simbolul in paranteze si punand "*" la coada - functia star. Alte functii 
prezente in acest fisier au rol ajutator, comportamentul lor fiind descris in 
comentariile din cod.

	Am implementat conversia din expresie regulata in AFN, din AFN in AFD si din
AFD in expresie regulata, avand astfel posibilitatea de a obtine orice forma 
echivalenta dintr-o reprezentare data.	 		 	 	  								
