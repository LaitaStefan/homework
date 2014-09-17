Papa Florin, 334CA
Tema 2 ASC

This project implements the dtrmv function for multiplying an upper/lower
diagonal matrix with a vector. The program was run on Nehalem, Opteron and
Quad processors and the results were compared with the ATLAS implementation
of the function (plots are available in the project folder).


Pentru rezolvarea temei, am implementat functia dtrmv, care inmulteste o matrice
superior/inferior triunghiulara cu un vector. Implementarea de baza se afla in
fisierul "dummy_dtrmv.c" si reprezinta o implementare naiva de inmultire.

Pentru a obtine un grafic, se ruleaza run.sh.

Pentru punctul 2 al temei, am folosit rutina cblas_dtrmv din ATLAS-BLAS. Pentru
a o putea folosi, am inclus header-ul "cblas.h". La compilare am inclus 
bibliotecile ATLAS si CBLAS(-latlas -lcblas) specifice fiecarui procesor
(Opteron, Nehalem si Quad).

Punctele 3 si 4 sunt facute impreuna. Pentru punctul 4, am optimizat varianta
naiva de inmultire, ignorand elementele nule de deasupra/dedesubtul diagonalei
principale, folosirea unei variabile registru pentru adunare si folosirea de
pointeri pentru adresarea matricii si a vectorului. Toate rezultatele - naiv, 
folosind ATLAS si imbunatatit - sunt afisate in graficele incluse in arhiva.
Am testat pe matrici de 25000, 30000 si 35000 de elemente, iar rezultatul este
cel asteptat: varianta naiva se comporta cel mai prost, cea imbunatatita ia de
doua ori mai putin decat cea naiva, iar ATLAS dtrmv se comporta cel mai bine, 
insa nu cu mult mai bine decat varianta imbunatatita.

Testele au fost facute pe matrici si vectori initializate cu 1 deoarece nu
intereseaza rezultatul, ci timpul rularii rutinei(s-a testat corectitudinea
inmultirii in prealabil). Se masoara numai timpul de rulare a rutinei, nu sunt
incluse si initializarile.

Programele primesc dimensiunea problemei in linie de comanda. Script-ul run.sh
ruleaza exemplele si obtine graficele(stocate intr-o imagine numita 
performance.png). Script-ul ruleaza destul de mult, in functie de gradul de
incarcare al cluster-ului. Trebuie ca script-ul sa fie lasat sa isi termine 
executia pentru a putea obtine rezultatul.

Se poate observa din grafice faptul ca, pe toate procesoarele, ierarhia
timpulor de rulare este aceeasi, de la cel mai prost la cel mai bun: varianta
"de mana" neoptimizata, varianta "de mana" optimizata si ATLAS dtrmv. Timpii de
rulare pentru quad si opteron sunt asemanatori, cu un mic avantaj pentru quad
in cazul variantei neoptimizate si invers pentru cele optimizate. De departe,
timpii de rulare pentru nehalem sunt cei mai buni, fiind la jumatate fata de 
celelalte arhitecturi: neoptimizat - 0.8s fata de 1.8s(opteron), ATLAS 
dtrmv - 0.3s fata de 0.5s(opteron), optimizat - 0.4s fata de 0.7s(opteron). 

Comparand specificatiile celor 3 procesoare, se observa diferenta mare de
memorie cache intre Opteron(L2 cache de 3MB) si Nehalem sau Quad(L2 cache de 
12MB). Din cauza aceasta, Opteron se comporta cel mai slab in cazul programului
neoptimizat. Nehalem este de departe cel mai bun, cu un cache de aceeasi 
dimensune cu cea a lui Quad, insa Quad se comporta mai slab din cauza
magistralei de date mai lente. Magistrala de date lenta face ca procesorul
IBM Quad sa piarda teren in fata lui Opteron in cazul testelor optimizate,
unde procesarea datelor este mai rapida, iar timpul de transfer pe
magistrala devine semnificativ.
