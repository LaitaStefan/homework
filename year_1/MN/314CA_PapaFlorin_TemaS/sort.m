function [v p]=sort(PR1)
	n=length(PR1);
	p=PR1;
	for i=1:n-1
		for j=i+1:n
			if(p(j)>p(i))
				aux=p(j);
				p(j)=p(i);
				p(i)=aux;
			endif
		end
	end
	for i=1:n
		for j=1:n
			if(p(i)==PR1(j))
				v(i)=j;
				count=0;
				for k=1:i-1
					if(v(k)==j)
						count++;
					endif
				end
				if(count==0)	
					break;
				endif
			endif
		end
	end		
endfunction				