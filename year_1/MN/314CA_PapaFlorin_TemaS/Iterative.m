function PR=Iterative(file,d,eps)
	in=fopen(file,"r");
	a=fgets(in);
	a=str2num(a);
	b=zeros(2,1);
	lth=zeros(a,1);

	count=1;
	L=zeros(a,1);	
	for i=1:a
		v=fgets(in);
		v=str2num(v);
		L(i)=v(2);
		lth(i)=length(v)-2;
		for j=3:lth(i)+2
			b(count)=v(j);
			if(v(j)==i)
				L(i)--;
			end
			count++;
		end	
	end	

	count=1;
	B=zeros(a,a);	%matricea de adiacenta
	for i=1:a
		for j=count:count+lth(i)-1
			B(i,b(j))=1;
		end
		B(i,i)=0;
		count=count+lth(i);
	end

	M=zeros(a,a);
	for i=1:a
		for j=1:a
			if(B(j,i)!=0)
				M(i,j)=1/L(j);
			end
		end
	end
	

	R=zeros(a,1);
	P=zeros(a,1);
	R=1/a*ones(a,1);

	rand_v=ones(a,1)*(1-d)/a;
	P=d*M*R+rand_v;
	while(norm(P-R)>eps)
		R=P;
		P=d*M*R+rand_v;
	end

	PR=R;
	fclose(in);

endfunction	