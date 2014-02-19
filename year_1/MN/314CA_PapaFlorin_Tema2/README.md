Papa Florin
314CA

This homework is divided into two parts. The first one is to solve a liniar
equation system using LU factorng methods(Crout, Doolittle). The second part
consists of correcting a one-bit error using a Hamming code.

Pentru Task 1.1 si 1.2, am parcurs matricea data ca parametru pe linii, 
respectiv pe coloane, retinand elementele nenule, precum si elementele necesare
pentru vectorii rowind, colind, rowptr si colptr.

Pentru Task 1.3 si 1.4 am modificat algoritmul clasic de rezolvare a sistemelor
triunghiulare pe linii, respectiv pe coloane pentru a parcurge doar elementele 
nenule ale matricii date ca parametru. Functiile triCL si triCC rezolva sisteme
inferior triunghiulare. Astfel, pentru o linie oarecare i, parcurgem elementele
rowptr(i):rowptr(i+1)-1, care reprezinta indicii din nzval al elementelor 
nenule de pe randul i al matricii initiale.

Pentru Task 1.5, in urma studiului efectuat, am ajuns la concluzia ca pot fi 
folosite atat metode iterative, cat si metode directe pentru rezolvarea 
eficienta a unui sistem de ecuatii cu matricea coeficientilor rara. In cazul in
care matricea este simetrica si pozitiv definita, cea mai eficienta metoda s-a 
dovedit a fi Cholesky deoarece descompune  matricea initiala intr-un produs 
dintre o matrice inferior triunghiulara si transpusa sa. Astfel , raman de 
rezolvat doua sisteme triunghiulare, unul superior, altul inferior, ceea ce se 
rezolva usor.
In cazul in care matricea nu indeplineste aceste conditii, se pot aplica alte 
metode de factorizare LU (Crout, Doolittle), dar matricele L si U rezultate pot
deveni dense, in functie sistemul de ecuatii considerat. Din aceasta cauza, in 
unele situatii, mai eficiente se pot dovedi metodele iterative(Gauss-Seidel, 
Jacobi), in cazul in care matricea este convergenta.

In ceea ce priveste partea a doua, pentru task 2.1 se transforma sirul primit 
ca string intr-un vector. Am definit functia putere2, care verifica daca un 
numar este putere a lui 2. Pentru a determina lungimea noului sir, verificam 
cate numere puteri ale lui 2 mai mici decat lungimea sirului care trebuie 
transformat in cod Hamming exista. Adaugam acest numar la lungimea sirului 
initial, dupa care verificam daca noua lungime admite un nou bit de paritate, 
pe care il adaugam daca este necesar. In continuare, se parcurge vectorul 
obtinut si se stabilesc bitii de paritate. In final, transformam vectorul 
inapoi in string.

Pentru Task 2.2 se salveaza intr-un vector bitii de paritate ai stringului 
primit ca parametru. In continuare, se creeaza un vector cu bitii de paritate 
pe care ar trebui sa-i aiba stringul si se compara cu cei actuali. Pozitia 
bitului gresit o reprezinta suma din 2 la puterea indicilor bitilor eronati. 
La final, se inverseaza bitul gresit. 