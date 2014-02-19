Elemente de grafica pe calculator - Tema 1

This project contains a 2D game implemented in OpenGL. It was written in Visual
Studio 2012 and can be run by opening the project in Visual Studio. 


	- Numele temei: Geometry Wars
	- Papa Florin, 334CA
	
Cuprins
	1.Cerinta
	2.Utilizare
	3.Implementare
	4.Testare
	5.Probleme Aparute
	6.Continutul Arhivei
	7.Functionalitati	
	
1.Cerinta
	Utilizand framework-ul oferit la laborator, a trebuit sa implementam un 
	joc de tipul Geometry Wars, in care jucatorul este reprezentat de un 
	obiect geometric si se poate misca pe o harta cu scopul eliminarii unor 
	inamici.
	
2.Utilizare
	Tema a fost implementata peste framework-ul oferit la laboratorul 2, asadar
	se ruleaza din Visual Studio apasand butonul "Run" sau F5. In momentul in 
	care a aparut fereastra de vizualizare, aceasta poate fi marita sau
	micsorata dupa preferinte(se poate si fullscreen). Dimensiunea implicita 
	este 1400x800. Jucatorul se poate misca inainte si inapoi folosind sagetile
	inainte si inapoi. Rotatia stanga sau dreapta se efectueaza la apasarea 
	sagetilor stanga sau dreapta. Pentru a trage cu gloante, se apasa tasta
	space. La apasarea tastei Escape, se iese din program.
	La terminarea jocului, se verifica daca scorul obtinut este un nou record,
	scorul record fiind retinut in fisierul highscore.txt din folderul lab2.
	
3.Implementare
	Tema a fost implementata in Visual Studio folosind framework-ul din cadrul
	laboratorului 2. Modul in care se ruleaza programul este descris in 
	sectiunea 2.Utilizare.
	
4.Testare
	Testarea temei s-a facut manual, prin verificarea functionalitatilor 
	implementate.
	
5.Probleme aparute
	Problema principala a fost implementarea unei solutii astfel incat
	jucatorul sa se poata misca inainte sau inapoi si sa se si roteasca in 
	acelasi timp(apasarea a 2 taste simultan). Pentru aceasta, am introdus 
	o functie noua in DrawingWindow.h si DrawingWindow.cpp numita 
	KeyboardRelease, care are acelasi model cu functia SpecialFunction. Aceasta
	reactioneaza la eliberarea unei taste si apeleaza functia onReleaseKey din
	main.
	
6.Continutul arhivei
	Arhiva contine, pe langa framework-ul de la laborator, fisierele README
	si Enemy1.h, Enemy2.h, Enemy3.h, Enemy4.h care implementeaza inamicii si 
	functionalitatile acestora(generare, miscare, eliminare etc.).
	
7.Functionalitati
	Jucatorul se poate misca inainte si inapoi si se poate roti in jurul 
	propriei axe(folosind sagetile) si poate trage cu gloante triunghiulare 
	catre inamici. Inamicii sunt implementati sa plece dintr-o pozitie fixa si
	sa urmareasca jucatorul. In momentul in care sunt atinsi de gloante, se 
	verifica daca au primit numarul de lovituri necesari pentru a fi eliminati,
	dupa care sunt mutati in pozitia lor initiala, crescand scorul. Daca nu 
	au primit suficiente lovituri, sunt eliminate numai gloanetele. Daca
	inamicii se lovesc de jucator, acestuia i se scade o viata, iar inamicul 
	este dus in pozitia sa initiala. La fiecare 1000 de puncte se trece la 
	nivelul urmator, care presupune cresterea vitezei jucatorului si a 
	inamicilor. La fiecare 5000 de puncte apare o viata suplimentara 
	care poate fi colectata prin impuscare sau prin contact direct.
	
	In ceea ce priveste tratarea coliziunilor, fiecare obiect este aproximat de
	un cerc cu dimensiune variabila(depinde de deimensiunea obiectului), iar in
	in momentul in care se verifica aceste coliziuni, este suficient sa se 
	verifice daca distanta euclidiana intre centrele a 2 obiecte este mai mica 
	sau egala cu suma razelor cercurilor aferente.
	
	La finalul jocului(am ramas fara vieti) sunt eliminate toate obiectele din 
	contextul vizual si se afiseaza mesajul "Game over". In cazul in care am
	atins un highscore, sub mesajul anterior este afisat mesajul "Achieved
	new highscore". Scorul maxim este retinut in fiserul highscore.txt din
	folderul lab2.