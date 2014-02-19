Papa Florin
314 CA

This homework contains an implementation of the Page Rank Algorithm. The 
algorithm is implemented in two ways: Iterative and Algebraic.

Tema a fost rezolvata in Octave 3.2.4 in Ubuntu.

Pentru cerinta 1 a temei(algoritmul Iterative), am folosit algoritmul pus la 
dispozitie pe site-ul Wikipedia. Dificultatea acestei cerinte a constituiut-o, 
pentru mine, citirea din fisier, formarea matricei de adiadenta, formarea 
vectorului L(numarul de noduri la care pointeaza un anumit nod din graf) si 
formarea matricii M din algoritm. Din momentul in care mi-am dat seama exact 
ce am de facut, lucrurile au devenit mult mai simple. Citirea din fisier am 
facut-o cu fgets, linie cu linie, transformand elementele obtinute din string 
in valori numerice. Am salvat toate elementele citite din fisier intr-un 
vector, retinand in acelasi timp numarul de elemente ale fiecarei linii. Cel 
de-al doilea element al fiecarei linii contine numarul de elemente de pe linia 
respectiva, deci vectorul L il reprezinta a doua coloana a matricii din fisier.
In cazul in care exista un element care pointeaza catre el insusi, scadem L cu 
o unitate pentru linia aferenta elementului. Pentru formarea matricii de 
adiacenta, am parcurs matricea pe linii, egaland fiecare element care are 
indicele coloanei egal cu un element din fisier cu 1. La fiecare pas, egalam 
elementul diagonal cu 0(asa este specificat in cerinta). Pasul urmator il 
constituie formarea matricii M(din algoritm), lucru facut conform 
instructiunilor din tutorial(M(i,j)=0 daca j nu pointeaza catre i si 
M(i,j)=1/L(j) daca j pointeaza catre i). Pasul final il constituie aplicarea 
algoritmului propriu-zis si obtinerea vectorului PR(iteratiile se fac atata 
timp cat norma diferentei dintre doi vectori succesivi obtinuti este mai mare 
decat un numar dat eps).

Cerinta 2 a temei(algoritmul Algebraic) este foarte asemanatoare cu cerinta 1, 
in afara algoritmului final de obtinere a vectorului PR. In plus fata de 
cerinta 1, pentru a obtine o precizie cat mai mare a algoritmului, am folosit 
functiile Gram.m si invTrSup.m. Gram.m obtine descompunerea unei matrici 
intr-un produs dintre o matrice ortogonala si o matrice superior triunghiulara.
Astfel, aceasta matrice este mult mai usor de inversat, fiind produsul dintre 
matricea superior triunghiulara inversata si matricea ortogonala transpusa. 
Functia invTrSup.m inverseaza o matrice superior triunghiulara.

Pentru cerinta 3, am folosit functiile suplimentare getVal.m, sort.m si u.m. 
Functia getVal.m returneaza cele doa valori de pe ultimele doua linii ale 
fisierului pe baza carora este formata functia u.m. Pentru ca functia u sa fie 
continua, trebuie ca valoarea sa in val1 sa fie 0, iar in val2 sa fie 1. 
Astfel, am obtinut valorile parametrilor a si b ai functiei. Functia sort.m 
intoarce vectorul PR(primit ca argument) sortat si un vector care retine 
pozitia fiecarui element din vectorul initial in vectorul sortat. 

!	Precizez ca in primul exemplu de test oferit in enunt sunt doua valori 
identice care in fisierul de output au ordinea inversata. Algoritmul de sortare
folosit de mine pastreaza ordinea initiala a doua elemente identice. 

Pentru obtinerea fisierului de output am concatenat numele fisierului de input 
cu ".out" si l-am deschis pentru scriere. Prima linie scrisa este aceeasi cu 
cea a fisierului de input. Urmatoarele 2 campuri introduse in fisier le 
reprezinta vectorii obtinuti cu algoritmii Iterative si Algebraic. In final, 
am introdus un "clasament" al paginilor, primul element continand pozitia in 
clasament, cel de-al doilea pozitia in vectorul PR, iar cel de-al treilea 
valoarea functiei u in punctul dat de vectorul PR.