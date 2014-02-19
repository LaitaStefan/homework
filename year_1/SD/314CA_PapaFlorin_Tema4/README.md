Papa Florin
314 CA

This homework implements a task scheduler using a Graph data structure. The 
task scheduler works on a number of processors larger than 1(unlike the task
scheduler implemented for homework 1).

Pentru rezolvarea temei de casa 4, am folosit clasa Graph, care implementeaza 
structura de graf. Pentru reprezentarea dependintelor am folosit o matrice de 
adiacenta(numita A in clasa). Pe langa matricea de adiacenta, clasa mai contine
numarul total de noduri(variabila N), un vector care retine durata de rulare a
fiecarui proces(vectorul durata) si un vector care arata daca un proces este in
rulare sau nu(vectorul running).
Asupra acestei clase am definit mai multe operatii. Constructorul primeste ca 
parametru numarul de noduri ale grafului si aloca matricea A si vectorii 
running si durata. Destructorul dezaloca memoria alocata anterior. Functia 
"addEdge" adauga o latura noua in matricea de adiacenta, in functie de datele 
citite din fisierul de intrare. Functia "addNodeDurata" seteaza durata unui nod
primit ca parametru la o valoare data. Functia "removeNode" este apelata in 
momentul in care un anumit proces a ajuns sa aiba timpul de rulare 0, deci si-a
terminat executia. Pentru a marca procesul ca finalizat, se seteaza elementul 
diagonal al unui anumit nod la valoarea -1. Functia "decreaseDurata" scade cu o
unitate durata unui proces primit ca parametru. In cazul in care durata 
rezultata este 0, este marcat procesul ca si finalizat. Functia "getVectReady" 
intoarce un vector de dimensiune primita ca parametru, care contine indicii 
proceselor care sunt gata de rulare(nu au -1 pe diagonala principala, nu sunt 
marcate ca running si nu au dependinte). Un proces care nu are dependinte are 
pe coloana aferenta numai elemente nule. Functia "getDurata" returneaza durata
unui proces primit ca parametru. Functia "setRunning" marcheaza un anumit 
proces ca fiind in executie, setand valoarea aferenta din vectorul running la 
1. Functia "isEmpty" returneaza valoarea 0 daca graful contine elemente care nu
si-au terminat executia si 1 in caz contrar.

In clasa Scheduler am implementat functiile necesare rezolvarii temei. 
Constructorul acestei clase initializeaza valorile N(numarul de noduri ale 
grafului) si nrProc(numarul de procesoare), precum si 2 variabile de tip FILE*
(fisierele de intrare). Functia "test4number" intoarce 0 daca un sir de 
caractere primit ca argument nu contine nicio cifra si numarul de cifre 
continute in caz contrar. Functia "readNodeFile" citeste fisierul care contine
dependintele si initializeaza matricea de adiacenta a grafului primit ca 
parametru. Citirea din fisier se face linie cu linie, apoi se verifica daca o 
linie citita contine cifre. Daca nu contine, nu se efectueaza nicio operatie 
asupra grafului, iar in caz contrar se separa numerele de pe linie folosind 
strtok, se transforma in int(folosind atoi) si se completeaza matricea de 
adiacenta in functie de valorile citite.

Functia "readTimeFile" efectueaza citirea fisierului care contine durata de 
rulare a fiecarui proces. Din nou, citirea se face linie cu linie, se 
transforma rezultatul din char in int(folosind atoi) si se introduce valoarea 
citita in vectorul durata din graf.

Functia "PlanProcess" realizeaza planificarea proceselor din graf. Vectorul 
proc retine starea fiecarui procesor(daca este liber, are valoarea -1 la 
indicele corespunzator, iar daca nu este liber are un numar nenul care 
reprezinta indicele unui anumit proces), vectorul plan retine indicele unui 
procesor pe care s-a facut o noua planificare la un anumit moment de timp, iar
vectorul typeProc retine indicele procesului planificat. Pentru planificare, se
apeleaza functia getVectReady din clasa Graph, care returneaza un vector cu 
procesele gata de rulare. In continuare, se parcurge vectorul proc, iar pe 
pozitiile unde are valoarea -1 se planifica procesele gata de rulare, 
salvandu-se parametrii acestora in vectorii plan si typeProc pentru afisare. 
In cazul in care un procesor nu este liber, se decrementeaza timpul de rulare 
al procesului care ruleaza. In cazul in care timpul a ajuns la 0, se marcheaza
procesul ca finalizat si se elibereaza procesorul. Acest lucru se face intr-un
while, cat timp graful contine procese care nu  si-au incheiat executia. La 
finalul fiecarui ciclu se afiseaza procesele planificate la acel moment de timp
si se incrementeaza timpul.

In main se deschid fisierele de input, se citesc numarul de procese, respectiv 
de procesoare, se apeleaza functiile descrise anterior(citire din fisiere, 
planificare), dupa care se inchid fisierele.

Mentionez ca eu realizez numerotarea nodurilor si a procesoarelor de la 0, nu 
de la 1, asa cum se face in enunt. Pentru a obtine rezultate corecte, valorile
citite din fisier(indicii nodurilor) sunt micsorate cu 1, iar afisarea
(outputul) se face adaugand 1 la valoarea efectiva.     