function s=putere2(a)
	while(a>0)
		if(rem(a,2)==0)
			a=a/2;
		elseif(a!=1)
			a=0;
			s=0;
		else		
			a=0;
			s=1;
		endif
	end
end			