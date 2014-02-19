Papa Florin
314 CA

This homework contains functions that operate on pgm images and decrease the 
value of each color, crop and increase the size of the image. The increasing 
is made in three ways, the first by doubling each value, the second by 
introducing additional blank lines and columns, and the third by interpolating 
the values in the new lines and columns between the adjacent original values.

Tema a fost rezolvata in Octave 3.2.4 in Ubuntu. 

Din cauza erorilor provocate de instructiunea imwrite, am folosit functia 
"afisare" pentru formarea imaginilor de output, functie al carui schelet l-am 
luat de pe forumul temei 3. Am comparat outputul obtinut de mine cu cel din 
arhiva de resurse si matricile fiecarei imagini sunt identice. Dimensiunile 
fisierelor difera deoarece functia "afisare" oferita ca si alternativa pentru 
"imwrite" nu face si compresie. Am intrebat pe forum si mi s-a spus ca este in 
regula sa folosesc aceasta functie pentru obtinerea imaginilor.

Pentru Task 1, in operatia care efectueaza scaderea elementelor imaginii 
initiale cu un anumit procent, a trebuit sa fac cast la double matricii de 
intrare(care este in mod normal in format uint8) deoarece atunci cand o 
comparam cu imaginea corecta de output obtineam diferente de 1 unitate in cazul
imaginii mountain.pgm. Aceasta masura a rezolvat problema.

Pentru Task 2, am selectat din matricea initiala de dimensiune nXm doar partea 
centrala de dimensiune (n/2)X(m/2). Am facut acest lucru selectand elementele 
incepand cu linia n/4+1 si terminand cu linia n-n/4 si elementele de pe coloana
incepand cu m/4+1 si incheind cu m-m/4.

Pentru Task 3.1, am considerat matricea nou formata ca fiind formata din 
patrate de cate 4 elemente din matricea initiala. Pentru a umple aceste 
patrate, am parcurs matricea noua in 4 moduri(pentru fiecare colt al 
patratului), de fiecare data sarind cate un element, egaland-o de fiecare data 
cu matricea initiala. Aceasta metoda este mult mai rapida decat daca se 
parcurge matricea element cu element si se egaleaza cu cea initiala.

Pentru Task 3.2.1, am parcurs matricea nou formata pe pozitiile sale impare si 
am egalat-o cu matricea initiala. Elementele neparcurse se initializeaza 
automat cu 0.

Pentru Task 3.2.2, am folosit taskul anterior, obtinand o matrice cu elementele
celei initiale pe pozitiile impare si 0 in rest. Pentru a introduce media 
elementelor pe pozitiile cu elemente nule parcurg matricea nou formata pe 
linii. In cazul in care linia pe care ma aflu este impara, trebuie sa introduc 
pe aceasta linie pe pozitiile pare media dintre vecinii din stanga, respectiv 
dreapta. In caz contrar, pe o linie para pe pozitiile impare introduc media 
aritmetica dintre vecinii de sus, respectiv de jos. Pe pozitiile pare introduc 
media aritmetica dintre vecinii din colturile imediat alaturate. Precizez ca 
am transformat matricea de iesire in uint16 pentru a putea face media 
aritmetica intr-un mod cat mai eficient(in uint8, daca fac suma elementelor, 
daca aceasta depaseste  255 se trunchiaza la 255).