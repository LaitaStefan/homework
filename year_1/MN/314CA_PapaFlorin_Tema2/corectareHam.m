function cod=corectareHam(sir)
	sir=turn2vect(sir);
	n=length(sir);
	pow=0;
	while( 2^pow < n)
		pow++;
	end

	sirpow=zeros(1,pow);
	for i=1:pow
		sirpow(i)=sir(2^(i-1));		%retin bitii de paritate
	end

	corect=zeros(1,pow);
	for p=0:pow-1
		cont=0;
		for i=2^p:n
			if(rem(i,2*(2^p))>=2^p && rem(i,2*(2^p))<=(2*(2^p)-1) && sir(i)==1 && putere2(i)!=1)
				cont++;
			endif
		end
		corect(p+1)=rem(cont,2);
	end
	
	s=0;
	for i=1:pow
		if(sirpow(i)!=corect(i))
			s+=2^(i-1);
		endif
	end
	
	if(s>0)
		if(sir(s)==1) 
			sir(s)=0;
		else
			sir(s)=1;
		endif
	endif		
	cod=turn2string(sir);
end				


