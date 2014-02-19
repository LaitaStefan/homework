function bilinear(input)

	A=imread(input);
	[n m]=size(A);

	B(1:2:2*n-1,1:2:2*m-1)=A;
	B=uint16(B);

	n1=2*n-1;
	m2=2*m-1;

	for i=1:n1
		if( mod(i,2)==1 )
			for j=2:2:m2-1
				B(i,j)=(B(i,j-1)+B(i,j+1))/2;
			end
		else
			for j=1:2:m2
				B(i,j)=(B(i-1,j)+B(i+1,j))/2;
				if(j!=m2)
					B(i,j+1)=(B(i-1,j)+B(i-1,j+2)+B(i+1,j)+B(i+1,j+2))/4;
				endif
			end
		endif				
	end

	B=uint8(B);
	n=length(input);
	name=input(10:n);
	out=strcat("out_bilinear_",name);
	afisare(B,out);
end		
