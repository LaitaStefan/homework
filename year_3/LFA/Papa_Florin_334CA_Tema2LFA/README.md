Nume: Florin Papa
Grupa: 334CA
Asistent: MD
Multime vida : O
Executabil: shaorma

Tema 2 LFA

This project implements conversion methods between different representation 
forms of automata(regular expressions(RE), non-deterministic finite automaton
(NFA) and deterministic finite automaton(DFA)) and XML. The parsing of the 
input is made using FLEX and its state machine. If an XML representation is
given as input, a normal representation is obtained and if we provide a normal
representation, an XML representation is obtained.


Pentru a realiza tema 2, am adus modificari temei 1. Structura de fisiere
este aceeasi ca in cazul primei teme. Fisierele care se ocupa de NFA, DFA, RE
nu mai contin acum algoritmii de conversie de la o forma de reprezentare la 
alta, ci doar functii de afisare.
In cazul in care se doreste conversie de la forma normala la XML, se foloseste
parserul de la tema1, care citeste un NFA/DFA sau o RE. In urma stocarii 
acesteia, se face afisarea in format XML. Pentru RE, afisarea se face prin
parcurgerea arborelui de parsare aferent expresiei regulate.
In sens invers, in cazul NFA si DFA, am construit in cadrul parserului FLEX o 
masina de stari care, in functie de elementul curent(tranzitie, alfabet, stare
initiala etc.) introduce tokenul care nu este tag(nu este inconjurat de <>) in
sectiunea corespunzatoare din cadrul reprezentarii interne a NFA/DFA. In urma
stocarii, se afiseaza forma de reprezentare.
In cazul RE, afisarea se face pe masura ce se citeste. Toate elementele sunt
inconjurate de paranteze pentru a preveni eventuale greseli. Cel mai complicat
de afisat sunt reununile deoarece trebuie sa ne dam seama cand trebuie pus 
semnul de reuniune. Pentru aceasta am folosit un contor si o stiva. In momentul 
in care afisam un element nou, verificam daca in varful stivei se afla opera-
torul de reuniune si daca contorul este diferit de 0. Daca da, se afiseaza "|".