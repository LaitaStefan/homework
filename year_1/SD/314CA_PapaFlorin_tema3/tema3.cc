#include "huff.h"
#include "bitmap.h"
#include<string.h>

uint8_t* CreateColorArray(uint8_t *pixels,uint32_t width, uint32_t height)
{
	unsigned int i;
	uint8_t *color;

	color = new uint8_t[256];

	for(i=0;i<256;i++)
	{
		color[i]=0;
	}	

	for(i=0;i<width*height;i++)
	{
		color[pixels[i]]++;
	}	

	return color;
}

void compress(FILE *f, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih,RGBQUAD ctable[256],Huffman<int> huffman,uint8_t *v)
{
	uint16_t aux;
	bfh.bfType = 'M' & 0x00FF;
	aux = 'C' & 0x00FF;
	bfh.bfType = (bfh.bfType<<8)+aux;
	bih.biCompression=BI_MCB;
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,f);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,f);
	fwrite(ctable,sizeof(RGBQUAD),256,f);
	fwrite(huffman.huff,sizeof(SerialHuffmanNode),511,f);
	fwrite(v,sizeof(uint8_t),huffman.dim,f);
}


int main(int argc, char **argv)
{
	char *out;
	FILE *fout,*in;
	BITMAPFILEHEADER bfh; // First bitmap header  
	BITMAPINFOHEADER bih; // Second bitmap header
	RGBQUAD ctable[256]; // The color table
	Huffman<int> huf;
	uint8_t *pixels,*color,*bitstr;
	int n,aux_color[256],i;
	unsigned int tableSize=0;

	in=fopen(argv[2],"rb");
	if (in == NULL) {
		fprintf(stderr, "ERROR: %s cannot be opened!\n", argv[2]);
		return 2;
	}

	if (!fread(&bfh, sizeof(BITMAPFILEHEADER),1, in)) {
				printf("Error reading from file\n");
				return 2;
	}

	if (!fread(&bih, sizeof(BITMAPINFOHEADER),1, in)) {
				printf("Error reading from file\n");
				return 2;
	}

	tableSize = bih.biClrUsed;
	if (tableSize == 0)		tableSize = 256;

	if (fread(ctable, sizeof(RGBQUAD), tableSize, in) < tableSize) {
				printf("Error reading from file\n");
				return 2;
	}

	if(argc>=3 && argc<=4)
	{
		if(strcmp(argv[1],"-c")==0)		//cazul in care se cere compresia
		{
			if (bih.biBitCount != 8 || bih.biClrUsed > 256 ||bih.biClrImportant > 256 || bih.biCompression != BI_RGB) 
			{
				printf("This bitmap is not suitable for My Compressed Bitmap format!\n");
				fclose(in);
				return 0;
			}

			if (( bfh.bfType       & 0x00FF) != 'B' || ((bfh.bfType >> 8) & 0x00FF) != 'M') 
			{
				printf("wrong file type\n");
				fclose(in);
				return 0;
			}

			fseek(in, bfh.bfOffBits, SEEK_SET);
			pixels=new uint8_t[bih.biWidth*bih.biHeight];
			fread(pixels, sizeof(uint8_t), bih.biWidth*bih.biHeight, in);
			color=CreateColorArray(pixels,bih.biWidth,bih.biHeight);
			for(i=0;i<256;i++)
			{
				aux_color[i]=color[i];		//conversie in date de tip int pentru a putea fi 
			}								//format arborele Huffman	
			huf.CreateHuffArray(aux_color);
			huf.codificareHuff(0,0,0);
			bitstr=huf.CreateBitString(pixels, bih.biWidth*bih.biHeight);
			if(argc==3)
			{	
				n=strlen(argv[2]);

				if(n>=4)		//formare nume fisier output
				{	
					if(argv[2][n-4]=='.' && argv[2][n-3]=='b' && argv[2][n-2]=='m' && argv[2][n-1]=='p')
					{
						out = new char[n];
						strncpy(out,argv[2],n-4);
						out[n-4]='.'; out[n-3]='m'; out[n-2]='c'; out[n-1]='b';
					}
					else
					{
						out = new char[n+4];
						strcpy(out,argv[2]);
						out[n]='.'; out[n+1]='m'; out[n+2]='c'; out[n+3]='b';
					}	
				}
				else
				{
					out = new char[n+4];
					strcpy(out,argv[2]);
					out[n]='.'; out[n+1]='m'; out[n+2]='c'; out[n+3]='b';
				}	
				
			}	
			if(argc==4)
			{
				out=argv[3];
			}

			fout=fopen(out,"wb");
			compress(fout,bfh,bih,ctable,huf,bitstr);
		}	
		else
		{
			if(strcmp(argv[1],"-d")==0)		//cazul in care se cere decompresia
			{
				if (bih.biBitCount != 8 || bih.biClrUsed > 256 ||bih.biClrImportant > 256 || bih.biCompression != BI_RGB) 
				{
					printf("This bitmap is not suitable for My Compressed Bitmap format!\n");
					fclose(in);
					return 0;
				}

				if (( bfh.bfType       & 0x00FF) != 'M' || ((bfh.bfType >> 8) & 0x00FF) != 'C') 
				{
					printf("Wrong file type\n");
					fclose(in);
					return 0;
				}

				if(argc==3)
				{	
					n=strlen(argv[2]);

					if(n>=4)
					{	
						if(argv[2][n-4]=='.' && argv[2][n-3]=='m' && argv[2][n-2]=='c' && argv[2][n-1]=='b')
						{
							out = new char[n];
							strncpy(out,argv[2],n-4);
							out[n-4]='.'; out[n-3]='b'; out[n-2]='m'; out[n-1]='p';
						}
						else
						{
							out = new char[n+4];
							strcpy(out,argv[2]);
							out[n]='.'; out[n+1]='b'; out[n+2]='m'; out[n+3]='p';
						}	
					}
					else
					{
						out = new char[n+4];
						strcpy(out,argv[2]);
						out[n]='.'; out[n+1]='b'; out[n+2]='m'; out[n+3]='p';
					}	
				
				}	
				if(argc==4)
				{
					out=argv[3];
				}
			}	
		}	
	}
	else
	{
		printf("Folosire incorecta a programului\n");
	}

	fclose(fout);
	fclose(in);

	return 0;	
}