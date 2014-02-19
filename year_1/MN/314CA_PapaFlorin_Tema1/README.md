Papa Florin
314ca

This homework contains an implementation of the K-Means Clustering Algorithm.
The input is read from 2 files, one containing the number of clusters we want 
to form and another file containing the actual 2D points. The "view_clusters"
function displays the clusters given in the input files using the Octave 
function "scatter3".

Versiunea de octave in care a fost rezolvata tema este 3.2.4.

Pentru implementarea functiei read_input_data am folosit instructiunea "load"
de 2 ori, o data pentru fisierul care contine numarul de clustere si o data 
pentru fisierul care contine punctele. Inainte de a citi fisierul care contine
punctele, a trebuit sa specific formatul dorit("format long") pentru a incarca 
toate zecimalele coordonatelor. Aici am intampinat putine dificultati deoarece 
reusisem sa implementez si functia clustering_pc, dar rezultatele nu erau 
corecte din cauza numarului de zecimale.

Pentru implementarea functiei clustering_pc a trebuit intai sa aflu numarul de 
linii al matricii points folosind "size". Apoi am ales doi contori(c=1 si i=1) 
pentru a forma matricea centroids. Contorul c urmareste numarul de linii al 
matricii centroids, iar i, numarul de linii al matricii points. In cazul in 
care c==1, matricea centroids nu are niciun element, deci alegem primul punct 
din points si il introducem in centroids. In caz contrar, trebuie sa verificam 
daca punctul curent din points exista deja in centroids. Daca nu exista, il 
retinem. Facem acest lucru pana cand c devine egal cu NC sau pana cand i devine
egal cu numarul de linii al matricii points(se ajunge la sfarsitul matricii).
In continuare, intr-o bucla while, asociem fiecare punct din points unui anumit
cluster, in functie de distanta euclidiana a punctului fata de centrul 
clusterului. Numarul clusterului din care face parte fiecare punct este salvat 
intr-un vector care are acelasi numar de elemente ca si numarul de puncte.
Urmatorul pas este sa calculam media coordonatelor punctelor dintr-un cluster, 
care devine noul centroid. Pentru a face acest lucru, parcurgem vectorul v, 
apoi matricea points si salvam, pe rand, intr-o noua matrice, punctele care fac
parte din acelasi cluster, dupa care facem media acestei matrici
(folosind mean). Media fiecarei astfel de matrice reprezinta un nou centroid. 
Facem acest lucru pana cand matricea centroids nu se mai modifica(se verifica 
salvand-o la inceputul unei bucle while si comparand-o cu cea de la sfarsitul 
buclei). In cazul in care un cluster contine un singur punct, alegem ca si 
centroid chiar punctul pe care il contine.

Pentru functia compute_cost_pc am initializat costul cu 0 si am aflat 
dimensiunile matricilor points si centroids. In continuare, am asociat fiecare
punct unui anume centroid folosind acelasi algoritm de la functia anterioara. 
Pentru a calcula costul total, am parcurs fiecare cluster, calculand distanta 
euclidiana de la fiecare punct al sau la centroid, adaugand aceasta valoare la 
costul total.

Pentru functia view_clusters, a trebuie intai sa asociez fiecare punct din 
points unui anumit cluster folosind algoritmul folosit si in functiile 
anterioare. Pentru trasarea graficului am folosit scatter3, dandu-i ca 
parametri cele 3 coloane ale matricii points, un vector gol(care controleaza 
grosimea punctelor afisate) si vectorul v care retine carui cluster ii apartine
fiecare punct(acest ultim parametru controleaza culoarea de afisare). Astfel, 
fiecare cluster va fi colorat diferit.

Pentru implementarea functiei view_cost_vs_nc am folosit, in primul rand, 
instructiunea load pentru a citi fisierul de intrare. Am considerat vectorul 
cost care are 10 elemente, retinand costul fiecarei implementari
(cu 1,2,....,10 clustere). Pentru fiecare valoare de la 1 la 10, am aplicat 
algoritmul de clustering(functia clustering_pc) si am calculat costul aferent
(folosind functia compute_cost_pc). La final, am trasat graficul costului in 
functie de numarul de clustere(folosind plot). 