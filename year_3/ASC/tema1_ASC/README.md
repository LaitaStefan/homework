Papa Florin, 334CA
Tema 1 ASC - Simularea comportamentului unui cluster

This homework implements the gaussian elimination algorithm using a thread pool
mechanism in Python.


Pentru implementarea temei, am pornit de la scheletul de cod pus la dispozitie,
in care am implementat functiile lipsa. De asemenea, am folosit modulul
barrier.py din cadrul laboratorului 3.

Am testat tema local(Core 2 Duo de 2.2Ghz, Ubuntu 12.04 LTS) si pe 
fep.grid.pub.ro si trec toate testele.

In functia __init__ a clasei node, am adaugat cateva campuri utile pentru restul
implementarii: barrier - bariera pentru sincronizare in algoritmul de eliminare
gaussiana, worker - thread care efectueaza eliminarea gaussiana, available - 
Lock care blocheaza accesul la rezultatul nodului curent pana cand acesta
devine disponibil, result - rezultatul, dataWorkers - lista cu thread-uri care
proceseaza request-uri, dataHandler - thread care gestioneaza request-uri,
elem_kk - pivotul, elem_kb - elementul aferent de pe coloana B.

DataHandler-ul este initializat imediat dupa initializarea datastore-ului,
impreuna cu elem_kk si elem_kb. In functia set_nodes, nodul cu indicele 0
instantiaza o bariera si o transmite tuturor celorlalte noduri. Functia
get_x porneste thread-ul worker si asteapta terminarea lui, dupa care intoarce
rezultatul. Functiile getElementAt si putElementAt trimit request-uri catre
dataHandler. In shutdown se opresc toate thread-urile de date.

Modulul threads.py contine toate clasele care extind Thread, dar si o clasa
Request, in care se stocheaza datele necesare unui request. DataHandler-ul
contine un obiect Queue, in care se pun request-urile, urmand a fi preluate de
thread-urile care se ocupa cu procesarea acestora. Metoda getElementAt se 
blocheaza pana cand rezultatul este disponibil prin intermediul unui obiect
Event din clasa Request, pe care se face wait pana la obtinerea rezultatului.

Clasa WorkerThread realizeaza eliminarea gaussiana. Pentru implementarea
algoritmului am folosit functiile my_max(identifica indicele unui rand inferior
care are elementul de pe pozitia k nenul) si my_swap(inverseaza doua noduri).
Functia getValue calculeaza rezultatul pentru nodul curent, care depinde de
rezultatele anterioare. Pentru a ma asigura ca nu se incearca folosorea unor
rezultate inexistente, am folosit un Lock in fiecare nod care determina daca
rezultatul sau este valabil. Fiecare nod il asteapta pe cel de dupa el sa obtina
rezultatul inainte de a calcula rezultatul propriu. Functia gauss realizeaza
eliminarea gaussiana. Este nevoie de o bariera dupa ce se face swap acolo unde
este necesar si dupa incheierea unei iteratii.