function x=triCC(A,b)
	n=length(b);
	x=b;
	[nzval rowind colptr]=ccFormat(A);
	for i=1:n
		x(i)=x(i)/A(i,i);	%stim ca elementul diagonal este nenul
		for j=colptr(i):(colptr(i+1)-1)
			if( i != rowind(j))			
				x(rowind(j))=x(rowind(j))-nzval(j)*x(i);
			endif
		end
	end
endfunction	
