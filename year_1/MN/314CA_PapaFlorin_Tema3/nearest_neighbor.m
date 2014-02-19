function nearest_neighbor(input)
	A=imread(input);
	[n m]=size(A);

	B(1:2:2*n-1,1:2:2*m-1)=A;
	B(2:2:2*n,1:2:2*m-1)=A;			%formare matrice output
	B(1:2:2*n-1,2:2:2*m)=A;
	B(2:2:2*n,2:2:2*m)=A;
	
	n=length(input);
	name=input(10:n);		%formare nume fisier output
	out=strcat("out_nn_",name);

	afisare(B,out);
end			
