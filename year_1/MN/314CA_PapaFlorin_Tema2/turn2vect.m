function x=turn2vect(a)
	n=length(a);
	x=zeros(n,1);
	for i=1:n
		if(a(i)=='0')
			x(i)=0;
		else
			x(i)=1;
		endif
	end
endfunction				