function y=u(x,val1,val2)
	if(x>=0 && x<val1)
		y=0;
	end
	
	if(x>=val1 && x<=val2)
		y=(x-val1)/(val2-val1);
	end
	
	if(x>val2 && x<=1)
		y=1;
	end
endfunction				