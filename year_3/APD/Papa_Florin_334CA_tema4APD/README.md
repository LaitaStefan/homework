Papa Florin, 334CA
Tema 4 APD - Simularea unei retele in MPI

This is an implementation of a distributed collection of processes that can 
communicate via OPENMP and estalish a topology, send messages and determine
a leader using wave algorithms. 


Cu ajutorul fisierului Makefile atasat se obtine executabilul tema4. Am atasat
2 topologii de test. Rularea pe aceste 2 exemple se face(dupa rulare make) 
astfel:

mpirun -n 12 ./tema4 fisier_topologie1 mesaje.txt
mpirun -n 12 ./tema4 fisier_topologie2 mesaje.txt



Pentru realizarea primei parti a temei - stabilirea topologiei, fiecare proces
citeste din fisierul primit ca parametru in linia de comanda lista cu vecinii
sai. Procesul 0 este initiator si trimite mesaje de sondaj tuturor vecinilor 
sai, dupa care asteapta raspunsul cu topologia. Procesele neinitiatoare primesc
primul mesaj de sondaj trimit topologia reala numai celui de la care a primit
acest mesaj. Din cauza ciclurilor care pot exista intr-un graf, un proces poate
primi mai multe sondaje. Dupa primirea primului sondaj, fiecare proces trimite 
tuturor vecinilor, cu exceptia celui de la care a primit primul mesaj, o sonda. 
In continuare, asteapta raspunsuri de la acesti vecini. Intre mesajele primite, 
pe langa ecouri, se pot primi si alte sonde. La acestea se raspunde cu o 
topologie nula. Dupa ce s-au primit toate ecourile, se trimite parintelui 
topologia formata. In urma acestui algoritm, procesul 0 va avea o topologie 
completa. Pentru ca toate procesele sa aiba topologia, se apeleaza metoda
difuzare_topologie, in cadrul careia procesul 0 trimite topologia tuturor
vecinilor, iar procesele neinitiatoare primesc topologia, dupa care o trimit 
tuturor vecinilor, cu exceptia celui de la care a primit. Pentru a trata si 
cazul in care avem bucle, se face Irecv, ignorandu-se continutul mesajului pana
cand nu mai exista mesaje de primit. Dupa ce toate procesele si-au stabilit 
topologia, se ruleaza algoritmul lui Dijkstra pentru contruirea unei tabele
de rutare, care este afisata la consola.

Pentru implementarea sistemului de mesaje, fiecare proces citeste fisierul
cu mesajele. Daca mesajul curent ii apartine, se seteaza antetul mesajului
(pozitia 0 - tipul de mesaj, pozitia 1 - sursa, pozitia 2 - destinatia, pozitia
3 - pasul de timp) si se trimite. Daca este un mesaj unicast, se trimite
intrarii aferente destinatarului din tabela de rutare. Daca este broadcast, se 
trimite tuturor vecinilor direct conectati. La final, se trimite un broadcast
pentru a marca sfarsitul comunicarii. In cazul proceselor care nu sunt sursa, 
se primeste broadcast-ul initial, apoi se primesc mesaje atata timp cat acestea
nu sunt de tip broadcast final. Daca se primeste un mesaj broadcast, se seteaza 
un flag pentru a sti ca la pasul curent s-a primit mesajul si sa nu se mai
trimita mai departe daca se mai primeste acelasi mesaj inca o data. Daca se 
primeste un mesaj unicast, se verifica daca procesul curent este destinatarul. 
Daca da, mesajul nu este trimis mai departe. In caz contrar, mesajul este 
trimis intrarii aferente din tabela de rutare. In ambele cazuri, se afiseaza 
informatii despre mesaj.

Pentru cea de-a treia parte - alegerea liderului, am folosit un algoritm unda
similar cu cel de stabilire a topologiei. Avem un initiator - procesul 0 - care
trimite mesaje de sondaj pentru a afla topologia. Dupa aflarea topologiei, 
aceasta este difuzata tuturor nodurilor, care pot apoi lua o decizie cu privire
la lider si secund. Criteriul de selectie il reprezinda cele mai mici 2 ranguri
din graf. Fiecare proces afiseaza selectia facuta.