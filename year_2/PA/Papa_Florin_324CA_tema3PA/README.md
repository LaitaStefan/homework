Papa Florin, 324CA

This project solves two problems, one that determines whether a team can still
win the championship given the already played matches and one that determines 
the minimum value of the faces of a dice that rolls from one point to another.
The first problem was solved using Ford Fulkerson's Max Flow algorithm and the 
second one uses Dijkstra's minimum distance algorithm to determine the minimum
distance between two given points.


Problema 1 - Fotbal

Pentru problema 1, am folosit implementarea pentru graf si pentru determinarea 
fluxului maxim prin graf din cadrul laboratorului 10("Flux Maxim"). 
In clasa InputParser am parsat datele de intrare si am format situatia 
campionatului in functie de datele de intrare. Numarul total de meciuri de 
jucat de catre fiecare echipa l-am initializat in vectorul remaining_games la 
numar_etape * (numar_echipe - 1), iar numarul de meciuri de jucat de fiecare 
echipa cu fiecare din celelalte echipe l-am salvat in matricea games_to_play, 
initializata cu valoarea numar_etape. In functie de datele de intrare, am 
decrementat valorile din aceste structuri de date si am adaugat pentru fiecare 
echipa punctajele aferente in caz de castig sau egalitate.

Functia getGraph imi intoarce un graf avand 
(numar_echipe - 2)*(numar_echipe - 1) / 2 + numar_echipe + 1 noduri. Pe pozitia
0 in acest graf se afla sursa, pe pozitiile de la 1 la 
(numar_echipe - 2)*(numar_echipe - 1) / 2 se afla noduri reprezentand jocurile 
care mai sunt de jucat intre fiecare dintre echipe, cu exceptia celei pentru 
care dorim sa aflam daca mai are sanse de castig sau nu. Urmatoarele 
numar_echipe - 1 noduri reprezinta echipele din campionat, cu exceptia celei 
cautate de noi, iar ultimul nod reprezinta drena. Intre sursa si nodurile 
reprezentand partidele se adauga muchii avand capacitatea egala cu numarul 
maxim de puncte pe care le mai poate castiga oricare din cele 2 echipe 
implicate. Intre aceste noduri si cele reprezentand echipele care se confrunta 
in fiecare partida se adauga muchii cu capacitate foarte mare (infinita), iar 
intre nodurile echipelor si drena se pun muchii avand capacitatea egala cu 
diferenta dintre punctajul maxim posibil al echipei cautate de noi dupa 
confruntarile ramase cu echipa reprezentata de nod, din care scadem punctele 
deja existente ale acestei echipe. In cazul in care obtinem un cost negativ, 
se seteaza flag-ul negative. In acest caz, echipa tinta nu mai are cum sa 
castige campionatul deoarece a fost deja batuta de cealalta echipa, caz in care
nu mai are rost sa aplicam algoritmul de flux pe graful obtinut. In cazul in 
care nu avem muchii cu capacitati negative, aplicam algoritmul de flux pe graf 
si verificam daca toate muchiile care ies din sursa sunt saturate. Daca da, 
echipa noastra mai are sanse de castig, iar daca nu, nu mai are.

Complexitatea algoritmului este de O(numar_muchii * |f_max|), intrucat gasirea 
fluxului maxim se bazeaza pe algoritmul Ford Fulkerson.

Problema 2 - Zar

In clasa "InputParser2" parsez datele de intrare, salvand valorile fetelor 
zarului, pozitia initiala a acestuia, pozitia finala si dimensiunea hartii.
Clasa "DiceStatus" contine informatii utile despre starea unui zar precum 
fetele acestuia, distanta de la sursa la nodul curent(variabila sum), 
coordonatele zarului, variabila choice care ia valoarea 1 daca nodul a fost 
procesat in algoritmul Dijkstra, si variabila index care retine indexul 
pozitiei zarului pentru coordonatele date(pentru aceleasi coordonate putem 
avea mai multe stari ale zarului, contorizate de index si care sunt maxim 24). 
Tot in aceasta clasa efectuez si mutarea zarului, in functia moveDice, in care 
modific corespunzator fetele zarului, dupa care intorc vectorul nou obtinut.
In clasa "P2" retin un array tridimensional de clase "DiceStatus", variabilele 
citite de la intrare si 2 matrici, "sel_nr", care retine cate variante diferite
ale zarului au trecut prin acelasi nod si  "chosen", care retine daca un nod 
anume a fost procesat sau nu. In constructor initializez o variabila DiceStatus
pentru a reflecta situatia zarului la start.
Functia "isSafe" intoarce true daca zarul poate fi mutat pe o directie primita 
ca parametru si false in caz contrar.
Costul minim intre sursa si destinatie se calculeaza folosind algorimul 
Dijkstra studiat la laborator, modificat corespunzator. Coada din algoritmul 
initial a fost inlocuita cu un LinkedList, motivul fiind costul foarte mic din 
punct de vedere al timpului pentru scoaterea primului element. Cat timp aceasta
coada nu este vida, scot primul element din ea si il procesez. Primul lucru 
este sa marchez nodul scos ca fiind procesat pentru a nu-l mai procesa mai 
tarziu din nou. In continuare, pentru cele 4 directii posibile pe care se poate
merge, verific daca nu ies de pe harta si formez indicii noii pozitii. In cazul
in care nodul in care doresc sa ma mut a fost deja vizitat, nu mai continui 
procesarea, trecand la urmatorul nod din coada. In caz contrar, formez noul nod
pornind de la cel din care am plecat si setand corespunzator parametrii 
acestuia. Pasul urmator este sa verific daca un nod cu aceeasi configuratie 
exista deja pentru nodul curent. Daca exista unul cu cost mai mare, se 
inlocuieste cu nodul curent, dupa care se trece la procesarea urmatorului nod 
din coada. Daca nu exista duplicate, se salveaza nodul nou format in matricea 
3-D de DiceStatus si se incrementeaza numarul de posibilitati gasite pentru 
nodul acesta. Daca destinatia nu a fost gasita, se adauga acest nod in coada 
pentru a fi procesat.
Gasirea costului minim se reduce la a gasi costul minim dintre toate 
posibilitatile gasite pentru nodul reprezentat de indicii de final. Aceasta 
valoare este scrisa in fisierul de output.

Complexitatea algoritmului este O(numar_noduri + numar_muchii) deoarece, spre 
deosebire de exemplul prezentat in documentatia laboratorului, extragerea din 
coada se face in O(1).
Numarul de noduri este reprezentat de numarul total de elemente, adica 
linii*coloane, iar numarul de muchii este egal cu 
(linii - 1)*coloane + (coloane - 1)*linii.