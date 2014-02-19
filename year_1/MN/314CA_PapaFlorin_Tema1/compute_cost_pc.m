% computes a clustering solution total cost
function cost = compute_cost_pc(points, centroids)
	cost = 0; %initializare cost cu 0
	[n m]=size(points);	% n retine numarul de puncte din matricea de intrare
	[NC m]=size(centroids);	% NC retine numarul de clustere

	v=zeros(1,n);	%vector care retine clusterul din care face parte fiecare punct
	for i=[1:n]
		min=norm(points(i,:)-centroids(1,:));
		v(i)=1;
		for j=[2:NC]
			if (norm(points(i,:)-centroids(j,:)) < min )		%algoritmul de asociere punct-cluster este acelasi de la taskurile precedente
				min=norm(points(i,:)-centroids(j,:));
				v(i)=j;
			endif
		endfor
	endfor

	for i=[1:NC]
		for j=[1:n]
			if (v(j)==i)
				cost+=norm(points(j,:)-centroids(i,:));		% incrementam costul cu distanta euclidiana a fiecarui punct fata de centroidul clusterului din 															% care face parte
			endif
		endfor
	endfor

end

