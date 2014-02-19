Papa Florin, 324CA

This project implements a client-server model using TCP sockets. A server is 
started, that "listens" on a given port. Multiple clients can connect to the 
server and make various requests: ask for a full history of requests a client 
has made, ask for a list of connected clients, send messages to other clients
(including broadcasts), send files to other clients and ask for ending the 
connection.


Fisierul client.c :

	Fiecare client contine variabilele globale s[MAX_CLIENTS], 
connected[MAX_CLIENTS], history[50][BUFLEN], open_files[MAX_CLIENTS] si dim. 
In vectorul de structuri de tip "sending_file" s retin informatii despre toate 
fisierele in curs de trimitere. Acest lucru este necesar pentru a nu trimite un
intrec fisier deodata, fapt ce ar produce blocarea clientului. Variabilele din 
vectorul connected iau valoarea portului pe care asculta un client direct 
conectat, iar indexul fiecarei variabile reprezinta socketul pe care poate 
trimite date clientul. Variabila history retine numele fisierelor si mesajele 
primite de un anumit client, dim fiind numarul de intrari memorate in acest 
vector. Vectorul open_files contine numele tuturor fisierelor deschise la un 
moment dat, indexul fiecaruia fiind file descriptorul fisierului in cauza. 
Aceasta variabila este necesara pentru a sti in ce fisier sa scriu atunci cand
se transfera fisiere. Pentru a nu produce o blocare a clientului cand se 
transmit fisiere de dimensiuni mari, se foloseste select pentru a permite 
multiplexarea comenzilor primite.

	Functia getSendingSock intoarce valoarea socketului pe care am de trimis un
fisier. In cazul in care nu am fisiere in curs de trimitere, functia va 
intoarce -1.

	Functia indexOf primeste ca parametru o variabila de tip in_port_t si 
intoarce socketul pe care poate fi accesat clientul cu acel port in cazul 
in care exista o conexiune, iar in cazul in care nu s-a stabilit o 
conexiune intoarce -1. Functia getFd intoarce file descriptorul unui fisier
primit ca parametru daca acesta exista si -1 in caz contrar.

	Functia send_message se ocupa cu transmiterea unui mesaj de la un client la
altul. Ea primeste ca parametru socketul pe care asculta serverul, mesajul 
de trimis, variabila read_fds (care se poate modifica), adresa IP a 
clientului, numele clientului si variabila fdmax. Primul pas spre 
trimiterea unui mesaj este trimiterea unei cereri "infoclient" catre server
pentru a afla portul pe care asculta clientul catre care vrem sa trimitem. 
O data ce am aflat portul acestuia, verific daca exista deja o conexiune 
directa (vectorul connected). Daca nu exista, creez conexiunea prin 
intermediul comenzii "connect", adaugand socketul nou in multimea read_fds 
si setand o noua valoare a variabilei fdmax, daca este necesar. In 
continuare, creez un mesaj cu antetul "messageR", la care adaug numele 
transmitatorului si mesajul propriu-zis si il trimit, urmand a fi procesat 
de catre acesta.

	Functia send_file functioneaza asemanator cu cea de mai sus, cu diferenta 
ca la final se trimite un fisier intreg. Mesajele trimise au o dimensiune
maxima de 1024B, fiecare dintre aceste mesaje continand antetul "file", urmat 
de numele fisierului, numarul de secventa, dimensiunea bucatii trimise si
numele transmitatorului. La final, pentru ca destinatarul sa stie ca s-a 
incheiat transmisia, se trimite un astfel de mesaj, dar care are la dimensiune 
valoarea 0. 

	In main se creeaza conexiunea cu serverul, se initializeaza variabilele si 
se face bind pe portul primit ca parametru. In primul rand, se realizeaza 
conexiunea cu serverul. In caz de succes, se trimit datele de identificare ale 
clientului(nume, port). In cazul in care serverul are deja conectat un client 
cu acelasi nume, cel care s-a conectat mai tarziu este deconectat. Urmatorul 
pas este sa se faca bind pe portul primit ca parametru pentru a putea asculta 
cereri de conectare. Inainte de a intra in bucla while, se adauga in read_fds 
descriptorul serverului, al clientului si cel al stdin(care are valoarea 0).

	In bucla while sunt tratate mesajele primite de la tastatura, de la server 
si de la ceilalti clienti. Intai se face select, dupa care, intr-un for, se 
parcurg toti socketii pe care am putea primi date. In cazul in care acest 
socket este 0(primim comenzi de la tastatura), se citeste comanda cu ajutorul 
functiei fgets. Daca am primit comanda "listclients", se trimite mai departe 
catre server, care raspunde trimitand o lista cu toti clientii conectati in 
acel moment. Pentru comanda "infoclient" se procedeaza similar, adica se 
trimite mai departe la server, care raspunde cu datele clientului solicitat 
sau cu mesajul "Nu exista clientul cerut" in cazul in care acesta nu este 
conectat.

	Daca se primeste comanda "message", se apeleaza functia send_message 
descrisa mai sus.

	In cazul comenzii "broadcast", se trimite catre server o cerere de tip 
"listclients" pentru a obtine numele tuturor clientilor conectati. Pentru 
fiecare dintre acestia se formeaza o comanda de tip "message" si se apeleaza 
functia "send_message".

	Daca se primeste comanda quit, se trimite la server un mesaj cu antetul
"quitC" pentru a-l instiinta de iesirea clientului, se inchid socketii 
serverului si al clientului si se iese din program.

	Pentru comanda sendfile, se apeleaza functia "send_file" descrisa mai sus.

	Pentru comanda history, se parcurge vectorul history si se afiseaza fiecare
intrare.

	In cazul in care primesc ceva pe socketul pe care ascult conexiuni, se 
adauga clientul nou in read_fds si in vectorul connected.

	In cazul in care primesc ceva de la clientii direct conectati, se trateaza
mesajele in functie de antetul acestora. Daca primesc mesaj cu antetul
"messageR", se extrage din acesta numele transmitatorului si mesajul si se 
afiseaza, impreuna cu ora la care a fost primit acesta(timestamp). In cazul 
antetului "quit"(primit numai de la server), se procedeaza identic cu comanda 
"quit" primita de la tastatura. Comanda "nameConflict" este primita in cazul in
care exista un conflict de nume cu un client deja conectat, iar rezultatul este
inchiderea conexiunii clientului mai nou. Comanda "file" semnaleaza primirea 
unui mesaj ce contine o bucata de fisier. In cazul in care numarul de secventa 
al fisierului este 0, se modifica numele fisierului conform cerintei(se adauga 
la final "_primit") si se deschide pentru a scrie in el. Pentru a nu bloca alte
transmisiuni, se salveaza bucata primita, salvand file descriptorul si numele 
fisierului pentru scrieri ulterioare. Finalul transmiterii unui fiser se 
detecteaza atunci cand se primeste un mesaj cu dimensiunea payload-ului 0, 
moment in care se inchide fisierul si se scoate din vectorul de fisiere 
deschise.


Fisierul server.c :

	Serverul contine variabilele address[MAX_CLIENTS], 
clients_names[MAX_CLIENTS][15], ports[MAX_CLIENTS], exists[MAX_CLIENTS] si 
beginnings[MAX_CLIENTS]. Vectorul "address" contine adresele IP ale tuturor 
clientilor conectati, clients_names contine numele acestora, ports contine 
porturile, iar beginnings, timpul la care s-au conectat la server. O variabila
din vectorul exists ia valoarea 1 daca exista conexiune pe socketul respectiv 
si -1 in caz contrar. Pentru fiecare dintre acestea, indexul reprezinta 
socketul clientului.

	Functia indexOf intoarce indexul unui nume de client primit ca parametru 
daca exista si -1 in caz contrar, iar functia present intoarce 1 daca un client
exista si 0 in caz contrar.

	In main se initializeaza variabilele si se face bind pe portul primit ca 
parametru, urmand a asculta pe acest port. 

	In cazul mesajelor venite pe socketul 0(stdin), se citeste mesajul folosind
fgets si se trateaza comenzile. Pentru comanda "status", se parcurge vectorul 
"exists" si, cand se intalneste valoarea 1, se afiseaza numele clientului, 
portul pe care asculta si adresa IP. Pentru comanda "kick", se cauta socketul 
clientului cerut(cu indexOf) si se trimite acestuia comanda "quit". In cazul in
care clientul nu exista, se afiseaza un mesaj corespunzator. Pentru comanda 
"quit", se trimite la fiecare client aceasta comanda, dupa care se inclide 
socketul serverului si se iese din program.

	In cazul mesajelor venite pe socketul pe care asculta serverul, se creeaza 
o noua conexiune, dupa care se primeste mesajul care contine datele de 
identificare ale noului client. Daca exista deja un client cu acelasi nume, se 
trimite noului client comanda "nameConflict" si se inchide conexiunea cu 
acesta.

	Pe socketii pe care se comunica direct cu clientii se pot primi 3 comenzi.
Pentru comanda "listclients" se formeaza o lista cu toti clientii conectati, 
care se pune intr-un vector si se trimite clientului care a facut solicitarea. 
In cazul comenzii "infoclient", se trimit solicitantului numele clientului, 
portul pe care asculta acesta si timpul scurs de la conectarea acestuia la 
server in secunde(folosind difftime) daca exista clientul, iar daca nu, se 
trimite un mesaj corespunzator. Daca se primeste comanda "quitC", se inchide 
conexiunea cu transmitatorul, intrarea acestuia in vectorul exists este setata 
la -1, iar numele este sters din vectorul clients_names.