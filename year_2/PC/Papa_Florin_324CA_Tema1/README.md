Papa Florin, 324CA

* PC, Tema1 *

This project contains an implementation of a sending mechanism between two 
processes. Four sending methods are approached: one where there are no losses
and no modifications of the messages, one where there are losses and uses the
GO BACK N mechanism,one that uses SELECTIVE REPEAT, and one where there are 
losses and modified bits that uses SELECTIVE REPEAT with parity check and 
discards the modified frames.

Compilare: make
Pornire + verificare transfer: ./run_experiment.sh

Unităti de măsură pentru parametrii din script:
	* SPEED: Mb/s
	* DELAY: ms
	* LOSS: procentaj cadre pierdute
	* CORRUPT: procentaj cadre corupte

Domeniu de valori:
	* SPEED = 5 .. 20 Mb/s
	* DELAY = 10 ... 1000 ms
	* LOSS = 0 ... 10 %
	* CORRUPT = 0 .. 10% / 100 %
	* TASK_INDEX = 0, 1, 2, 3 și 4
	
Fisierele send.c si recv.c contin apeluri de functii care implementeaza 
task-urile temei. Functiile sunt definite in fisierul utils.h si sunt 
implementate in utils.c pentru o modularizare cat mai buna. Am trimis in arhiva
temei inclusiv fisierul makefile deoarece este modificat pentru a include la 
compilare sursa utils.c.

Task0:
	Pentru task 0, avand in vedere ca nu exista cadre corupte si nici pierderi, 
pentru trimitere am citit din fisier bucati de 1400 de octeti, pe care le-am 
pus in payload-ul unei structuri msg folosind memcpy si le-am trimis. Am trimis
la inceput un numar de cadre egal cu dimensiunea ferestrei, dupa care am 
adaugat cate un cadru in fereastra. A trebuit in sender sa primesc si un 
raspuns de la receiver(pe care nu il folosesc deoarece transmisia nu are 
pierderi) intrucat, pentru fisiere mari, senderul termina de trimis si se 
inchidea inainte ca receiverul sa termine de primit, avand cadre pierdute. 
Pentru ultima bucata din fisier, am citit numai dimensiunea ramasa, pe care am 
salvat-o in campul len, pentru a purea sti in receiver cat sa citesc din 
payload. In cazul in care fisierul are o dimensiune multiplu de 1400, pentru ca
receiver-ul sa stie cand sa se opreasca, am trimis la final un mesaj avand in 
campul len valoarea -1   (deoarece daca puneam 0 primeam eroare). In receiver, 
se receptioneaza mesajele intr-un while(1), iar in momentul in care se primeste
un mesaj cu campul len mai mic de 1400, se scrie in fisier, dupa care dau break
pentru a opri primirea de mesaje. Numele fisierului trimis se salveaza in 
primul cadru transmis, care este tratat separat in receiver, pentru a forma 
numele fisierului de iesire si pentru a-l deschide.

Task1:
	In cazul task-ului 1, am definit o noua structura, denumita pkt, cu 
campurile nr_seq, type(ACK, DATA, NACK) si payload(dimensiune 1395). In sender,
primul mesaj contine numele fisierului transmis pentru a putea forma in 
receiver fisierul in care se salveaza datele. La inceput, intr-un for, am 
trimis un numar de cadre egal cu dimensiune ferestrei. Apoi, cat timp ultimul 
ACK primit are numarul de secventa mai mic decat numarul total de cadre, 
primesc cate un cadru(folosind recv_message_timeout). In cazul in care primesc 
timeout sau daca ACK-ul primit nu este cel asteptat, reumplu fereastra cu cadre
incepand de la ultimul ACK primit. Altfel, daca ACK-ul primit este cel 
asteptat, trimit urmatorul cadru. Pentru a evita intrarea in timeout, in 
receiver primesc mesajele si dau ACK la toate cadrele primite, salvandu-le in 
fisierul de primire numai daca au numarul de secventa asteptat. In sender, 
pentru a sti ce mesaje trebuiesc ignorate, folosesc variabilele ignore si 
in_progress. In cazul in care ignore are o valoare strict pozitiva si primesc 
un ACK cu numar de secventa diferit de cel asteptat, il ignor si nu retrimit de
la ultimul ACK primit.

Task2:
	Pentru task 2, am definit vectorul acks pentru a sti care cadru a fost 
confirmat si care nu. Cadrele neconfirmate coincid unei valori de -1, iar 
cele confirmate, unei valori de 1. Asemanator task-ului precedent, primul 
cadru contine informatii despre numele fisierului, dar si numarul de cadre 
ce urmeaza a fi trimise. Intr-un for, trimit un numar de cadre egal cu 
dimensiunea ferestrei. Apoi, cat timp ultimul ACK primit este mai mic decat
numarul total de cadre, primesc un mesaj de confirmare(cu 
recv_message_timeout). In cazul in care primesc timeout, inseamna ca 
fereastra s-a golit si o reumplu cu cadre care nu au primit ACK(incepand cu
primul cadru neconfirmat, wanted_ack si cautand in vectorul acks mesaje 
neconfirmate). In cazul in care nu primesc timeout, marchez cadrul primit 
ca si confirmat si trimit urmatorul cadru. In cazul in care cadrul primit 
are numarul de secventa wanted_ack, setez valoarea lui wanted_ack cu 
urmatorul cadru din vector care nu este confirmat. Acest mecanism ma 
asigura ca nu trimit niciodata duplicate, iar pe acest lucru se bazeaza 
receiver-ul. Acesta primeste cadre si le salveaza intr-un vector(care are 
o dimensiune initiala de 100 si este realocat la nevoie), indiferent de 
ordinea in care ajung. In cazul in care primesc cadrul 0, salvez numele 
fisierului si numarul total de cadre. In cazul in care primesc ultimul 
cadru, salvez dimensiunea lui pentru a sti la final cat sa scriu in fisier.
Deoarece cadrele ajung neordonate, dupa ce le primesc pe toate, le sortez 
si le scriu in fisier.
	
Task3:
	Task 3 este foarte asemanator cu task 2, insa are o conditie suplimentara, 
cea a sumei de control. Pentru acest task am definit structura pkt2 cu 
campurile sum si payload(de dimensiune 1399). Primii 4 octeti din payload sunt 
ocupati de numarul de secventa al cadrului. Deoarece un octet din payload poate
fi corupt, suma de control o fac pentru fiecare bit din fiecare octet. Suma de 
control este reprezentata de un octet al carui prim bit reprezinta suma modulo 
2 al primului bit din fiecare octet al payload-ului, acest lucru fiind valabil 
pentru ceilalti 7 biti. Trimiterea se face identic cu trimiterea de la task 2, 
cu diferenta ca inainte de a trimite un cadru, se calculeaza si suma de  
control. La primire, calculez suma de control a payload-ului primit si o compar
cu ce primita. In cazul in care primesc un cadru corupt, nu trimit niciun 
raspuns, ceea ce activeaza mecanismul de timeout si retrimite cadrele eronate. 
Pentru calculul sumei de control, mi-am definti functiile get_bit, set_bit si 
get_parity.