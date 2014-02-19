Papa Florin, 324CA

This homework contains the implementation of an algorithm that changes the 
numbers in a matrix according to its neighbours(symilar to "Game of Life"). 
Three different implementations are provided: serial unoptimised, serial 
optimised and parallel optimised. The aim is to see how much we can improve the
algorithm by making different changes. The implementation is made in C, and for
making the parallel version I used "pragma omp parallel".

Performance:
unoptimised:  	2 min 28 sec
1 thread: 		25.77 seconds with default scheduling/chunk
2 threads:		14.10 seconds(speedup 1.54) with schedule(static, 1) or 
				(dynamic, 1)
4 threads: 	7.64 seconds(speedup 2.86) with schedule(static, 20) or default
8 threads:	6.84 seconds(speedup 3.18) with schedule(static, 1)


In cadrul temei am implementat cele 3 variante:seriala(serial.cpp), seriala 
optimizata(serial_optim.cpp) si paralela(paralel.cpp).

In cadrul fisierului Makefile, regula build_serial obtine executabilul pentru 
sursa seriala neoptimizata(numit serial), build_optim obtine executabilul pentru
sursa seriala optimizata(numit serial_optim), iar build_paralel obtine
executabilul pentru sursa paralela(numit paralel). Regula clean sterge toate
executabilele.

In cadrul variantei seriale, pentru fiecare element al matricii
parcurg din nou matricea pentru a calcula distantele minime catre fiecare 
culoare. Tabloul distantelor este initializat cu valoarea N(dimensiunea 
matricii) deoarece nicio distanta nu poate aceasta valoare, asadar orice
distanta gasita o inlocuieste in vector. Distanta curenta este calculata cu 
formula din cerinta temei(max(abs(i-i1), abs(j-j1))). In cazul in care este 
diferita de 0(ma aflu pe elementul curent) si mai mica decat valoarea din
vectorul de distante, o salvez. Dupa ce am parcurs toata matricea, vectorul
de distante este complet si pot extrage din el valoarea maxima, al carei indice
va reprezenta noua culoare din matrice. Noua valoare este stocata intr-un tablou
auxiliar, iar dupa ce calculam toate valorile o copiem in matricea originala.
In momentul in care am gasit o noua culoare, o contorizez in vectorul color_nr, 
initializat la inceputul fiecarei saptamani cu 0. Astfel, nu este necesara inca 
o parcurgere pentru determinarea numarului fiecarei culori.

Functiile auxiliare folosite sunt get_max(care intoarce indicele valorii maxime
din vectorul de distante), print_color_numbers(care scrie in fisierul de output
numarul fiecarei culori la finalul saptamanii) si initializare(care 
initializeaza vectorul de distante cu valoarea N).

Varianta seriala are complexitate O(nr_saptamani * n^4), iar timpul de rulare
pentru exemplul de 100 de saptamani si matrice 100x100 este de 2 minute si 28
de secunde.

In varianta seriala optimizata, cautarea culorilor se face concentric, cautand
in jurul punctului de interes pe niveluri. Variabila elim retine cate colori
au fost eliminate pentru a nu le cauta degeaba. Vectorul de distante se
initializeaza tot cu valoarea N, iar atunci cand gasesc o culoare la distanta
mai mica decat N, o salvez si nu mai poate fi inlocuita deoarece se afla la
distanta minima reprezentata de nivelul pe care ma aflu. Pentru a cauta in zone 
concentrice, folosesc 4 for-uri(pentru laturile sus, jos, stanga si dreapta).
Limitele de cautare variaza in functie de nivel si sunt calculate o fingura data
inainte de a intra in bucle. In cazul in care am gasit toate culorile, se iese
din bucla for si se seteaza flag-ul finish pe 1 pentru a nu se cauta mai
departe. Mai departe, totul se desfasoara ca la varianta neoptimizata.

Varianta seriala optimizata are timpul de rulare pentru exemplul de 100 de 
saptamani si matrice 100x100 de 21.80 de secunde. Pe sistemele din laboratorul
de APD rularea acestui test dureaza 13.311 secunde.

In cadrul variantei paralele, am paralelizat calculul noilor culori ale 
senatorilor. Variabilele shared sunt reprezentate de tablourile definite in 
afara buclelor, iar variabilele private sunt nedeclarate intrucat sunt definite
in interiorul regiunii paralele, asadar sunt private de la natura. Spre
deosebire de varianta seriala optimizata, numararea senatorilor din fiecare 
culoare se face separat deoarece accesul concurent la vectorul continand numarul
din fiecare culoare produce neconcordante.Asadar, dupa calculul noii matrici,
intr-un for paralelizat, se calculeaza numarul din fiecare culoare(incrementarea 
se face intr-o regiune critical pentru a nu se produce erori). Apoi, 
intr-un alt for paralelizat, se copiaza noua matrice in matricea originala.

Varianta paralela are timpul de rulare pentru exemplul de 100 de 
saptamani si matrice 100x100 de 14.22 de secunde(2 threaduri si scheduling/chunk
automate), obtinand un speedup de 1.52.

PENTRU TASK 3:
Toate testele au fost rulate pe exemplul cu N=100 si nr_saptamani=100.
1 thread: 		25.77 secunde cu scheduling/chunk automate
2 threaduri:	14.10 secunde(speedup 1.54) cu schedule(static, 1) sau 
				(dynamic, 1)
4 threaduri: 	7.64 secunde(speedup 2.86) cu schedule(static, 20) sau automat
8 threaduri:	6.84 secunde(speedup 3.18) cu schedule(static, 1)

Am efectuat teste cu schedule static/dinamic si chunk variind intre 1, 10, 20, 
precum si cu valorile automate(nespecificate), iar timpii(pentru fiecare numar
de threaduri) au fost foarte apropiati, observandu-se o tendinta de marire
(foarte subtila) o data cu cresterea chunk-ului.

Testele au fost efectuate pe un laptop cu procesor Intel Core i7-2630QM, 
4 core-uri, 8 threaduri hardware, 2.00GHZ, 6144KB cache, 6GB RAM. Sistemul de 
operare este Ubuntu 12.04, iar compilatorul este g++ Ubuntu/Linaro 
4.6.3-1ubuntu5. 