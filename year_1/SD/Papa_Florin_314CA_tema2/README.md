Papa Florin
314 CA

This homework implements a SkipList data structure for performing different 
operations on a input text. Every word in the text is stored in the SkipList
with its frequency.


Pentru implementarea structurii de date Skiplist, am pornit de la structura 
Node, care contine "inaltimea" nodului(numarul de niveluri), frecventa 
aparitiei tokenului pe care il contine, tokenul propriu-zis(char*) si un vector
de pointeri de tip Node(care pointeaza la nodurile urmatoare din lista in 
functie de nivelul pe care se afla).
Skiplist-ul contine doar un pointer Node *pFirst si dimensiunea listei(numarul 
de elemente). Nodul pFirst are doar rol de referinta, el nu contine date si are
inaltimea maxima(din motive de eficienta, am ales-o 16). Operatiunea de baza 
din cadrul Skiplist-ului este functia search, pe care se bazeaza toate 
celelalte functii. Ea primeste ca parametru un char* si returneaza nodul 
imediat anterior (de pe nivelul 0) celui unde ar trebui sa se afle vectorul de 
caractere. In functiile insert si sterg, algoritmul de cautare stocheaza si un 
vector de pointeri de tip Node de dimensiune MAX (16) care retine nodul imediat
anterior celui cautat pe fiecare nivel. Astfel, la inserare si stergere pot fi 
refacute legaturile cu celelalte noduri. 
In plus, am adaugat functiile decreaseFrecv, getFrecv, setFrecv, getData si 
getSize. Functia getFrecv returneaza frecventa unui string de caractere primit 
ca parametru, decreaseFrecv scade cu 1 frecventa unui char* primit ca 
parametru, setFrecv seteaza frecventa unui char* la o valoare primita ca 
parametru. Functia getData returneaza un vector cu toti pointerii de tip Node 
din Skiplist pentru a le afisa datele.

In main am definit o structura "cheie" care va retine toate partile de vorbire 
care apar. Va fi alocat la inceput un vector de 10 astfel de structuri, dupa 
care, la nevoie, va fi realocat cu cate 10. Am definit functia getInd care 
returneaza indicele la care se afla o anumita parte de vorbire sau valoarea -1 
in cazul in care aceasta nu exista in vector. Functia turn2char transforma o 
variabila de tip std::string intr-una de tip char*.

In continuare, se deschide fisierul de intrare al carui nume este primit ca 
parametru al liniei de comanda, dupa care se deschid fisierul corpus si 
fisierul de output, ale caror nume se afla in fisierul de intrare si se citeste
si numarul de comenzi ce urmeaza a fi primite. 
Urmatorul pas este parsarea textului din fisierul corpus si introducerea 
tokenilor in Skiplist-urile corespunzatoare.

In acest moment, se poate trece la procesarea comenzilor primite, care 
efectueaza modificarile asupra Skiplist-urilor si produc output-ul.

Cel mai dificil aspect al acestei teme a fost implementarea Skiplist-ului 
deoarece nu reuseam sa implementez corect algoritmul se search. In momentul in 
care am reusit sa trec peste acest prag, au mai aparut probleme(multe 
segmentation fault), dar au fost mult mai usor de rezolvat decat prima parte.  