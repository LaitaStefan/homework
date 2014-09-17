/*
 * Papa Florin, 334CA
 * Tema 3 ASC 2014
 */

#include <stdio.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include <math.h>
#include <libmisc.h>
#include <stdlib.h>

#include "../ppu/btc.h"

#define MAX_TRANS 16384

#define waitag(t) mfc_write_tag_mask(1<<t); mfc_read_tag_status_all();

void btc_decompress_vectorial(struct img* image, struct c_img* c_image){
	int block_row_start, block_col_start, i, j, nr_blocks, k;
	unsigned char a, b;

	nr_blocks = c_image->width * c_image->height / (BLOCK_SIZE * BLOCK_SIZE);

	image->width = c_image->width;
	image->height = c_image->height;
	block_row_start = block_col_start = 0;
	for (i=0; i<nr_blocks; i++){
		k = block_row_start * image->width + block_col_start;
		a = c_image->blocks[i].a;
		b = c_image->blocks[i].b;
		for (j=0; j<BLOCK_SIZE * BLOCK_SIZE; j++){
			image->pixels[k++] = (c_image->blocks[i].bitplane[j] ? b : a);
			if ((j + 1) % BLOCK_SIZE == 0){
				k -= BLOCK_SIZE; //back to the first column of the block
				k += image->width ; //go to the next line
			}
		}
		block_col_start += BLOCK_SIZE;
		if (block_col_start >= image->width){
			block_col_start = 0;
			block_row_start += BLOCK_SIZE;
		}
	}
}

void btc_compress_vectorial(short int *pixels,
							int width,
							int height,
							struct c_img* c_image) {

	int row, col, bl_row, bl_col, bl_index, bt_index, nr_blocks;
	float f1, f2, m, q, stdev, a, b, mean;
	unsigned char tmp;
	int i, j;

	c_image->width = width;
	c_image->height = height;
	nr_blocks = width * height / (BLOCK_SIZE * BLOCK_SIZE);
	m = BLOCK_SIZE * BLOCK_SIZE;

	vector unsigned short int *v[BLOCK_SIZE];
	vector unsigned short int v_mean;

	bl_index = 0;
	for (bl_row = 0; bl_row < height; bl_row += BLOCK_SIZE){

		for (bl_col = 0; bl_col < width; bl_col += BLOCK_SIZE){

			//compute mean
			mean = 0;
			v_mean = (vector unsigned short int){0, 0, 0, 0, 0, 0, 0, 0};

			for(i = 0; i < BLOCK_SIZE; i++) {
				v[i] = (vector unsigned short int*)(pixels +(i + bl_row)*width);
				v_mean += v[i][0];
			}

			for(i = 0; i < BLOCK_SIZE; i++) {
				mean += v_mean[i];
			}
			mean /= (BLOCK_SIZE * BLOCK_SIZE);

			//compute standard deviation
			stdev = 0;

			for(i = 0; i < BLOCK_SIZE; i++) {
				for(j = 0; j < BLOCK_SIZE; j++) {
					float diff = pixels[i * BLOCK_SIZE + j] - mean;
					stdev += diff * diff;
				}
			}
			stdev /= (BLOCK_SIZE * BLOCK_SIZE);
			stdev = sqrt (stdev);

			//compute bitplane
			bt_index = 0;
			q = 0;
			for (row = bl_row; row < bl_row + BLOCK_SIZE; row++){
				for (col = bl_col; col < bl_col + BLOCK_SIZE; col++){
					tmp = pixels[row * width + col] > mean ? 1 : 0;
					c_image->blocks[bl_index].bitplane[bt_index++] = tmp;
					q += tmp;
				}
			}

			//compute a and b
            if (q == 0){
                a = b = mean;
            }
            else {
	    		f1 = sqrt(q / (m - q));
                f2 = sqrt((m - q) / q);
        		a = (mean - stdev * f1);
    			b = (mean + stdev * f2);
            }
	        
            //avoid conversion issues due to precision errors
            if (a < 0)
                a = 0;
            if (b > 255)
                b = 255;

			c_image->blocks[bl_index].a = (unsigned char)a;
			c_image->blocks[bl_index].b = (unsigned char)b;
			bl_index++;			
		}
	}
}


/* gets a block sized BLOCK_SIZE * BLOCK_SIZE from the specified address */

void get_block(short int *local,
				short int *remote,
				int width,
				uint32_t *tag) {

	int i;

	for(i = 0; i < BLOCK_SIZE; i++) {
		mfc_get((void*)(local + i * BLOCK_SIZE),
				(uint32_t)(remote + i * width),
				(uint32_t)BLOCK_SIZE * sizeof(short int), 
				tag[0], 
				0, 
				0);
		waitag(tag[0]);
	}
}

/* gets a block for double buffering mechanism */

void get_block_double(short int *local,
						short int *remote,
						int width,
						uint32_t *tag,
						int buf) {
	int i;

	for(i = 0; i < BLOCK_SIZE; i++) {
		mfc_getb((void*)(local + i * BLOCK_SIZE),
					(uint32_t)(remote + i * width),
					(uint32_t)BLOCK_SIZE * sizeof(short int), 
					tag[buf], 
					0, 
					0);
	}
}


/* puts a block sized BLOCK_SIZE * BLOCK_SIZE at the specified address */

void put_decomp_block(short int *local,
						short int *remote,
						int width,
						uint32_t tag) {

	int i;

	for(i = 0; i < BLOCK_SIZE; i++) {
		mfc_put((void*)(local + i * BLOCK_SIZE),
				(uint32_t)(remote + i * width),
				(uint32_t)BLOCK_SIZE * sizeof(short int),
				tag,
				0,
				0);
		waitag(tag);
	}
}


/* puts a compressed block at the specified address */

void put_comp_block(struct block *local,
					struct block *remote,
					uint32_t tag) {

	int size = sizeof(struct block);

	mfc_put((void*)local, (uint32_t)remote, (uint32_t)size, tag, 0, 0);
	waitag(tag);
}

int main(unsigned long long speid,
		unsigned long long argp,
		unsigned long long envp)
{

	uint32_t tag[2];
	struct my_img img __attribute__ ((aligned(16)));
	short int *pixels[2];
	struct c_img compressed;
	struct img decomp;
	int i, buf = 0, nxt_buf;
	int nr_blocks, blocks_per_width, pixel_offset;
	int mod;

	/* reserve tags for DMA transfer */
	tag[0] = mfc_tag_reserve();
	if(tag[0] == MFC_TAG_INVALID) {
		fprintf(stderr, "Error reserving tag 1\n");
		exit(1);
	}

	tag[1] = mfc_tag_reserve();
	if(tag[1] == MFC_TAG_INVALID) {
		fprintf(stderr, "Error reserving tag 2\n");
		exit(1);
	}

	/* allocate internal buffers */

	for(i = 0; i < 2; i++) {
		pixels[i] = malloc_align(BLOCK_SIZE * BLOCK_SIZE * sizeof(short int), 16);
		if(pixels[i] == NULL) {
			fprintf(stderr, "Error allocating pixels in SPU\n");
			exit(1);
		}
	}		

	compressed.blocks = malloc_align(sizeof(struct block), 16);
	if(compressed.blocks == NULL) {
		fprintf(stderr, "Error allocating blocks in SPU\n");
		exit(1);
	}


	/* inital transfer of control structure */
	mfc_get((void*)&img, argp, (int) envp, tag[0], 0, 0);
	waitag(tag[0]);


	/* process each block */
	mod = img.mode;
	nr_blocks = (img.width * img.height) / (BLOCK_SIZE * BLOCK_SIZE);
	blocks_per_width = img.width / BLOCK_SIZE;


	/* normal mode */

	if(mod == 0) {
		decomp.pixels = pixels[0];

		for(i = 0; i < nr_blocks; i++) {
			short int *current;
			struct block *blk_addr = compressed.blocks;

			/* calculate current offset */
			pixel_offset = BLOCK_SIZE * (i / blocks_per_width) * img.width;
			pixel_offset += BLOCK_SIZE * (i % blocks_per_width);
			current = img.pixels + pixel_offset;

			get_block(pixels[0], current, img.width, tag);

			btc_compress_vectorial(pixels[0], BLOCK_SIZE, BLOCK_SIZE, &compressed);
			btc_decompress_vectorial(&decomp, &compressed);

			put_decomp_block(pixels[0], current, img.width, tag[0]);
			put_comp_block(blk_addr, img.blocks + i, tag[0]);
		}
	}


	/* double buffering */

	else {
		get_block_double(pixels[buf], img.pixels, img.width, tag, buf);

		for(i = 0; i < nr_blocks - 1; i++) {
			short int *current, *next;
			struct block *blk_addr = compressed.blocks;
			int next_offset;

			nxt_buf = buf ^ 1;
			next_offset = BLOCK_SIZE * ((i + 1) / blocks_per_width) * img.width;
			next_offset += BLOCK_SIZE * ((i + 1) % blocks_per_width);
			next = img.pixels + next_offset;
			get_block_double(pixels[nxt_buf], next, img.width, tag, nxt_buf);

			/* calculate current offset */
			pixel_offset = BLOCK_SIZE * (i / blocks_per_width) * img.width;
			pixel_offset += BLOCK_SIZE * (i % blocks_per_width);
			current = img.pixels + pixel_offset;

			decomp.pixels = pixels[buf];
			waitag(tag[buf]);
			btc_compress_vectorial(pixels[buf], BLOCK_SIZE, BLOCK_SIZE, &compressed);
			btc_decompress_vectorial(&decomp, &compressed);

			put_decomp_block(pixels[buf], current, img.width, tag[buf]);
			put_comp_block(blk_addr, img.blocks + i, tag[buf]);

			buf = nxt_buf;
		}

		/* process last block */
		pixel_offset = BLOCK_SIZE * (i / blocks_per_width) * img.width;
		pixel_offset += BLOCK_SIZE * (i % blocks_per_width);
		short int *current = img.pixels + pixel_offset;
		struct block *blk_addr = compressed.blocks;

		decomp.pixels = pixels[buf];
		waitag(tag[buf]);
		btc_compress_vectorial(pixels[buf], BLOCK_SIZE, BLOCK_SIZE, &compressed);
		btc_decompress_vectorial(&decomp, &compressed);

		put_decomp_block(pixels[buf], current, img.width, tag[0]);
		put_comp_block(blk_addr, img.blocks + i, tag[0]);
	}


	/* free memory */

	free_align((void*)pixels[0]);
	free_align((void*)pixels[1]);
	free_align((void*)compressed.blocks);
	mfc_tag_release(tag[0]);
	mfc_tag_release(tag[1]);
	
	return 0;
}
