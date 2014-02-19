Papa Florin, 324CA

This is a Clips implementation of a "phutball" game. The ball is placed on a 
map described by facts. The map also contains men and the ball can only move by
jumping over men. The aim is to decide whether the ball can reach the lowest
row of the field. This is achieved by defining rules with different priorities
(salience in Clips) and analysing every possible move until reaching our goal
or until we run out of moves.


Regula cu cel mai mare salience(10) o reprezinta cea care verifica daca s-a 
ajuns pe ultima linie a terenului. Face acest lucru verificand coordonatele 
mingii dintr-un world si asertand un fapt de win daca a gasit o minge pe 
ultima linie. In continuare, cu salience de 9 am regulile removeWorldsAfterWin,
removeMenAfterWin, removePosAfterWin, care indeparteaza toate faptele 
suplimentare pentru un anumit ID daca s-a gasit un fapt de win.

In cazul in care am pierdut, se aplica regulile removePosAfterLoss, 
removeWorldsAfterLoss, removeMenAfterLoss, care au salience -1, cel mai mic 
dintre toate regulile. Asadar, acestea se aplica doar in cazul in care nu mai 
am mutari de facut si nu s-a castigat pana atunci. Ele scot toate faptele in 
afara de initial-fact.

Regulile care verifica posibilitatea de a merge pe o anumita directie au 
salience 5 si sunt 8 la numar, una pentru fiecare directie posibila(up, 
up-right, right, down-right, down, down-left, left, up-left). In cazul in care 
se detecteaza un om in apropierea mingii, se aserteaza un fapt de tip 
(possible ?dir1 ?dir2 ?x ?y ?hops ?id $?men), unde ?dir1 si ?dir2 reprezinta 
directia pe care s-a gasit omul, ?x si ?y reprezinta pozitia actuala a mingi, 
?hops reprezinta de cate ori putem sari in directia respectiva, ?id este id-ul 
lumii pe care lucram, iar $?men reprezinta toti oamenii de pe harta. A fost 
necesar sa tin si oamenii deoarece altfel se facea match cu situatii gresite.

Regula incMove are acelasi salience cu regulile de testare directii si 
realizeaza incrementarea numarului de hop-uri daca gaseste oameni de-a lungul 
directiei continute in faptul possible.

Regulile moveInit si moveAfter au salience 0 implicit si sunt folosite pentru 
a realiza mutarea, in momentul in care nu mai sunt incrementari de facut la 
numarul de hop-uri de la possible. Aceste reguli sunt identice deoarece a 
trebuit sa fac diferenta intre cazul in care nu am nicio mutare la moves si 
cazul in care s-au realizat deja mutari. In primul caz, trebuie sa pun in acest
camp si pozitia de unde am plecat, iar in celalalt caz trebuie sa pun doar 
pozitia in care am ajuns. Ele fac match intre o lume si un fapt possible 
aferent acelei lumi, dupa care inlatura faptul de tip possible, adauga un nou 
fapt de tip eliminateMen care contine exact aceleasi campuri ca si possible 
(cu exceptia ca directia este inversata si pozitia mingii este inlocuita cu 
pozitia acesteia dupa mutare) si un nou fapt de tip world cu mingea in pozitia 
finala. Mai ramane de scos oamenii de pe directia pe care am venit, lucru 
realizat de regulile eliminateMen si eliminateMen2.

Aceste 2 reguli au salience 6, pentru a fi prioritare oricarei reguli in afara 
de win. Dupa ce adaug o lume noua, primul lucru ce trebuie facut este sa 
inlatur oamenii de pe directia pe care am venit. Acest lucuru se face facand 
match intre pozitiile oamenilor si directiile inmultite cu numarul de hop-uri, 
in cazul in care acesta este strict pozitiv.In continuare, se scoate faptul de 
tip removeMen si este inlocuit de altul cu numarul de hop-uri mai mic cu o 
unitate si se scot oamenii din campul men. Din nou, a trebuit sa fac diferenta 
intre cazul in care am un singur om si cazul in care am mai multi pentru a 
putea aserta fapte potrivite.

Cam asta e tema :)