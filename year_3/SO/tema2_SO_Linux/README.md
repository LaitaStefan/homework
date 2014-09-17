Papa Florin, 334CA
Operating Sytems 2014 - Assignment 2

This homework contains the implementation of a shell that can handle all the
commands given to a standard Linux terminal. The aim was to familiarize us
with the "fork" system call, but I found that the most challenging part was
correctly parsing and interpreting the commands.


Tema nu a impus probleme deosebite, a trebuit sa urmaresc doar scheletul de
cod, care a fost foarte folositor si foarte usor de inteles, din punctul meu de
vedere.

Intre Linux si Windows difera doar apelurile de sistem pentru deschidere
fisiere, creare thread-uri, schimbare director curent, logica programului fiind
aceeasi in ambele parti.

Pentru executia in paralel, pe Linux am pornit doua thread-uri prin doua 
apeluri fork(), dupa care am efectuat wait pe ambele pid-uri rezultate.
Pe Windows, am pornit doua thread-uri prin metoda CreateThread, careia 
i-am pasat ca argument o functie modificata parse_command, care primeste doar
comanda ca si argument(am eliminat campurile level, father, void *h, pe care
oricum nu le-am folosit deloc).

Pentru pipe, am creat un pipe prin metoda specifica fiecarui sistem de operare
si am redirectat stdout pentru thread-ul care scrie in pipe, respectiv stdin
pentru thread-ul care citeste din pipe.

Si pe Windows si pe Linux am o functie care efectueaza redirectarile pentru un
anumit thread. Pe Windows se numeste redirect_all, iar pe Linux se numeste
perform_redirects. 

Pentru ambele teme am avut grija sa eliberez memoria folosita si sa inchid
descriptorii de fisiere folositi.

