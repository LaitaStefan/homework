function PageRank(file,d,eps)
	outfile=strcat(file,".out");
	out=fopen(outfile,"w");

	PR1=Iterative(file,d,eps);
	n=length(PR1);
	fprintf(out,"%d\n",n);
	for i=1:n
		fprintf(out,"%f\n",PR1(i));
	end

	fprintf(out, "\n");

	PR2=Algebraic(file,d);
	for i=1:n
		fprintf(out,"%f\n",PR2(i));
	end

	[v p]=sort(PR1);
	[val1 val2]=getVal(file);

	fprintf(out, "\n");

	for i=1:n
		y=u(p(i),val1,val2);
		fprintf(out,"%d %d %f\n",i,v(i),y);
	end	

	fclose(out);
endfunction	