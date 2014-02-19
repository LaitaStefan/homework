Elemente de grafica pe calculator - Tema 4
Nume tema: "Sleeping Beauty"
Papa Florin 334CA

This project implements a woman changing her attitude when musical notes appear
in the background. She goes from a sleeping state to an amazed state and then 
to anger and back to sleep. The effects are obtained using three different
textures that are interpolated, a Phong light model implemented in the fragment
shader and a vertex shader for the musical notes that uses alpha discard and 
the billboards technique to keep them parallel with the viewing plane at all
times.

Cuprins
	1.Cerinta
	2.Utilizare
	3.Implementare
	4.Testare
	5.Probleme Aparute
	6.Continutul Arhivei
	7.Functionalitati
	
1. Cerinta
	Se cere sa se implementeze un sistem de animatie faciala ce reactioneaza la
un sistem de particule.	

2. Utilizare
	Tema a fost realizata in Visual Studio 2012, asadar rularea programului se 
face deschizand fisierul "tema4_EGC.sln" si apasand tasta F5. In urma
incarcarii fisierelor obj(care dureaza aproximativ 30 de secunde), pe ecran va
aparea o fata umana care initial doarme, dar este trezita de zgomote 
reprezentate de note muzicale multicolore care apar in spatele ei si devine 
speriata. Dupa ce trece sperietura, devine nervoasa ca a fost trezita, iar apoi
se culca la loc, moment in care dispar si notele muzicale. Acest ciclu se 
repeta la nesfarsit sau pana la apasarea tastei "ESC" care inchide programul.

	Taste de control:
		-ESC - inchidere program
		-space - reincarcare shadere
		-w - afisare wireframe/fill
		-F1 - intrare mod fullscreen
		-F2 - iesire mod fullscreen	

2.1 Fisiere
	Implementarea temei a pornit de la laboratorul 5 de shadere, la care am 
adaugat fisiere proprii:
	- shader_billboard_vertex - vertex shader pentru particule
	- shader_billboard_fragment - fragment shader pentru particule
	- utils.h - contine metoda care genereaza nota muzicala

3. Implementare
	Tema a fost realizata pornind de la laboratorul 5 de shadere, in Visual 
Studio 2012. Pentru a realiza prima parte a temei, animatia faciala, a trebuit
sa construiesc metoda loadMultipleObj din lab_mesh_loader.hpp care imi permite 
sa incarc 3 obiecte diferite in aceeasi mesha pentru a le putea interpola in
vertex shader. Cele 3 meshe au fost incarcate in vectori de vertecsi si indici
diferiti, dupa care toti vertecsii au fost introdusi in acelasi vector. Pentru
fiecare obiect pozitiile, normalele si coordonatele texturale au fost trimise 
pe pipe-uri diferite catre vertex shader pentru a permite interpolarea.
	Pentru a realiza interpolarea, se trimite o variabila uniforma de tip float
care stabileste intre care 2 stari se face tranzitia. Sunt interpolate pozitia
si normala fiecarui vertex. Aceste lucruri se realizeaza in vertex shader. De 
asemenea, am implemantat iluminare de tip Phong, pornind de la laboratorul 4 de
shadere. Spre deosebire de laborator, unde culoarea era trimisa ca variabila 
uniforma si calculata o singura data, in cazul temei culoarea se calculeaza de
3 ori, pentru fiecare din componentele r, g, b. Shader-ele folosite pentru fata
se numesc "shader_vertex" si "shader_fragment".
	Pentru cea de-a doua parte a temei, notele muzicale, am folosit un alt
shader din cauza cerintelor diferite. In vertex shader, se calculeaza noua 
pozitie a unui vertex in functie de pozitia centrului. Centrul este acelasi
pentru fiecare particula, originea sistemului de axe. Acest punct este mutat in
view space, aplicandu-i matricea de proiectie, vizualizare si modelare. Dupa
aceasta, se aduna coordonatele vertex-ului pentru a obtine mereu o fata 
paralela cu ecranul. In fragment shader, pentru ca numai nota muzicala sa fie 
vizibila, fara dreptunghiul care o incadreaza, se foloseste discard pentru 
culoarea alb, pastrandu-se numai negrul in cazul in care variabila uniforma
has_alpha este setata la 1. Daca fragmentul nu a fost aruncat, este colorat in
culoarea specificata de normala la suprafata. Shader-ele folosite pentru 
particule se numesc "shader_billboard_vertex" si "shader_billboard_fragment".
	Functia drawNote creeaza un dreptunghi format din 2 triunghiuri, pe care se
va aplica textura. Notele muzicale sunt tinute intr-un vector, impreuna cu 
matricile de modelare aferente. Pozitia initiala a fiecarei note este generata
random. In functia de desenare, se verifica variabila care stabileste intre ce
stari se face tranzitia in acel moment. Notele muzicale apar numai daca se face
tranzitie intre starea dormind si surprinsa sau intre surprinsa si nervoasa.
Cand se trece de la nervoasa la somn, notele muzicale se opresc.

4. Testare
	Tema a fost testata pe calculatorul personal, folosind Visual Studio 2012
in Windows 8. 	

5. Probleme aparute
	Problema principala de care m-am lovit a fost incarcarea a 3 fisiere obj in
acceasi mesha pentru a face interpolarea. Dupa ce mi-am dat seama ce offset 
trebuie folosit pentru a incarca acelasi vector in mai multe pipe-uri diferite,
problema s-a rezolvat. Rezolvarea temei a fost facilitata si de faptul ca multe
lucruri au fost facute in prealabil la laborator.

6. Continutul Arhivei
	Tema a fost realizata in Visual Studio 2012, pornind de la laboratorul 5 de
shadere. Fisierele adaugate de mine sunt:
	- shader_billboard_fragment - fragment shader-ul implementat pentru 
								particule
	- shader_billboard_vertex - vertex shader-ul implementat pentru particule
	- utils.h - contine metoda care genereaza o nota muzicala
	- README - fisierul citit in acest moment

7. Functionalitati
	Functionalitati standard(cerute in enunt):
	- iluminare Phong
	- schimbare expresie faciala intre somn, surprinsa si nervoasa la nesfarsit
	- interpolare efectuata in vertex shader
	- sistem de particule
	- billboards
	- pozitii calculate in vertex shader
	- folosire alpha discard
	- sincronizare intre animatie si particule