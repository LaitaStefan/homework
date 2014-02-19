//Papa Florin, 324CA

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"
#include "utils.h"



int main(int argc, char *argv[])
{
	int task_index;
	task_index = atoi(argv[1]);	
	printf("[RECEIVER] Receiver starts.\n");
	printf("[RECEIVER] Task index=%d\n", task_index);
		
	init(HOST, PORT2);

	if(task_index == 0) {

		task_0_recv();
	
	}

	if(task_index == 1) {

		task_1_recv();
		
	}

	if(task_index == 2) {

		task_2_recv();

	}

	if(task_index == 3) {

		task_3_recv();

	}	
	
	
	return 0;
}
