Papa Florin, 334CA
Tema 4 SO

This homework implements a userspace thread planner using a Round Robin 
algorithm. The difference between the Linux and Windows versions of the
homework are the system calls.


Pentru implementarea temei am definit un ENUM care sa ma ajute sa tin evidenta
starii thread-urilor. Header-ul utils.h mai contine si o structura thread, care
retine informatii cu privire la un anumit thread(id, prioritate, variabila
conditie etc.). Variabila de conditie este necesarra pentru a putea opri si
porni thread-ul in orice moment.

Implementarea functiilor din so_scheduler.h este conforma cu cerintele temei:
se executa instructiunea, apoi se verifica daca exista un thread mai prioritar
sau daca thread-ului curent i-a expirat cuanta si trebuie sa predea procesorul
urmatorului thread cu aceeasi prioritate. Acest lucru se face cu ajutorul
functiei plan_Round_Robin, care invalideaza dreptul thread-ului curent de a
rula, apoi cauta un thread cu aceeasi prioritate pentru a-i pasa stafeta. Acel
thread poate fi chiar cel de la care am plecat.

Thread-urile sunt retinute in structura thread, care este o lista simplu
inlantuita. Se poate cauta in lista dupa thread_id folosind functia get_thread.
In cazul functiei fork, se creaza noul proces si se intoarce id-ul sau doar
dupa ce ne asiguram ca procesul a ajuns in starea READY, lucru care se face cu
ajutorul unei variabile conditie.

Pentru implementarea functiei signal, iau in considerare mai multe posibilitati:
nu exista thread-uri blocate la dispozitivul dat ca parametru, nu exista
dispozitivul sau exista cel putin un thread blocat la dispozitiv. In primul si
al doilea caz, se apeleaza planificatorul, iar in cel de-al treilea caz,
thread-ul care a apelat signal se blocheaza pana cand este preemptat de
planificator.

Pentru wait, se asteapta la dispozitivul dat ca parametru(daca exista), apoi
se apeleaza planificatorul.
