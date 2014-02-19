function afisare(matrice_rezultat,nume_fisier_iesire)

matrice_rezultat = matrice_rezultat';
[n m]=size(matrice_rezultat);

out = fopen(nume_fisier_iesire,'w');

fprintf(out, "P2\n"); %%tipul fisierului

fprintf(out, "%i %i\n", n, m);

fprintf(out, "%i\n", 255); %%max = 255 = valoarea_maxima_din_matricea_rezultat

fprintf(out,"%i\n", matrice_rezultat);

fclose(out);
