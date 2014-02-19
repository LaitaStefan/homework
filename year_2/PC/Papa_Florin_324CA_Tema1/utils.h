//Papa Florin, 324CA

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

void task_0_recv();
void task_1_recv();
void task_2_recv();
void task_3_recv();

char get_parity(char* load, int size);
void setBit(char *buff, int nr_bit, char val);
char getBit(char nr,int poz);
void sort(pkt* p, int size);
void sort2(pkt2* p, int size);
int max(int a, int b);

void task_0_send(int size, int window_size, int fd, char* filename);
void task_1_send(int size, int window_size, char* filename, int delay, int fd);
void task_2_send(int size, int window_size, char* filename, int delay, int fd);
void task_3_send(int size, int window_size, char* filename, int delay, int fd);
