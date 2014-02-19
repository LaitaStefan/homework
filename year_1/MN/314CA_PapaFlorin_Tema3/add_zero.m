function add_zero(input)
	A=imread(input);
	[n m]=size(A);

	B(1:2:2*n-1,1:2:2*m-1)=A;
	
	n=length(input);
	name=input(10:n);
	out=strcat("out_add_zero_",name);
	afisare(B,out);
end			
