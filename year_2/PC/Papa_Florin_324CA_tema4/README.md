Papa Florin, 324CA

This homework implements a DNS Resolver that sends a DNS Request using the 
format described in RFC 1034. The response is received and interpreted 
according to its headers.


Am inclus in arhiva un Makefile care compileaza sursa, obtinand executabilul 
my_dns_client. Acesta este sters folosind regula clean.

Arhiva contine si antetul dns_message.h deoarece am adaugat in el structura 
dns_SOA. Daca este suprascris acest fisier, interogarea de tip SOA nu va 
functiona.


Pentru implementarea clientului de DNS, am folosit o conexiune pe socketi UDP 
pe portul 53. Functia getServerIP intoarce in variabila server_ip prima adresa 
IP valida din cadrul fisierului dns_servers.conf. In momentul in care am ajuns 
la finalul fisierului, programul se termina, afisand la consola un mesaj 
corespunzator. In cazul in care am primit un mesaj de la un server DNS, in urma
interpretarii acestui mesaj programul se va termina. Pentru a lua in considerae
si cazul in care raspunsul de la server nu se primeste, am setat timeout de 1 
secunda pentru socket la receive. Daca nu am primit nimic, citesc urmatoarea 
adresa IP din fisier si incerc o noua interogare.

Functia createQuery primeste ca parametri numele domeniului, tipul interogarii 
si buffer-ul in care va depune rezultatul. Aceasta intoarce dimeniunea 
query-ului. In primul rand, in buffer se pune o structura de tip dns_header_t 
setata pe 0 initial. Campurile modificate sunt bitul rd(recursion desired), 
care este pus pe 1, campul id, care primeste valoarea 50(nu are importanta 
valoarea acestui camp, este doar de control) si campul qdcount, care primeste 
valoarea 1(folosind htons deoarece este o valoare pe 2 octeti). In continuare, 
se adauga numele domeniului formatat corespunzator(pereche label - byte stream)
si o structura de tip dns_question_t, la care se seteaza campurile qtype(in 
functie de input) si qclass(se seteaza la 1, adica tipul IN). Functia care face
conversia intre string-ul reprezentand tipul interogarii si valoarea numerica 
aferenta este convertType. Aceste valori trebuiesc trecute intai la network 
byte order folosind htons. Functia care realizeaza conversia intre numele de 
domeniu primit ca parametru si formatul necesar pentru server este 
formatNameForQuery, care separa numele in tokeni folosind strtok cu separatorul
".", adaugand inainte de fiecare token, intr-un buffer, lungimea acestuia, 
urmata de token. Foarte important este faptul ca, atunci cand adaug structura 
dns_question_t, o adaug lasand un octet de 0 dupa numele de domeniu. Acest 
lucru marcheaza sfarsitul acestiu camp.

Cele mai mari dificultati le-am intalnit cand am incercat sa interpretez 
datele primite deoarece trebuie respectat intocmai formatul impus, iar orice 
eroare produce probleme majore. Spre exemplu, atunci cand citeam din buffer in 
structura dns_rr_t, citeam sizeof(dns_rr_t), urmand a citi in continuare dupa 
aceasta valoare. Din cauza faptului ca datele din structura sunt de tipuri 
diferite, dimensiunea intoarsa de sizeof nu este dimensiunea efectiva a 
acesteia, ci cu 2 unitati mai mare, ceea ce ma facea sa nu citesc bine datele 
in continuare.

Cea mai complexa functie din tema, cea care interpreteaza datele primite si 
scrie in fisierul de log("logfile") este functia processMessage. Aceasta 
primeste buffer-ul receptionat de la server si il "decapsuleaza" pentru a 
obtine informatiile dorite. Decapsularea reprezinta procesul invers celui 
descris mai sus in ceea ce priveste campurile header si question. In plus fata 
de etapa de incapsulare, campurile ancount, nscount si arcount sunt retinute 
deoarece folosesc la interpretarea restului mesajului. In cazul in care sunt 
toate nule, inseamna ca nu am primit nicio informatie, deci ies din functie si 
incerc urmatorul server din lista. In caz contrar, scriu in fisier adresa 
serverului si interogarea facuta, urmand a adauga si raspunsul acestuia. 
Intr-un for de la 0 la 2 tratez cele 3 cazuri posibile: answer, authority si 
additional. Pentru fiecare, campurile count indica numarul de resource records 
primite. Acestea se trateaza identic in toate cele 3 cazuri. Se incepe cu 
extragerea numelui din RR folosind functia formatNameForPrint, care primeste ca
parametri buffer-ul si offset-ul de unde trebuie sa citesc. In cazul in care la
acel offset se gaseste un pointer(dupa aplicarea ntohs pe primii doi octeti, 
primii 2 biti ai acestora sunt 1), la numele construit pana la acel moment se 
adauga numele continut la adresa indicata de pointer. Constructia se face 
recursiv, aplicand functia din nou dupa fiecare token scos,dar pe noul offset. 
Se intoarce dimeniunea efectiva a string-ului, pentru a sti de unde se citeste 
in continuare.

Mai departe, se citesc datele din antetul RR intr-o structura dns_rr_t, dupa 
care se citesc datele efective, in functie de tipul acestora:

1. A - RDATA ocupa 4 octeti reprezentand o adresa IP
2. NS - RDATA contine un nume de domeniu, citit folosind functia formatName
		ForPrint
3. CNAME - RDATA contine tot un nume de domeniu
4. MX - RDATA contine 2 octeti ce contin un intreg reprezentand preferinta 
		pentru acea adresa, urmati de un nume de domaniu
5. SOA - RDATA contine un nume de domeniu, urmat de mai multe valori, pe care 
		le-am codificat in structura dns_SOA. Aceste valori sunt serial, 
		refresh, retry, expire, minimum, toate fiind intregi pe 4 octeti.
6. TXT - RDATA contine un string de caractere

In functie de toate aceste tipuri, interpretez datele, scriind in fisierul de 
log datele relevante. Pentru toate valorile pe mai mult de 1 octet aplic ntohs 
pentru a le trece la host order.

In concluzie, a fost o tema destul de dificila, insa consider ca a fost 
folositoare pentru a ne face sa intelegem mai bine mecanismul DNS. 

Am inclus in arhiva temei si fiserul de log obtinut dupa interogari pentru 
numele google.com pentru fiecare situatie din tema.