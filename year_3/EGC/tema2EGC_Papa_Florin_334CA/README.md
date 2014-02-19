Elemente de grafica pe calculator - Tema 2

This project implements a 3D obstacle game. The project is written in Visual
Studio 2012 using OpenGL. The project can be run by opening it in Visual Studio
and pressing the "Run" button.


Crazy Driver
Papa Florin, 334CA

Cuprins:
1. Cerinta
2. Utilizare
3. Implementare
4. Testare
5. Probleme aparute
6. Continutul arhivei
7. Functionalitati

1. Cerinta
	Utilizand framework-ul laboratorului 3, a trebuit sa cream un joc 3D
wireframe de masini cu obstacole.

2. Utilizare
	In ceea ce priveste utilizarea jocului propus de mine, jucatorul este 
reprezentat de masina neagra cu geamuri albe care apare in partea de jos, in
centrul ecranului. Aceasta se misca prin intermediul tastelor up, down,	right
si left. O apasare a tastei left sau right produce mutarea masinii de pe o 
banda pe alta, in situatia in care exista o banda in directia specificata.
Mutarea se face prin rotatia corpului masinii, dupa care urmeaza translatia 
pe banda dorita. Scopul este parcurgerea unei distante de 3000 de metri,
adunand un punctaj cat mai mare(prin evitarea obstacolelor). Jocul se termina
in momentul in care am ramas fara carburant, am terminat distanta sau ne-am
lovit de obstacole de mai mult de 5 ori. Carburantul se consuma mai agresiv 
cand se accelereaza si mai lent cand nu. La fecare 90 de secunde, apare un 
bonus care ofera carburant jucatorului pentru a putea parcurge distanta
propusa. Obstacolele sunt fie fixe(corpuri geometrice), fie mobile(masini de 
culoare rosie). In cazul in care am atins un scor record, se va afisa un mesaj 
corespunzator si se va salva scorul intr-un fisier text pentru comparatii 
ulterioare. Dimensiunea implicita a ferestrei de afisare eset 1400x800, insa se
poate mari sau micsora fereastra in functie de necesitati.

3. Implementare
	Implementarea jocului a fost facuta in Visual Studio peste framework-ul pus
la dispozitie in cadrul laboratorului 3. Pe langa clasele existente, am adaugat
urmatoarele clase suplimentare, pentru modularizare si vizibilitate a codului: 
Background, Car, Collisions, Cube, FuelCylinder, Road, Stripes, Tethra, Wheel, 
WireSphere.	Programul se ruleaza deschizand tema2EGC.sln, dupa care se apasa 
F5(Run).

4. Testare
	Testarea s-a facut in Visual Studio 2012 pe calculatorul personal, 
verificand functionalitatile implementate.

5. Probleme aparute
	O problema majora cu care m-am confruntat a fost faptul ca framework-ul nu
oferea rezultatele asteptate pentru proiectii perspectiva. Problema a fost
rezolvata prin efectuarea unor modificari in framework propuse pe forumul
temei.

6. Continutul arhivei
	Arhiva contine, pe langa framework-ul de la laborator, fisierele:

	- README - acest fisier
	- Background.h - colorarea drumului cu gri si delimitarea acestuia de 
					 restul fundalului folosind linii negre
	- Car.h - genereaza masina si o muta in punctul specificat
	- Collisions.h - generarea inamicilor si a cilindrului cu benzina, precum
					 si gestionarea coliziunlor
	- Cube.h - genereaza un cub
	- FuelCylinder.h - genereaza un cilindru 2/3 plin
	- Road.h - genereaza drumul
	- Stripes.h - genereaza dungile de delimitare a benzilor 
	- Tethra.h - genereaza un tetraedru
	- Wheel.h - genereaza o roata
	- WireSphere.h - genereaza o sfera
	- highscore.txt - contine recordul curent

7. Functionalitati
	Jucatorul se poate misca inainte, stanga sau dreapta sau poate frana
folosind sagetile. Masina se translateaza de pe o banda pe alta efectuand si
rotatie in acelasi timp. Modalitatea prin care am obtinut acest efect este
implementarea unei functii care reactioneaza la ridicarea unei taste
(onReleaseKey - main), pentru care au fost facute modificarile necesare in 
fisierele DrawingWindow.h si DrawingWindow.cpp(adaugarea functiilor 
glutSpecialUpFunc, keyboardRelease). In momentul in care a fost apasata o tasta
incepe translatia si rotatia masinii, care se termina doar in momentul in care 
masina a ajuns pe banda ceruta. Miscarea inainte se face cu acceleratie, pana
la o anumita viteza maxima. Si pentru acest lucru a fost folosita functia de
eliberare a unei taste. In momentul in care tasta UP este apasata, se consuma
mai mult carburant decat atunci cand nu este apasata. Tasta DOWN franeaza 
masina.
	Perspectiva din care se vede totul este un punct aflat la (0, 250, 760), 
masina aflandu-se pe axa Ox la 430. Toate obiectele sunt generate pe y = 0
(inaltime 0). Perspectiva nu se modifica, actionand ca o camera aflata mereu in
spatele masinii.	
	In ceea ce priveste tratarea coliziunilor, fiecare obiect este aproximat 
de o sfera cu raza de 35. In momentul in care se produce o suprapunere a
acestor sfere, se considera ca s-a produs o coliziune. 
	In functia init din main se adauga elementele de baza: contextele vizuale,
masina, drumul, dungile, scorul, timpul etc. Contextele vizuale sunt 4 la
numar, 2 dintre ele folosite ca si container pentru obiecte(3D sau text), iar
celelalte 2 sunt aplicate peste cele initiale pentru a le colora, pastrand
afisarea elementelor acestora. 
	In functia onIdle din main se genereaza noi inamici(unul la fiecare 0.5 
secunde), se verifica coliziunile cu acestia si se actualizeaza datele 
afisate. In cazul in care s-a terminat jocul, se afiseaza un mesaj 
corespunzator si se elimina toti inamicii. Daca am realizat un scor record,
se afiseaza un mesaj si se suprascrie valoarea anterioara continuta de fisier.

