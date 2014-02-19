Papa Florin, 334CA
Tema 2 APD - Map Reduce

This homework contains an implementation of a method of determining the 
symilarity between a given file and each file from a a given set using the 
Map-Reduce Paradigm. A set of workers count the frequencies of the words in a
part of the given files and the results are centralised at the end. To obtain
the final numbers, a formula is applied for each of the words in the target
file in order to obtain its symilarity with another file.


In arhiva am inclus un fisier Makefile cu reguli build si clean. Regula build 
creeaza executabilul FileParser, iar regula clean sterge toate fisierele .class
rezultate dupa compilare. Rularea se face manual apeland java FileParser NT 
fisin.txt fisout.txt.

Implementarea temei a pornit de la scehletul de cod pus la dispozitie in cadrul
laboratorului 5. Am modificat clasele PartialSolution, Worker si WorkPool
pentru implementarea functionalitatilor cerute de tema.

Clasa MyPair defineste o pereche de tipul <String, 
Vector<ConcurrentHashMap<String, Integer>>> si este folosita pentru a asocia 
numele unui fisier cu vectorul de HashMap-uri rezultate din procesarea 
bucatilor de fisiere.

Clasa Worker reprezinta clasa care extinde clasa Thread. Aceasta primeste in 
constructor un obiect WorkPool din care ia / in care pune task-uri. In momentul
in care se preia un task din WorkPool, se verifica tipul acestui task ("m" - 
map, "r" - reduce si "a" - add). In functie de tipul task-ului, se aplica o 
procesare corespunzatoare. Pentru "m", inseamna ca am primit o bucata de fisier
de procesat. Fisierele sunt transmise unui Worker sub forma unui String ce 
reprezinta numele fisierului si 2 intregi care specifica de la ce offset la ce
offset trebuie sa citim din fisier. Deschiderea si citirea fisierului se fac la
nivel de Worker. Un Worker citeste din fisier incepand cu offset initial - 1 si
terminand cu offset final + 1(daca este posibil). Se face acest lucru deoarece
trebuie sa stim daca primul respectiv ultimul token apartine de bucata 
procesata. In cazul in care caracterul de la offset_initial - 1 sau de la 
offset_initial sunt delimitatori inseamna ca primul token apartine Worker-ului 
curent si va fi procesat. In caz contrar, va fi ignorat. Daca simbolul de la 
offset_final sau offset_final + 1 sunt delimitatori, nu mai trebuie sa citesc 
mai departe. In caz contrar inseamna ca tokenul incepe in bucata curenta si 
se continua in urmatoarea. In aceasta situatie, se citeste mai departe pana la 
intalnirea unui delimitator si se proceseaza cuvantul. Cuvintele intalnite sunt
introduse intr-un ConcurrentHashMap in care cheia este tokenul, iar valoarea,
numarul de aparitii. Dupa ce am procesat bucata de fisier, introducem 
rezultatul in WorkPool prin metoda sincronizata putSolution.
In cazul in care tipul PartialSolution este "r", inseamna ca avem un task de 
tip reduce. Actiunea pentru reduce este iterarea prin HashMap-ul primit si 
introducerea fiecarui element in vectorul rezultat din WorkPool. Astfel, 
fiecare thread proceseaza task-ul primit si depune reultatul intr-o variabila
din cadrul WorkPool. La adaugare, frecventa curenta se cumuleaza cu frecventa
cuvantului din cadrul WorkPool. Pentru a evita erorile, depunerea noilor valori
se face prin metoda sincronizata putValues.
Daca tipul actiunii este "a" inseamna ca trebuie sa calculam similitudinea 
intre 2 fisiere prin formula din cadrul enuntului. Fisierul cu care vrem sa 
comparam fisierul primit ca si task este reprezentat in WorkPool prin indexul
target care indica localizarea acestuia in vectorul de solutii. Un thread
calculeaza asemanarea cu fisierul tinta si depune rezultatul in WorkPool prin
metoda sincronizata setResemblance.
In cadul clasei Worker am definit 2 metode, starts si ends, care determina daca
un string incepe/se termina cu un delimitator.

Clasa PartialSolution desemneaza solutia partiala care trebuie procesata de un 
Worker. In functie de stadiul la care ne aflam, tipul acesteia poate fi "m", 
"r", "a", explicate mai sus. Din acest motiv, clasa are 3 constructori, unul
pentru fiecare tip.

Clasa WorkPool din cadrul laboratorului a fost modificata pentru cerintele 
temei. Aceasta contine in plus fata de clasa initiala un camp target_index care
identifica in pasul final fisierul cu care calculam asemanarile, un vector de 
tip MyPair result care va contine rezultatele dupa fiecare pas, un vector de 
intregi totals care contine numarul total de cuvinte din fiecare fisier si un 
array de tip float resemblance care contine, in final, asemanarile pentru 
fiecare fisier. Metodele adaugate la varianta initiala a clasei sunt 
putSolution(introduce in vectorul result un ConcurrentHashMap in cadrul 
fisierului corespunzator. Daca fisierul nu are nicio intrare, se creeaza una),
putValues(introduce in vectorul result o noua asociere token-frecventa), 
getTotal(calculeaza numarul total de cuvinte din fiecare fisier si le depune 
in vectorul totals), sortResemblance(sorteaza asemanarile in ordine 
descrescatoare in vederea afisarii), swapPair(interschimba 2 MyPair din cadrul 
result), swap(interschimba 2 valori din resemblance), setTargetIndex(seteaza
variabila target cu indexul fisierului cu care se doreste comparatia), 
getTarget(intoarce valoarea target-ului), getMap(intoarce o intrare din cadrul
result), getTotals(intoarce o intrare din cadrul totals), setResemblance
(seteaza un element din resemblance la valoarea specificata).

Clasa FileParser contine metoda main. Pe langa main, mai contine functia 
addToWorkPool care primeste ca argumente un string, un intreg si un WorkPool.
Stringul reprezinta numele unui fisier, intregul indica dimensiunea bucatilor 
in care se imparte fisierul, iar WorkPool indica unde se introduc task-urile.
Citirea din fisierul de input se face cu BufferedReader. Dupa citirea tuturor
informatiilor necesare, se adauga in WorkPool fiecare fisier prin metoda
addToWorkPool. In acest moment avem toate task-urile in WorkPool, asadar pornim
toate thread-urile, dupa care le oprim prin join(). Astfel se termina operatia 
de map.
In pasul urmator se iau toate elementele din vectorul result al primului 
WorkPool si se depun in cel de-al doilea WorkPool(cel de reduce). In cazul in 
care elementul curent este primul din cadrul fisierului sau, se introduce 
direct in result-ul WorkPool-ului 2 urmand a fi adaugate date in el de catre
fiecare Worker. In caz contrar, se adauga elementul curent in lista de task-uri
din WorkPool, urmand a fi preluat de un lucrator. Urmatorul pas este pornirea
Worker-ilor si oprirea lor dupa ce si-au terminat treaba. In acest moment, in 
result avem atatea HashMap-uri cate fisiere avem, fiecare HashMap continand 
toti tokenii si numarul de aparitii al acestora. In continuare, se apeleaza 
metodele getTotal(formeaza vectorul totals) si set_target_index(seteaza 
targetul). Pentru a calcula asemanarile, in WorkPool se adauga un numar de 
task-uri egal cu numarul de fisiere, fiecare PartialSolution avand tipul "a".
Pentru procesare, se pornesc si se opresc din nou Worker-ii. La final, se 
afiseaza asemanarile(daca trec de pragul impus) pentru fiecare fisier in 
formatul cerut.