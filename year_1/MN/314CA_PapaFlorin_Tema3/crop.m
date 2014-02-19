function crop(input)
	A=imread(input);
	[n m]=size(A);

	B=A(n/4+1 : n - n/4, m/4 + 1 : m - m/4);
	
	out=strcat("out_crop_",input);
	afisare(B,out);
end	