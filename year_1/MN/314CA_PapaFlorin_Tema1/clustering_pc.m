% computes the NC centroids corresponding to the given points using K-Means
function centroids = clustering_pc(points, NC)

	[n m]=size(points);		%	n=numarul de puncte din din matricea de intrare
	centroids = zeros(NC,m);	%	matricea centroids va avea NC linii si m=3 coloane


%formare matrice centroizi	

	c=1; 	%contor care retine numarul de centroizi salvati
	i=1;	%contor care retine numarul de linii parcurse de algoritmul de alegere a centroizilor

	while c<=NC & i<=n	
		if (c == 1)		% daca c=1 inseamna ca nu avem niciun centroid salvat, asadar consideram primul punct din matricea points
			centroids(c,:)=points(c,:);
			c++;
		else
			cont=0;		% contor care verifica daca punctul curent din matrice exista deja in matricea centroids
			for j=[1:1:c]
				if ( points(i,:) == centroids(j,:) )
					cont++;
				endif
			endfor
			if(cont == 0)	% in cazul in care punctul nu exista in matricea centroids, il retinem
				centroids(c,:)=points(i,:);
				c++;
			endif	
		endif
		i++;
	endwhile

	d=0; %contor care retine daca matricea centroids s-a modificat de la ultimul pas

	while d==0
		A=centroids; % salvam matricea centroids intr-o matrice A pentru a o compara la final cu cea modificata

		%asociere puncte-centroizi

		v=zeros(1,n); % vector care retine clusterul din care face parte fiecare punct din matricea points
		for i=[1:n]
			min=norm(points(i,:)-centroids(1,:)); 	%initializam minimul cu distanta de la punctul curent la primul centroid
			v(i)=1;		%consideram initial ca punctul curent face parte din primul cluster
			for j=[2:NC]
				if (norm(points(i,:)-centroids(j,:)) < min )
					min=norm(points(i,:)-centroids(j,:));
					v(i)=j;
				endif
			endfor
		endfor

		%calcul medie
		
		med=zeros(NC,3);	%media punctelor dintr-un cluster
		for i=[1:NC]
			elem=zeros(1,3);	%matrice care retine, pe rand fiecare cluster
			c=1;			%contor care retine numarul de linii al matricii elem	
			for j=[1:n]
				if (v(j)==i)	%daca un punct apartine clusterului curent, il introducem in matricea elem
					elem(c,:)=points(j,:);
					c++;
				endif
			endfor
			centroids(i,:)=mean(elem);  %noile valori ale centroizilor sunt reprezentate de media punctelor clusterelor
		endfor
		
		if (A==centroids)	%daca matricea centroids nu se modifica, d devine diferit de 0 si bucla while se opreste
			d++;
		endif

	endwhile		

	nrelem=zeros(NC,1);  % vector care retine numarul de elemente al fiecarui cluster
	for i=[1:NC]
		for j=[1:n]
			if (v(j)==i)
				nrelem(i)++;
			endif
		endfor
	endfor

	for i=[1:NC]
		if (nrelem(i)==1)	% in cazul in care un cluster contine numai un punct, centroidul aferent acestui cluster este chiar punctul pe care il contine
			for j=[1:n]
				if (v(j)==i)
					centroids(i,:)=points(j,:);
				endif
			endfor
		endif
	endfor
				
end
