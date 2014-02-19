Papa Florin, 324CA

This is a Scheme implementation of a database that can be interogated by a user
that wants to extract various information from the database. The entries can 
have a hierarchical structure(multiple levels of paranthesis). 


Implementarea mea porneste de la rezultatele intoarse de primul query(lista de 
liste de perechi variabila-valoare), pe care le extinde progresiv cu cele 
intoarse de urmatorul query. Daca intre 2 liste ce trebuiesc concatenate exista
conflicte (variabile cu valori diferite), rezultatul concatenarii va fi null. 
In cazul in care query-ul curent este o clauza if, folosesc functia 
replaceVariables - care arunca eroarea "Unbound Variable Exception" daca este 
solicitat un element care nu exista - care inlocuieste in query variabilele cu 
valori si evalueaza conditia.

Selectia variabilelor se face tot in mod progresiv, pornind de la functia 
"selectOne" care intoarce o pereche variabila-valoare daca variabila este 
continuta si arunca eroarea "Unbound Variable Exception" in caz contrar. 
Functia "selectMultiple" aplica "selectOne" pentru fiecare variabila din lista 
primita ca parametru, iar functia "selectAll" intoarce o lista de liste de 
perechi aplicand functia anterioara pe lista de liste primita ca parametru.

Functiile care verifica potrivirea intre 2 liste sunt "match" si "match-final".
Match verifica daca primele doua elemente(care sunt sigur simboluri) se 
potrivesc, dupa care apeleaza match-final pentru a verifica potrivirea pentru 
ultima parte a listelor. Functia "evaluate" intoarce false daca intr-o lista 
primita ca parametru exista cel putin un element #f si este necesara deoarece 
in match-final aplic map pentru a evalua toate elementele dintr-o lista.

Functia valueOf intoarce valoare unei variabile daca se gaseste in lista 
primita ca parametru si null altfel. 

Functionalitatea celorlalte proceduri este descrisa in comentarii inaintea 
fiecareia dintre acestea in fisierul ask.rkt. 

