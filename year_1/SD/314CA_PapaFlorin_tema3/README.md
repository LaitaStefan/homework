Papa Florin
314CA

This homework contains an implementation of a data compression method using
Huffman Trees. A word is replaced by a binary code that describes the position
of the node in the tree(0 for right and 1 for left).

 
Cele doua clase fundamentale folosite in rezolvarea temei sunt heap si Huffman.
Heap-ul este alcatuit din structuri de tip Node care contin campurile symbol, 
frecv si address. Symbol se refera la indicele unui pixel in tabela de culori, 
frecv retine frecventa aparitiei lui si address retine indicele in arborele 
Huffman(initializat cu -1, ceea ce arata ca nu se afla in arbore la inceput). 
Heap-ul mai contine campurile dimVect(care retine numarul de elemente continute
de heap) si capVect(care retine dimensiunea maxima a heap-ului). Operatiile 
care pot fi efectuate asupra acestei clase sunt inserarea, stergerea sau 
afisarea elementului superior(elementul minim din heap). Pentru ca aceste 
operatii sa fie posibile, avem nevoie si de functiile PushUp, PushDown(care 
urca sau coboara un element pentru a pastra proprietatea de heap) si parent, 
leftSubtree si rightSubtree(care intorc indicele parintelui, fiului stang sau 
drept al unui nod si -1 daca nu are parinte sau fiul cerut). Dimensiunea maxima
aleasa pentru heap este 256 deoarece acesta este numarul maxim de culori care 
exista(de la 0 la 255).

Pentru implementarea clasei Huffman, am avut nevoie de structura 
TagSerialHuffmanNode, folosita la formarea arborelui Huffman. Pe langa aceasta,
am folosit structura codes, carea are campurile length si code(lungimea si 
codul aferent unei anumite culori), o clasa heap si variabila dim care va 
retine la final dimensiunea vectorului care va contine codificarea.

Functia CreateHuffArray creeaza arborele Huffman, stocat sub forma de vector. 
Ea primeste ca parametru un vector al carui indice il reprezinta simbolul, iar
valoarea o reprezinta frecventa. Algoritmul folosit pentru formarea lui este 
cel pus la dispozitie in enuntul temei(extragerea primelor doua elemente, 
insumarea frecventelor acestora si introducerea inapoi in heap doar a unui nod 
fara simbol, dar cu frecventele insumate si introducerea celor 3 in arborele 
huffman). Functia codificareHuff obtine codificarea elementelor terminale ale 
arborelui Hufman si se bazeaza pe o parcurgere in adancime(DFS). O mutare la 
dreapta adauga 0 la cod, iar o mutare la stanga adauga 1. In mod recursiv, se 
obtin codificarile pentru toate frunzele arborelui. Parametrii primiti de 
functie sunt codul, lungimea si indicele de la care se incepe. Pentru a obtine 
codificarea tuturor elementelor, se apeleaza functia pentru radacina(care are 
codul, lungimea si indicele egale cu 0). Functia createBitString formeaza un 
vector de tip uint8_t care retine codificarea Huffman intr-un sir continuu de 
biti care poate fi apoi scris in fisier. 

Antetul bitmap.h este cel oferit in enuntul temei si este folosit pentru a citi
elementele imaginii.

Implementarea temei este realizata in fisierul tema3.cc. Aici se face citirea 
din fisier, formarea numelui fisierului de output in functie de diferitele 
situatii si compresia si decompresia. Functia createColorArray creeaza vectorul
al carui indici il constituie simbolul culorii, iar valoarea o constituie 
frecventa. Functia compress face modificarile necesare la structurile-antet ale
unui fisier bmp pentru a-l transforma in mcb si scrie datele in fisier.