function a=turn2string(x)
	n=length(x);
	a=blanks(n);
	for i=1:n
		if(x(i)==0)
			a(i)='0';
		else
			a(i)='1';
		endif
	end
endfunction				