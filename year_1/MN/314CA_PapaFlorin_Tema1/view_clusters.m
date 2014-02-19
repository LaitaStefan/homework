% outputs a graphical representation of the clustering solution
function view_clusters(points, centroids)

	[NC m]=size(centroids); % NC retine numarul de clustere
	[n m]=size(points);		% n retine numarul de puncte din matricea de intrare

	v=zeros(n,1);	% vector care retine clusterul din care face parte fiecare punct
	for i=[1:n]
		min=norm(points(i,:)-centroids(1,:));
		v(i)=1;
		for j=[2:NC]
			if (norm(points(i,:)-centroids(j,:)) < min )
				min=norm(points(i,:)-centroids(j,:));
				v(i)=j;
			end
		endfor
	endfor
	
	scatter3(points(:,1),points(:,2),points(:,3),[],v(:)) % culorile in care apar punctele depind de clusterul din care face parte fiecare, intrucat la 
														  %	campul care specifica culoarea este pus vectorul v. Asadar, fiecare cluster va fi colorat 															  %	diferit
				
end

