Papa Florin, 324CA

This homework implements a bot that can play a game where the field is 
represented by dots inside a rectangle. Each player has to get in the other
player's "home" situated at one of the ends of the field. If the point where 
we just moved has already been visited, we get to move again. Only one move can
be made on an edge. The bot was implemented using the MiniMax method.


Pentru implementarea temei, am definit mai multe clase ajutatoare. Clasa 
"Coord" retine o pereche de intregi ce reprezinta coordonate intr-o matrice(x 
reprezinta linia, iar y coloana). Clasa "Directions" contine un vector de 8 
elemente si un camp "visited", care ia valoarea 1 daca punctul curent a fost 
sau nu vizitat anterior. Vectorul "direction" are pe pozitia i valoarea 0 daca 
directia i este inaccesibila(este marginea terenului sau calea a fost deja 
folosita), respectiv 1 daca se poate merge in acea directie. Clasa "MyInteger"
este folsita la calcularea celei mai bune directii(functia Maxi intoarce 
directia prin parametru). 

Clasa "Pitch" reprezinta terenul de fotbal ca o matrice 13x9 de obiecte de tip 
"Directions". Constructorul fara argumente initializeaza un teren gol, cu 
punctul curent in centru si cu limitarile aferente marginilor si portilor
(scoate, in functie de coordonate, directii in care se poate merge), iar 
constructorul cu argumente creeaza o noua clasa pornind de la o matrice 
"Directions" data si un punct curent dat. Matricea "values" retine costul 
fiecarei pozitii din teren(din ce in ce mai mare cu cat se indeparteaza de 
poarta mea). Functia "eval" intoarce valoarea din "values" corespunzatoare 
coordonatelor curente. Functia "move" executa o mutare pe o directie data, 
setand punctul de plecare ca vizitat si marcand ca inaccesibila calea folosita 
(acest lucru inseamna scoaterea caii din caile punctului de start, dar si din 
cele ale punctului in care am ajuns). Functia "MultipleMoves" primeste un 
string si executa mutarile continute de acesta (este folosita pentru a executa 
mutarile adversarului). Functia "gameOver" intoarce 1 daca am castigat, -1 daca
am pierdut si 0 daca mai este de jucat.

Pentru aflarea celei mai bune cai, am implementat in clasa "Bot" functia 
"Maxi". Aceasta primeste ca parametru terenul si intoarce cea mai buna mutare. 
In cazul in care se detecteaza faptul ca am invins, intoarce valoarea 1000. 
In cazul in care am pierdut sau nu mai exista mutari, va intoarce -1000. Daca 
mai sunt mutari de facut, pentru fiecare mutare valida existenta se cloneaza 
terenul si se face mutarea. Daca punctul in care am ajuns este unul deja 
vizitat, se apeleaza din nou maxi pentru copia hartii. In caz contrar, se 
apeleaza functia "evaluate". La final, se compara rezultatul intors cu maximul 
initializat la intrarea in functia "Maxi" cu -Inf. Daca rezultatul curent este 
mai mare decat maximul, este inlocuit maximul cu noul rezultat si se salveaza 
mutarea.