function [a b]=getVal(file)
	in=fopen(file,"r");
	a=fgets(in);
	a=str2num(a);

	for i=1:a
		fgets(in);
	end
	
	a=fgets(in);
	b=fgets(in);

	a=str2num(a);
	b=str2num(b);
endfunction		