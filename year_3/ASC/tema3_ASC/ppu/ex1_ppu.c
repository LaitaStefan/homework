/*
 * Papa Florin, 334CA
 * Tema 3 ASC 2014
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libspe2.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <libmisc.h>
#include <sys/time.h>

#include "btc.h"

extern spe_program_handle_t ex1_spu;


void initialize_arg(struct my_img *thread_arg,
                    struct block *blk,
                    short int *pixels,
                    int width,
                    int height,
                    int spu_no,
                    int mod) {

    /* blocks represents the number of groups of BLOCK_SIZE lines */ 
    int i, blocks = height / BLOCK_SIZE; 
    int blocks_per_thread = blocks / spu_no;
    int blocks_per_width = width / BLOCK_SIZE;
    int current_line;

    /* 
     * divide in blocks of height 8
     * and
     * Initialize my_img structure
     */

    for(i = 0; i < spu_no; i++) {

        thread_arg[i].width = width;
        thread_arg[i].height = blocks_per_thread * BLOCK_SIZE;

        /* last spu may process more than others */
        if(i == spu_no - 1) {
            int remaining_blocks = blocks - (spu_no - 1) * blocks_per_thread;
            thread_arg[i].height = remaining_blocks * BLOCK_SIZE;
        }

        current_line = i * blocks_per_thread * BLOCK_SIZE;
        thread_arg[i].pixels = pixels + current_line * width;
        thread_arg[i].blocks = blk + i * blocks_per_thread * blocks_per_width;
        thread_arg[i].mode = mod;
    }
}

void *ppu_pthread_function(void *thread_arg) {

	spe_context_ptr_t ctx;
	struct my_img *arg = (struct my_img *) thread_arg;

	/* Create SPE context */
	if ((ctx = spe_context_create (0, NULL)) == NULL) {
            perror ("Failed creating context");
            exit (1);
    }

	/* Load SPE program into context */
	if (spe_program_load (ctx, &ex1_spu)) {
        perror ("Failed loading program");
        exit (1);
    }

	/* Run SPE context */
	unsigned int entry = SPE_DEFAULT_ENTRY;
	if (spe_context_run(ctx,
                        &entry,
                        0,
                        arg,
                        (void*)sizeof(struct my_img),
                        NULL) < 0) {  
		perror ("Failed running context");
		exit (1);
	}

	/* Destroy context */
	if (spe_context_destroy (ctx) != 0) {
        perror("Failed destroying context");
        exit (1);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    int i, spu_threads;
    struct img big_picture;
    int width, height, size, num_blocks;
    struct c_img compressed __attribute__ ((aligned(16)));
    short int *pixels;
    struct my_img *thread_arg;
    int mod = atoi(argv[1]);
    struct timeval t1, t2, t3, t4;
    float process_time, total_time;

    /* safety first */
    if(argc != 6) {
        fprintf(stderr, "Usage: ./tema3 mod num_spus in.pgm out.btc out.pgm\n");
        exit(1);
    }

    gettimeofday(&t1, NULL);

    /*
     * Initialization
     */
    read_pgm(argv[3], &big_picture);
    pixels = big_picture.pixels;
    width = big_picture.width;
    height = big_picture.height;
    size= width * height;

    compressed.width = width;
    compressed.height = height;
    num_blocks = (width / BLOCK_SIZE) * (height / BLOCK_SIZE);

    compressed.blocks = malloc_align(num_blocks * sizeof(struct block), 16);
    if(compressed.blocks == NULL) {
        fprintf(stderr, "Error allocating pixels PPU\n");
        exit(1);
    }

    /* 
     * Determine the number of SPE threads to create.
     */

    spu_threads = atoi(argv[2]);
    pthread_t threads[spu_threads];
    thread_arg = malloc_align(spu_threads * sizeof(struct my_img),16);
    initialize_arg(thread_arg,
                    compressed.blocks,
                    pixels, width,
                    height,
                    spu_threads,
                    mod);

    /* 
     * Create several SPE-threads to execute algorithm
     */
    gettimeofday(&t2, NULL);

    for(i = 0; i < spu_threads; i++) {

        /* Create thread for each SPE context */
        if (pthread_create (&threads[i],
                            NULL,
                            &ppu_pthread_function,
                            &thread_arg[i]))  {
            perror ("Failed creating thread");
            exit (1);
        }
    }

    /* Wait for SPU-thread to complete execution.  */
    for (i = 0; i < spu_threads; i++) {
        if (pthread_join (threads[i], NULL)) {
            perror("Failed pthread_join");
            exit (1);
        }
    }

    gettimeofday(&t3, NULL);

    /* write images to file */

    write_pgm(argv[5], &big_picture);
    write_btc(argv[4], &compressed);

    gettimeofday(&t4, NULL);

    process_time = GET_TIME_DELTA(t2, t3);
    total_time = GET_TIME_DELTA(t1, t4);

    printf("Results for mode %d, num_spus %d, input file %s\n", mod,
                                                            spu_threads,
                                                            argv[3]);
    printf("Compression and decompression time: %lf\n", process_time);
    printf("Total run time: %lf\n\n", total_time);

    return 0;
}
