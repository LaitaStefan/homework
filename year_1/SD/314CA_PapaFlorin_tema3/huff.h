#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include "heap.h"

typedef struct tagSerialHuffmanNode {
  uint8_t isTerminal;
  union {
    uint8_t colorIndex;
    struct {
      uint16_t leftChld;
      uint16_t rightChld;
    } __attribute__((__packed__)) chldData;
  } __attribute__((__packed__));
} __attribute__((__packed__)) SerialHuffmanNode; 

typedef struct 
{
	int code;
	int length;
} codes;

template <typename T>
class Huffman
{
	public:
		SerialHuffmanNode huff[511];
		codes array[256];
		Heap<T> h;
		int dim;

		Huffman();
		~Huffman();
		void CreateHuffArray(T vect[]);
		void codificareHuff(T code,T length,T ind);
		uint8_t* CreateBitString(uint8_t *v,int n);
};