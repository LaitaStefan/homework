Papa Florin
314 CA

This homework implements a process scheduler using double linked lists. Every 
process is described by PID, start time, duration and priority. Every process
is allowed to run continuously for three time slots before allowing another 
process to enter execution(in order to prevent entering starvation). 


Pentru rezolvarea temei am folosit 2 tipuri de cozi implementate cu ajutorul 
listelor dublu inlantuite circulare. Una dintre ele(din list.h) contine 4 
campuri(timp,durata,pid si prioritate) si este folosita pentru a salva 
procesele care urmeaza a fi rulate, iar cealalta(din list1.h) contine doua 
campuri(timp si coada) si este folosita pentru a salva semnalele ce urmeaza a 
fi trimise proceselor.

Functia newproc din main.cc adauga un nou proces in coada corespunzatoare 
prioritatii sale. Functia functionwait adauga un semnal wait in coada denumita
"wait", iar functia functsignal adauga un semnal signal in coada denumita 
"sign". Aceste 3 functii sunt folosite doar pentru a salva ceea ce urmeaza a se
executa, iar in acest stadiu nu se executa nimic. Executia proceselor incepe 
abia in functia finalize, unde sunt interpretate datele de intrare.

Functia first este folosita in finalize si returneaza indicele cozii in care se
afla procesul care urmeaza a fi executat. In cazul in care nu este gasit niciun
proces, se intoarce valoarea -1.

Variabila t retine momentul de timp curent, care se incrementeaza pas cu pas in 
functia finalize. Variabila prevpid retine pid-ul unui proces care si-a inceput
executia, iar tr timpul continuu de rulare al acestuia. In cazul in care tr 
este 0, nu exista un proces care trebuie sa-si continue executia, deci cautam 
altul nou. Daca tr este 3, se opreste executia procesului curent si se cauta 
urmatorul proces.

Pentru a rezolva cerintele temei am adaugat la list.h si la queue.h functii 
ajutatoare(denumite sugestiv getTime,getPid etc.). Functia addBetween adauga un
element in coada corespunzatoare in pozitia corecta pentru timpul sau de rulare
(de exemplu, daca intr-o coada exista numai procese cu timpul de incepere a 
rularii mai mare decat procesul care urmeaza a fi adaugat, acesta va fi adaugat
la inceputul cozii).

In functia finalize din main.cc, verificam prima data daca exista vreun semnal 
ce poate fi aplicat, deoarece acest lucru influenteaza ordinea proceselor. Daca
exista, il aplicam, dupa care il stergem din coada. Daca nu exista, nu se 
modifica nimic. La pasul urmator, verificam daca exista deja un proces in 
rulare si daca asupra acestuia se aplica un semnal wait. Daca da, il bagam in 
coada w corespunzatoare si cautam un alt proces care sa ruleze. In caz contrar,
continuam rularea procesului anterior decrementand durata sau stergandu-l din 
lista daca si-a terminat rularea. Daca exista un proces in rulare si tr==3(a 
atins limita), il stergem din capatul listei si il adaugam in pozitia corecta
folosind addBetween. In continuare, cautam urmatorul proces care trebuie sa 
ruleze. In cazul in care acesta exista, verificam daca exista semnale wait de
aplicat si procedam ca la pasii precedenti. Daca nu exista un proces ce urmeaza
a fi rulat si coada sign este goala inseamna ca s-a ajuns la finalul 
programului. Daca sign mai contine elemente, continuam rularea, marcand 
momentul cu NOP.

Ceea ce mi-a pus mari probleme este numarul mare de cazuri ce trebuiesc luate 
in considerare, intrucat de multe ori obtineam un output gresit din cauza ca 
existau cazuri netratate.

Interpretarea comenzilor se face in main folosind functii de manipulare a 
sirurilor de caractere sin string.h. Comenzile sunt citite de la tastatura pana
in momentul aparitiei comenzii FINALIZE, care produce outputul, dupa care iese 
din program.