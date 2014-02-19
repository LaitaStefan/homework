#include "huff.h"

template<typename T>
Huffman<T>::Huffman()
{
}

template<typename T>
Huffman<T>::~Huffman()
{
}

template<typename T>
void Huffman<T>::CreateHuffArray(T vect[])
{
	int count;
	Node<T> r_son,l_son;
	int r_index,l_index,i;

	for(i=0; i<256; i++)
	{
		h.insertElement(i,vect[i],-1);		//introd in heap
	}	

	count=1;
	while(h.getDim() > 1)
	{
		l_son=h.extractMin();
		r_son=h.extractMin();
		r_index=r_son.address; 
		l_index=l_son.address;

		if(l_son.symbol!=-1)
		{
			huff[count].isTerminal = 1;
			huff[count].colorIndex = l_son.symbol;
			l_index=count;
			count++;
		}
		if(r_son.symbol!=-1)
		{
			huff[count].isTerminal = 1;
			huff[count].colorIndex = r_son.symbol;
			r_index=count;
			count++;
		}	

		if(h.getDim()==0)
		{
			huff[0].isTerminal = 0;
			huff[0].chldData.leftChld = l_index;
			huff[0].chldData.rightChld = r_index;
			break;
		}
		
		huff[count].isTerminal = 0;
		huff[count].chldData.leftChld = l_index;
		huff[count].chldData.rightChld = r_index;	
		count++;

		h.insertElement(-1,r_son.frecv + l_son.frecv,count-1);
	}
}

template <typename T>
void Huffman<T>::codificareHuff(T code,T length,T ind)
{
	if(huff[ind].isTerminal==1)
	{
		array[huff[ind].colorIndex].code=code;
		array[huff[ind].colorIndex].length=length;	
		if(array[huff[ind].colorIndex].length==0)
		{
			array[huff[ind].colorIndex].length=1;
		}
	}
	else
	{
		codificareHuff( code<<1 | 1, length+1, huff[ind].chldData.leftChld);
		codificareHuff( code<<1, length+1, huff[ind].chldData.rightChld);
	}		

}

template <typename T>
uint8_t* Huffman<T>::CreateBitString(uint8_t *pixels,int n)
{
	int reallocate,i,j,k,count;
	uint8_t *v,aux=0;
	v=(uint8_t*)malloc(20*sizeof(uint8_t));
	dim=0;
	count=0;
	reallocate=1;
	for(i=0;i<n;i++)
	{
		for(j=0;j<array[pixels[i]].length;j++)
		{
			k=(array[pixels[i]].code >> j )&1;
			aux=(aux<<1) + k;
			count++;
			if(count==7)
			{
				v[dim]=aux;
				dim++;
				if((dim+1)%20==0)
				{
					reallocate++;
					v=(uint8_t*)realloc(v,reallocate*sizeof(uint8_t));
				}	
			}	
		}	
	}	
	return v;
}

template class Huffman<uint8_t>;	
template class Huffman<int>;