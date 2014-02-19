function x=triCL(A,b)
	n=length(b);
	x=zeros(n,1);
	[nzval colind rowptr]=clFormat(A);
	for i=1:n
		for j=rowptr(i):(rowptr(i+1)-1)
			if(i != colind(j))
				x(i)=x(i)+nzval(j)*x(colind(j));
			endif
		end
		x(i)=(b(i)-x(i))/A(i,i);	%stim ca elementul diagonal este nenul
	end
endfunction		
