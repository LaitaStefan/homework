Elemente de grafica pe calculator - Tema 3
Nume tema: "Atletism"
Papa Florin 334CA

This project implements a 3D racing game. The project is written in Visual
Studio 2012 using OpenGL and shaders. The project can be run by opening it in 
Visual Studio and pressing the "Run" button.

Cuprins
	1.Cerinta
	2.Utilizare
	3.Implementare
	4.Testare
	5.Probleme Aparute
	6.Continutul Arhivei
	7.Functionalitati
	
1. Cerinta
	Se cere sa se implementeze o cursa de atletism intr-o scena 3D.

2. Utilizare
	Tema a fost realizata in Visual Studio 2010, asadar rularea programului se 
face deschizand fisierul "tema3.sln" si apasand tasta F5. Jucatorul este
reprezentat de masina rosie, iar adversarii de masinile verde, respectiv 
albastra. Adversarii se deplaseaza cu viteza constanta, citita dintr-un fisier,
iar jucatorul se deplaseaza cu ajutorul sagetilor. Jucatorul are libertate de 
miscare deplina in interiorul pistei(nu poate iesi insa de pe teren), iar
adversarii se pot misca numai pe benzile pe care sunt pusi inca de la inceput.
Pentru vizualizare, sunt disponibile 5 camere, in aceasta ordine: o camera 
mobila care "priveste" de deasupra stadionului, o camera care priveste din
perspectiva jucatorului, 2 camere care privesc din perspectivele adversarilor si 
o camera care urmareste jucatorul de pe marginea stadionului.
	Taste de control:
		-sageata sus - miscare inainte jucator
		-sageata jos - miscare inapoi jucator
		-sageata stanga - rotatie stanga jucator
		-sageata dreapta - rotatie dreapta jucator
		-v - schimbare camera
		-w - translatie inainte prima camera
		-s - translatie inapoi prima camera
		-d - translatie dreapta prima camera
		-a - translatie stanga prima camera
		-r - translatie in sus(upworld)
		-f - translatie in jos(downworld)
		-q - rotatie stanga pe Oy prima camera
		-e - rotatie dreapta pe Oy prima camera
		-z - rotatie stanga pe Oz prima camera
		-c - rotatie dreapta pe Oz prima camera
		-t - rotatie dreapta pe Ox prima camera
		-g - rotatie stanga pe Ox prima camera

2.1 Fisiere
	Implementarea temei a pornit de la laboratorul 3 de shadere, la care am 
adaugat fisiere proprii:
	- my_fragment_shader - fragment shader-ul implementat de mine
	- my_shader_vertex - vertex shader-ul implementat de mine
	- CarMovement.h - contine clase care se ocupa cu gestiunea miscarilor
	- utils.h - contine metode care genereaza diferite obiecte

3. Implementare
	Tema a fost realizata pornind de la laboratorul 3 de shadere, in Visual 
Studio 2012. In cadrul laboratorului aveam deja implementate metode pentru a 
importa obiecte din fisiere .obj, insa culoarea acestor obiecte era data in 
functie de normala citita din fisier. Pentru a putea atribui o culoare la 
alegere obiectelor, am adaugat campul "culoare" antetului functiei loadObj(din 
fisierul lab_mesh_loader). Cu ajutorul acestui camp, alterez normalele pentru 
a obtine culorile dorite.
	De asemenea, camerele erau gata implementate, iar pentru a putea alege 
dintre mai multe camere a fost necesara construirea unui vector de camere, 
schimbarea intre acestea facandu-se cu ajutorul unei variabile de tip intreg 
care selecteaza camera curenta.
	Toate obiectele din tema au fost generate cu ajutorul VAO, VBO si IBO. 
Jucatorii si camera urmaritoare sunt generate din fisiere .obj, iar terenul, 
linia de finish si steagul sunt generate "manual"(in utils.h). Masinile si 
camera urmaritoare sunt generate pe directie inversa fata de cea dorita de mine,
asadar, pentru o reprezentare corecta, unghiul de rotatie este initializat la 
valori diferite(in functie de pozitie). Jucatorul este manipulat de la taste, 
fiind posibila apasarea mai multor taste in acelasi timp. Acest lucru este 
realizat prin atribuirea unei variabile fiecarei taste de control, variabile
care iau valoarea 1 atunci cand se apasa tasta(notifySpecialKeyPressed), fiind 
resetate la 0 atunci cand se ridica tasta(notifySpecialKeyReleased). Valorile
tastelor apasate sunt trimise unui obiect de tipul CarMovement, care realizeaza
miscarea in functie de ce taste sunt apasate la un anumit moment de timp. 
Adversarii au viteza constanta si sunt miscati pe pista cu ajutorul unui 
obiect de tipul AdvMovement.
	Pentru a detecta cine a castigat, in cazul adversarilor se verifica doar 
daca au ajuns inapoi in punctul de start. Acest lucru nu este suficient in cazul
jucatorului, acesta putand sa mearga si inapoi. Pentru a detecta corect 
castigul, in momentul in care trece linia de finish in directie gresita se 
decrementeaza un contor, iar cand trece in directia corecta, se incrementeaza. 
Daca atunci cand s-a trecut linia contorul este 0, inseamna ca am castigat si se
afiseaza steagul fluturand.
	Steagul care indica victoria are culoarea castigatorului si are un efect de
fluturare. Acest efect a fost realizat in vertex shader. Pentru afisarea 
steagului folosesc "my_shader_vertex" si "my_fragment_shader". Noul fragment 
shader este identic cu cel oferit in cadrul laboratorului, insa vertex shader-ul 
nou altereaza coordonata z a steagului folosind o sinusoida in functie de 
coordonatele x, y si variabila time, variabila uniforma care este adusa din 
fisierul main. Variabila time este variata la fiecare ciclare pentru ca 
dimensiunea z sa varieze pentru momente diferite de timp, oferind impresia de 
fluturare. Cand un jucator castiga, dispar toate elementele si este afisat numai
steagul.	 

4. Testare
	Tema a fost testata pe calculatorul personal, folosind Visual Studio 2012
in Windows 8. 	

5. Probleme aparute
	Problema principala de care m-am lovit a fost faptul ca miscarile de rotatie
si translatie se realizeaza diferit in OpenGl fata de framework-ul in care au 
fost realizate celelalte 2 teme. Diferenta consta in faptul ca, in framework, 
pentru a realiza rotatia obiectului in jurul propriei axe, trebuia translatat
in origine, aplicata rotatia si dupa translatat in pozitia dorita. In OpenGl, 
prin apelul functiei glm::rotate, se obtine rotatia obiectului in jurul propriei
axe, indiferent de pozitia sa.

6. Continutul Arhivei
	Tema a fost realizata in Visual Studio 2012, pornind de la laboratorul 3 de
shadere. Fisierele adaugate de mine sunt:
	- my_fragment_shader - fragment shader-ul implementat de mine
	- my_shader_vertex - vertex shader-ul implementat de mine
	- CarMovement.h - contine clase care se ocupa cu gestiunea miscarilor
	- utils.h - contine metode care genereaza diferite obiecte
	- README - fisierul citit in acest moment
	- speed.txt - fisier care contine vitezele adversarilor(valori subunitare) 
				  pe linii separate

7. Functionalitati
	Functionalitati standard(cerute in enunt):
	- miscare jucator folosind sagetile
	- miscare adversari cu viteze constante citite din fisier
	- camere FPS atasate jucatorilor
	- camera FPS care priveste stadionul de sus
	- camera TPS care urmareste jucatorul
	- jucatori in culori diferite(rosu, verde, albastru)
	- schimbare intre camere prin apasare de taste
	- steag in culoarea castigatorului
	- miscare steag in vertex shader
	- jucatori, steag si camera atasata jucatorului principal construite 
	  folosind VAO, VBO si IBO