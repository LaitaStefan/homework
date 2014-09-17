Papa Florin, 334CA
Tema 3 SO

This project contains the implementation of a virtual memory allocator,
together with a page fault handler that imitates the behaviour of the page fault
handlers existent in operating systems. The difference between Linux and Windows
versions are system calls.


Toata implementarea temei 3 se afla in fisierele vmsim.c si helpers.h. 

Header-ul helpers.h contine structurile page_table_entry si frame, care retin 
informatiile necesare cu privire la pagini, respectiv frame-uri. In functie de
aceste informatii se efectueaza in handler mapari/demapari pentru a indeplini 
conditiile temei. Pe langa aceste structuri, mai exista si structura mem_alloc,
folosita pentru a retine informatii utile cu privire la un spatiu virtual
alocat.

Fisierul vmsim.c contine implementarile functiilor din vmsim.h. Functia
vmsim_init realizeaza inregistrarea handler-ului de exceptie folosind
wrapper-ul pus la dispozitie pe Linux si pe Windows. Functia vmsim_cleanup
deinregistreaza handler-ul de exceptie.

Functia vm_alloc aloca spatiul virtual, deschide si trunchiaza fisierele de
ram si swap la dimensiunea dorita si realizeaza initializarea unei structuri
mem_alloc si adaugarea acesteia la o lista inlantuita care retine toate
alocarile facute. Logica este aceeasi pentru Linux si Windows, fiind diferite
doar apelurile de sistem pentru mapare/demapare, trunchiere fisier etc.

Functia vm_free dezaloca toata memoria folosita pentru a tine evidenta
spatiului virtual alocat.

Handler-ul de exceptie este asemanator intre sistemele de operare, din nou fiind
diferite doar apelurile de sistem si definirea handler-ului(vector de exceptie
pe Windows). Prima data se cauta spatiul virtual din care provine page fault-ul.
Daca nu se gaseste nicio potrivire, de iese cu eroare. Mai departe, se
identifica pagina care a dat page fault si se verifica permisiunile existente. 
Daca pagina nu este alocata(STATE_NOT_ALLOC), se cauta un frame liber in ram(se 
face swap out la nevoie) si se mapeaza pagina cu drept de scriere. Daca pagina
este in swap, se cauta un frame liber in ram si se mapeaza pagina din swap
folosind permisiunile pe care le-a avut cand s-a facut swap out la pagina
respectiva. In final, daca pagina este deja in RAM si s-a primit page fault,
inseamna ca incercam sa scriem intr-o zona care are protectie doar la citire,
asadar demapam pagina si o mapam in acelasi loc cu drepturi READ | WRITE.

De departe, cel mai greu a fost pe Windows din cauza lipsei unei documentatii
adecvate si a modului destul de complicat de lucru cu functiile de gestiune a
memoriei virtuale. Pe Linux avem mmap si munmap, pe cand pe Windows avem
VirtualAlloc, VirtualFree, MapViewOfFile, MapViewOfFileEx, UnmapViewOfFile etc.
