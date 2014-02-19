Papa Florin, 324CA

In this homework, we had to solve three problems. The first problem was to find
the maximum distance that can be obtained by removing a maximum number of M 
pillars from a fence with N pillars. The fist and last pillars could not be 
removed. This problem was solved using binary search. Binary search was 
suitable due to the time performance and the fact that the possible distances
could be considered as a sorted array from 1 to L, L being the distance between
the first and last pillar.
The second problem was to find the maximum increasing subsequence of an array
and was solved using dynamic programming.
The third problem was a generalisation of the first one: application on 
circular arrays.


Pentru prima problema, am folosit binary search pe intervalul [1,L] pentru a 
determina distanta maxima care poate fi obtinuta eliminand cel mult M stalpi 
din gard. Functia "getNumberOfElements" intoarce un intreg ce reprezinta 
numarul de stalpi din gardul initial ce pot fi folositi pentru a obtine o 
anumita distanta minima data ca parametru. Aceasta functie este folosita in 
BinarySearch pentru a determina pe ce ramura se continua. Daca numarul intors 
de functie se incadreaza in intervalul [N-M,N], salvam rezultatul in variabila 
max si mergem pe ramura din dreapta pentru a verifica daca nu putem obtine un 
maxim mai mare. In caz contrar, cautam in partea stanga.
Dupa ce aflam valoarea lui max, reconstituim solutia parcurgand vectorul si
retinand valorile care se incadreaza in conditia impusa de maximul obtinut.
Complexitatea obtinuta este O(N*log(L)) (log(L) de la binary search si N 
deoarece la fiecare pas se parcurge vectorul pentru a obtine numarul de 
stalpi). 


Pentru ce de-a doua problema, am folosit progamare dinamica. Pentru a lua in 
considerare toate solutiile posibile, am construit o matrice de dimensiune NxT 
denumita max_sums care retine solutiile partiale. Relatia de recurenta este 
urmatoarea:

max_sums(i,j) = max(max_sums(i-1,j-1) + cost[i], 
			    max_sums(k, j-1) for k = j-1..i-1 )).

Prima coloana a matricii am initializat-o cu 0 deaorece orice sir format cu un 
singur element are valoarea 0. Elementul maxim de pe ultima coloana reprezinta 
"profitul" maxim care poate fi obtinut. Pentru reconstituirea solutiei am 
folosit clasa MyInterval care retine profitul, indexul elementului curent, dar 
si inceputul sirului din care face parte acest element. Din motive de 
performanta, am mai folosit o matrice denumita maxes care retine pe fiecare 
coloana profitul maxim de la linia 0 la linia curenta. Pentru reconstructia 
solutiei, selectez maximul de pe ultima coloana si introduc in vectorul de 
solutii linia curenta si inceputul de sir pentru elementul curent. Avand primul
interval, scad din coloana curenta numarul de elemente al acestui interval si 
repet procesul pana cand am introdus toate elementele. La fiecare pas, caut 
maximul pe coloana de la linia 0 la ultimul element introdus pentru a evita 
producerea de duplicate. Complexitatea astfel obtinuta este O(N*T) data de 
producerea matricii de costuri.
	
	
Cea de-a treia problema reprezinta o generalizare a problemei 2. Pentru 
rezolvarea acesteia am folosit problema 2, insa pentru a obtine intervale 
circulare, am rearanjat vectorul de costuri astfel: am selectat elementul minim
din vector, iar toate elementele de la minim la final le-am pus intr-un nou 
vector; elementele de la 0 la minim-1 le-am adaugat la finalul vectorului nou 
creat. In continuare, am aplicat solutia descrisa la punctul precedent, dupa 
care am adaptat intervalele la vectorul original de intrare. Complexitatea 
este, la fel ca la problema 2, O(N*T). 