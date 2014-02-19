function [nzval rowind colptr]=ccFormat(A)
	[n n]=size(A);
	nzval=zeros(1,2);
	rowind=zeros(1,2);
	colptr=zeros(1,2);
	c=0;
	for i=1:n
		cont=0;
		for j=1:n
			if(A(j,i)!=0)
				if(cont==0)
					colptr(i)=c+1;
				endif
				c++;
				nzval(c)=A(j,i);
				rowind(c)=j;
				cont++;
			endif
		end
		if(cont==0)
			colptr(i)=0;
		endif
	end
	colptr(n+1)=c+1;
endfunction	
