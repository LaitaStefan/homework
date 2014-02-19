Papa Florin, 324CA

This homework implements a Dynamic Routing Protocol(link state) that uses 
message exchange in order for each router to have an accurate topology and form
a routing table. The program treats the situation where new routers are added
to the topology, but does not have update messages that could detect a link 
gone down, therefore it can only detect addition, but not deletion. Once a 
message changes the topology, we recalculate the routing table using Dijkstra's
algorithm. 


Pentru implementarea temei, am pornit de la indicatiile din enunt, pe care 
le-am urmat intocmai.

Cel mai greu pentru mine a fost sa fac diferenta intre un eveniment si un 
mesaj, insa o data ce am reusit sa le diferentiez, totul a devenit mai clar. 
Implementarea tratarii evenimentelor este facuta in functia 
"procesare_eveniment". In cazul evenimentelor de tip 1 (aparitia unui router in
retea), am parsat datele de intrare folosind strtok si am trimis Database 
Request la fiecare vecin specificat in input, salvand costul fiecarei legaturi 
in payload pentru a putea fi salvat de catre destinatar. Am incrementat 
valoarea lui "seq_no" o singura data intrucat nu creez un mesaj nou pentru 
fiecare vecin, ci trimit acelasi mesaj la toti vecinii, modificand doar campul 
next-hop.

Pentru evenimentul de tip 2(aparitia unui nou link), parsez din nou inputul si 
verific daca routerul curent este id1 sau id2. Daca nod_id este id1, formez un 
pachet Database Request(tip 2) si il trimit catre id2. In caz contrar, trimit 
catre id1, incrementand de fiecare data seq_no si salvand costul legaturii in 
payload.

Pentru evenimentul de tip 3(disparitia unui link), marchez in topologie 
legatura ca inexistenta, dupa care trimit pachete LSA la toti vecinii nodului 
curent. Si de aceasta data seq_no se incrementeaza o singura data.

In cazul evenimentului de tip 4(formarea unui mesaj cu date), formez un mesaj 
care are campul payload gol. Creatorul este reprezentat de campul sursa, iar 
destinatarul, de campul dest. Destinatarul este salvat in payload, iar la 
next-hop trecem intrarea aferenta acestuia in tabela de rutare a nodului 
curent. In cazul in care intrarea nu exista, pachetul nu este trimis mai 
departe.

LSADatabase este reprezentat de un vector de mesaje, la indexul k fiind 
pastrata intrarea aferenta mesajului primit de la creatorul k. Initializarea 
LSADatabase se face la inceput in main, setand timpul si seq_no la valoarea -1 
si continutul payload-ului la null(nicio intrare in tabela de topologie). 
Vecinii se retin in payload intr-o structura de tip "packet" ce contine un 
vector cu costurile aferente fiecarui vecin al unui anumit nod.

Pentru a face distinctia intre mesaje vechi si noi, am ales sa folosesc doi 
vectori de structuri msg sub forma de cozi(old_queue, new_queue), retinand 
pentru fiecare dimensiunea sa pentru a sti cate elemente trebuiesc extrase din 
fiecare. In cazul primirii unor mesaje de tip 1-4, singura actiune efectuata 
este introducerea in coada new_queue. Pentru un mesaj de tip 6, se scot toate 
elementele din new_queue si se introduc in old_queue, dupa care se incepe 
procesarea mesajelor. Pentru un mesaj de tip 1(LSA), se actualizeaza 
LSADatabase in cazul in care LSA-ul primit este mai nou decat cele existente
(lucru care se vede dupa timpul de creare) si se trimite tuturor vecinilor 
nodului, cu exceptia celui de la care a fost primit pentru a evita buclele. 
Valoarea lui seq_no nu se modifica deoarece nu se creeaza un mesaj nou, ci doar
se retrimite cel primit. 

In cazul unui mesaj de tip 2(Database Request), se actualizeaza topologia cu 
nodul de la care am primit acest mesaj, dupa care acestuia i se trimit toate 
intrarile in LSADatabase care au fost initializate la un pas de timp anterior
(timp != -1). Creatorul pentru fiecare din aceste mesaje este nu nod_id, ci 
nodul caruia ii corespunde intrarea in LSADatabase. Urmatorul pas este 
trimiterea unui mesaj LSA catre toti vecinii, inclusiv noul nod. Seq_no se 
incrementeaza o singura data in cadrul acestei tratari de eveniment.

Pentru un mesaj de tip 3(Database Reply), verific daca mesajul este vechi sau 
duplicat, iar daca nu este, il salvez in LSADatabase, insa fara a modifica time
anterior deoarece urmeaza sa primesc un mesaj LSA care nu ar mai fi luat in 
considerare daca as actualiza campul time.

Cand un mesaj de tip 4 este primit(rutare pachet), se verifica daca nodul 
curent este destinatarul sau daca intrarea pentru destinatar nu exista in 
tabela de rutare, cazuri in care mesajul este aruncat. In cazul in care 
destinatarul se gaseste in tabela de rutare, se trimite mesajul mai departe 
next-hop-ului aferent, modificand senderul si timpul de trimitere.

Atunci cand in LSADatabase se primeste un nou mesaj, topologia trebuie 
actualizata conform noilor conditii din retea. Acest lucru se face in functia 
"update_topologie", in care se parcurge LSADatabase si, pentru fiecare vecin 
din cadrul payload-ului, se verifica daca exista un LSA mai nou. Daca exista, 
informatiile despre punctul curent sunt luate din acesta, altfel sunt folosite 
informatiile din LSA-ul initial.

Calculul tabelei de rutare se face pe baza topologiei, folosind algoritmul lui 
Dijkstra de drum minim prezentat la curs. In plus fata de varianta prezenta in 
curs am adaugat o conditie suplimentara pentru cazul in care se detecteaza o 
cale de cost egal cu una deja existenta pentru un anumit nod. In cazul in care 
calea este mai scurta decat cea existenta, se salveaza. In cazul in care este 
egala cu cea existenta, se salveaza numai daca next-hop are un indice mai mic 
decat cel existent(pentru a satisface cerinta temei care specifica faptul ca 
daca exista 2 cai de cost egal, se retine cea cu indicele mai mic).      