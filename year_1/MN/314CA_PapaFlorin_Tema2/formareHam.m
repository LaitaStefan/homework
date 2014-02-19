function cod=formareHam(sir)
	sir=turn2vect(sir);
	n=length(sir);
	pow=0;
	while( 2^pow <= n)
		pow++;
	end
	lung=n+pow;
	
	if(lung>=2^(pow)) 
		lung++;
		pow++;
	endif

	cod=zeros(1,lung);
	c=1;
	i=1;
	while c<=lung
		if(putere2(c)!=1)
			cod(c)=sir(i);
			i++;
		endif
		c++;
	end

	for p=0:pow-1
		cont=0;
		for i=2^p:lung
			if(rem(i,2*(2^p))>=2^p)
				if(rem(i,2*(2^p))<=(2*(2^p)-1))
					if(cod(i)==1)
						cont++;
					endif
				endif		
			endif
		end
		cod(2^p)=rem(cont,2);
	end
	cod=turn2string(cod);
endfunction	
					
