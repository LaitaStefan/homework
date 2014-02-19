function reduce_dimension(input,rate)

	in=imread(input);

	in=in-double(in)*rate/100;		%am convertit elementele matricei in la double
	in=uint8(in);					%pentru a avea o precizie buna la calcul

	out=strcat("out_rd_",num2str(rate),"_",input);
	afisare(in,out);
end	
