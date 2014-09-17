Papa Florin, 334CA
Tema 4 ASC

This project implements functions that calculate the convolution matrix for a
given matrix using the nVIDIA CUDA processor. The algorithm is implemented
with and without double buffering and the results can be seen in the
"results.jpg" picture included in the project.


Fisierul Makefile contine o regula de build, care obtine executabilul a.out. De
asemenea, mai contine si o regula plot, care obtine un grafic al timpilor de
rulare, salvat in fisierul performance.jpg.

Pentru implementarea temei, am pornit de la scheletul de cod pus la dispozitie.
Am modificat header-ele functiilor ConvolutionOnDevice si
ConvolutionOnDeviceShared pentru a primi si un parametru de tip FILE*, util
pentru a scrie timpii de rulare intr-un fisier text. Acest fisier se numeste
time_out.txt si este folosit pentru a produce un plot cu toti timpii de rulare
de la toate testele. Timpul de rulare serial este truncat la 2.5ms deoarece
este foarte mare in comparatie cu cel obtinut pe GPU, afectand claritatea
graficului. Timpii originali sunt salvati in time_out_backup.txt.

Pentru varianta fara memorie partajata, am ales o dimensiune a blocului de
16 x 16. In functia ce calcul a convolutiei, se afla indicii fiecarui thread
si, daca acesta se gaseste printre indicii matricii de intrare, efectueaza
calculele necesare. In cazul in care, la calculul convolutiei, un element
se afla in afara matricii originale, acesta nu se ia in considerare.

Pentru varianta cu memorie partajata, este putin mai complicat. Am ales o
dimensiune a blocului de 16 x 16. Pentru a utiliza cat mai eficient mecanismul
de memorie partajata, fiecare bloc stocheaza atat matricea kernel, cat si
un bloc 16 x 16 din matricea originala, bordat cu cate 2 randuri si 2 coloane
pe fiecare parte. Bordarea este necesara pentru a nu mai accesa memoria globala
in cazul in care avem nevoie de elemente din afara celor din blocul curent de
16 x 16. Asadar, in memoria shared, tinem o matrice de 5 x 5(kernel) si una de
20 x 20(bloc bordat). Pentru a borda, in cazul in care elementele necesare
exista, se iau din memoria globala, altfel se seteaza pe 0. Thread-urile care
se ocupa cu elementele de pe margine au responsabilitatea de a efectua si
bordarea. Astfel, calculul fiecarui element se face fara a mai considera
cazurile in care se iese din matrice. 

Am ales astfel dimensiunea blocului partajat din matricea originala datorita
dimensiunii blocului de thread-uri. Fiecare bloc este limitat la 512 thread-uri
(16 * 16 * 2). Fiecare thread poate, astfel, sa proceseze cate un element, iar
impartirea sarcinii de lucru este mai intuitiva.

In ceea ce priveste performantele, acestea sunt foarte clar prezentate in
imaginea performance.jpg. In fiecare caz, cu exceptia testului 1(matrice 5 x 5),
varianta care foloseste shared memory are cele mai bune performante, urmata
indeaproape de varianta fara shared memory, iar pe ultimul loc se afla varianta
de pe CPU, cu performante foarte slabe(timpi de aproximativ 100 de ori mai mari
decat GPU cu shared memory). Exceptie face testul 1, in care varianta seriala se
comporta cel mai bine, din cauza faptului ca dimensiunea problemei este foarte
mica si devine semnificativ timpul de initializare a thread-urilor. Se poate
observa in imagine ca timpii cresc liniar cu dimensiunea problemei pentru toate
cele 3 variante, pastrandu-se "clasamentul". In medie, timpul fara shared memory
este de 1.5 ori mai slab decat cel cu shared memory.

Timpul obtinut pe CPU este truncat la 2.5ms pentru a obtine grafice cat mai
clare. Timpii originali se afla in time_out.txt.

La rularea run_tests.sh, tema trece toate testele. La rularea scriptului de
test, in functie de incarcarea cluster-ului, poate dura mai mult obtinerea
output-ului.
