//Papa Florin, 324CA

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"
#include "utils.h"


int main(int argc, char *argv[])
{
	char *filename;
	int task_index, speed, delay, window_size;

	task_index = atoi(argv[1]);
	filename = argv[2];
	speed = atoi(argv[3]);
	delay = atoi(argv[4]);
	window_size = ((speed*delay*1000)/8)/1404;

	init(HOST, PORT1);
	printf("[SENDER] Sender starts.\n");
	printf("[SENDER] Filename=%s, task_index=%d, speed=%d, delay=%d\n", filename, task_index, speed, delay);

	int fd = open(filename, O_RDONLY);
	struct stat s;
	stat(filename,&s);
	int size = s.st_size;

	if(task_index == 0) {

		task_0_send(size, window_size, fd, filename);
		
	}
	
	if(task_index ==1) {
		
		task_1_send(size, window_size, filename, delay, fd);
	}
	
	if(task_index == 2) {

		task_2_send(size,window_size,filename,delay,fd);

	}

	if(task_index == 3) {
		
		task_3_send(size,window_size,filename,delay,fd);
		
	}
	
	return 0;
}
