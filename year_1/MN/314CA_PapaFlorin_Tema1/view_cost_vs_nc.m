function view_cost_vs_nc(file_points)
	
	load(file_points);
	cost=zeros(10,1);	%vector care retine costul implementarilor cu 1,2,....,10 clustere

	for i=[1:10]
		centroids=zeros(1,3);
		centroids=clustering_pc(points,i); 				%aplicam functia clustering_pc pentru i=[1:10]
		cost(i)=compute_cost_pc(points,centroids);		%calculam costul aferent si il salvam in vectorul cost()
	endfor

	plot([1:10],cost(:));		%trasare grafic cost in functie de numarul de clustere
end

