function [nzval colind rowptr]=clFormat(A)
	[n n]=size(A);
	nzval=zeros(1,2);
	colind=zeros(1,2);
	rowptr=zeros(1,2);
	c=0;
	for i=1:n
		cont=0;
		for j=1:n
			if(A(i,j)!=0)
				if(cont==0)
					rowptr(i)=c+1;
				endif
				c++;
				nzval(c)=A(i,j);
				colind(c)=j;
				cont++;
			endif
		end
		if(cont==0)
			rowptr(i)=0;
		endif
	end
	rowptr(n+1)=c+1;
endfunction	
