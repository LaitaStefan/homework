function B = invTrSup(A)

	[n, n] = size(A);
	for j = n : -1 : 1
		B(j, j) = 1 / A(j, j);
		for i = j - 1 : -1 : 1
			B(i,j)=-A(i,i+1:j)*B(i+1:j,j)/A(i,i);
		end
	end
	B = triu(B);
endfunction	