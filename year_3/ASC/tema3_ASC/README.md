Papa Florin, 334CA
Tema 3 ASC 2014

This project implements image compressing/decompressing using the IBM CELL
processor.


Pentru a rula toate imaginile de test si pentru toate variantele de numere de
SPU, se apeleaza script-ul "run_all.sh"(s-ar putea sa fie nevoie de a se seta
drepturi de executie pe toate script-urile din folder-ul principal). Rularea
script-ului va scrie timpii de rulare pentru fiecare test in fisierul
"run.sh.o----" si imaginile de output. Imaginile fara double buffering au in
componenta numelui cifra 0.

Pentru implementarea temei, am pornit de la laboratorul 9, cel cu double
buffering.

Pentru partitionarea pozei, am impartit imaginea in grupuri de cate 8 linii,
dupa care am impartit aceste grupuri in mod egal la primele (n - 1) SPU-uri,
ultimul SPU fiind responsabil cu restul datelor, intrucat imaginea poate sa nu
se imaprta exact la toate SPU-urile. Structura de control se numeste my_img,
se afla in fisierul btc.h si contine latimea imaginii, inaltimea pe care o are
fiecare SPU de procesat, locatiile de memorie unde trebuie sa scrie blocurile,
respectiv pixelii decomprimati, precum si modul de operare(normal sau double
buffering).

Structura de control este initializata in functia initialize_arg, iar adresa ei,
precum si dimensiunea, sunt trimise ca argumente SPU-urilor.

SPU-urile isi iau prin DMA continutul structurii de control. In cazul in care 
modul de operare este simplu, se determina numarul de blocuri, se transfera
bloc cu bloc imaginea, se compreseaza, se decompreseaza si se trimit rezultatele
inapoi la PPU.

In cazul in care avem double buffering, se transmite primul bloc, dupa care se
cere transmiterea urmatorului bloc si se incepe procesarea primului bloc. La 
final, se receptioneaza si se proceseaza ultimul bloc.

Instructiunile vectoriale sunt folosite in functia de comprimare pentru a obtine
media unui bloc. Fiecare linie din bloc reprezinta un vector, fiind foarte usor
sa adunam toate elementele intr-un alt vector. La final, se aduna elementele din
vectorul rezultat si se imparte la 64 pentru a afla media. Pentru a afla
deviatia standard, nu am folosit instructiuni vectoriale deoarece rezultatul
calculului este un float, iar elementele sunt short int, iar la fiecare pas
trebuia sa scadem media(care este float) si sa ridicam la patrat. Daca as fi
trunchiat media la short int, as fi avut pierderi de precizie.

Rezultatele rularii pentru imaginea de intrare /export/asc/btc_input/in3.pgm,
fara double buffering, sunt:
	- 1 thread: 
			compresie + decompresie: 	1.88 s
			total:						2.71 s
	- 2 thread-uri:
			compresie + decompresie: 	0.94 s
			total:						1.71 s
	- 4 thread-uri:
			compresie + decompresie: 	0.64 s
			total:						1.30 s
	- 8 thread-uri:
			compresie + decompresie: 	0.33 s
			total:						1.23 s

Rezultatele rularii pentru imaginea de intrare /export/asc/btc_input/in3.pgm,
fara double buffering, sunt:
	- 1 thread: 
			compresie + decompresie: 	1.41 s
			total:						2.06 s
	- 2 thread-uri:
			compresie + decompresie: 	0.71 s
			total:						1.35 s
	- 4 thread-uri:
			compresie + decompresie: 	0.35 s
			total:						1.11 s
	- 8 thread-uri:
			compresie + decompresie: 	0.22 s
			total:						0.88 s

Se poate observa faptul ca operatiile de input/output au o durata constanta de
aproximativ 0.85 secunde, care se adauga la timpul de procesare pentru a obtine 
timpul total. Pentru rularile fara double buffering, se poate observa o
imbunatatire liniara a performantei odata cu cresterea numarului de SPU-uri.

Pentru varianta cu double buffering, rezultatele sunt notabil mai bune decat
pentru varianta anterioara. Se obtine un speedup de pana la 40% pentru rularea
cu 4 thread-uri. Si aici, performanta creste liniar cu numarul de SPU-uri.
