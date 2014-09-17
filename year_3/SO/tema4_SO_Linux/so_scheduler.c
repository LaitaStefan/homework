/* 
 * Papa Florin, 334CA
 * Tema 4 SO
 */

#include "so_scheduler.h"
#include "utils.h"
#include <string.h>

#define MAX_IO 	256
#define ERROR 	-1
#define SUCCESS 0

int timeQuantum = -1;
int currentTime;
int io_dev;
pthread_cond_t *io_cond;
pthread_mutex_t *io_mutex;
pthread_mutex_t *bl_mutex;
thread *threadList;
int *blocked;
int calledFunction = 0;

/* init function */
int so_init(unsigned int quantum, unsigned int io_devices) {
	int i;

	/* double init */
	if(timeQuantum != -1) {
		return ERROR;
	}

	if(pthread_mutex_init(&(list_mutex), NULL) != SUCCESS) {
		return ERROR;
	}
	if(pthread_mutex_init(&(all_done), NULL) != SUCCESS) {
		return ERROR;
	}	
	pthread_mutex_lock(&all_done);
	currentTime = quantum;
	timeQuantum = quantum;
	if(io_devices > MAX_IO || (int)io_devices < 0 || (int)quantum <= 0) {
		return ERROR;
	}
	io_dev = io_devices;
	io_cond = malloc(io_devices * sizeof(pthread_cond_t));
	io_mutex = malloc(io_devices * sizeof(pthread_mutex_t));
	bl_mutex = malloc(io_devices * sizeof(pthread_mutex_t));
	blocked = malloc(io_devices * sizeof(int));
	memset(blocked, 0, io_devices * sizeof(int));

	for(i = 0; i < io_devices; i++) {
		if(pthread_cond_init(&(io_cond[i]), NULL) != SUCCESS) {
			return ERROR;
		}
		if(pthread_mutex_init(&(io_mutex[i]), NULL) != SUCCESS) {
			return ERROR;
		}
		if(pthread_mutex_init(&(bl_mutex[i]), NULL) != SUCCESS) {
			return ERROR;
		}
	}
	threadList = NULL;

	return SUCCESS;
}

void* start_thread(void *params) {
	tid_t my_id = pthread_self();
	parameters *p = (parameters*)params;
	thread *next_to_run;
	unsigned int prio = p->priority;
	so_handler *h = p->handler;

	/* create thread structure */
	thread *newThread = create_struct_thread(p->priority, my_id);

	pthread_mutex_lock(&list_mutex);
	threadList = add_thread(newThread, threadList);
	pthread_mutex_unlock(&list_mutex);

	/* notify getting ready */
	while(p->blocked != 1) {
		/* wait */
	}

	pthread_mutex_lock(&(p->mutex_cond));
	pthread_cond_signal(&(p->cond));
	pthread_mutex_unlock(&(p->mutex_cond));

	/* wait to be scheduled */
	pthread_mutex_lock(&(newThread->mutex_cond));
	newThread->blocked = 1;
	pthread_cond_wait(&(newThread->cond), &(newThread->mutex_cond));
	pthread_mutex_unlock(&(newThread->mutex_cond));
	
	/* call handler function */
	h(prio);


	/* mark process as terminated */
	pthread_mutex_lock(&list_mutex);
	newThread->state = TERMINATED;
	newThread->next_to_run = 0;
	planner(my_id, &currentTime, &timeQuantum, threadList, 1);

	return NULL;
}

tid_t so_fork(so_handler *handler, unsigned int priority) {
	parameters *params = malloc(sizeof(parameters));
	thread *my_thread;
	tid_t newThread;
	tid_t my_id = pthread_self();
	int prio;

	calledFunction = 1;

	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) prio = my_thread->priority;

	/* init params structure */
	params->priority = priority;
	params->handler = handler;
	params->blocked = 0;

	if(pthread_cond_init(&(params->cond), NULL) != SUCCESS) {
		fprintf(stderr, "Error initializing condition\n");
		exit(1);
	}
	if(pthread_mutex_init(&(params->mutex_cond), NULL) != SUCCESS) {
		fprintf(stderr, "Error initializing mutex\n");
		exit(1);
	}

	/* create new thread */
	if(handler == NULL) {
		newThread = INVALID_TID;
	}
	else {
		//pthread_mutex_lock(&list_mutex);
		if(pthread_create(&newThread, NULL, &start_thread, params) != SUCCESS) {
			fprintf(stderr, "Error starting new thread\n");
			exit(1);
		}

		/* wait to get READY */
		pthread_mutex_lock(&(params->mutex_cond));
		params->blocked = 1;
		pthread_cond_wait(&(params->cond), &(params->mutex_cond));
		pthread_mutex_unlock(&(params->mutex_cond));
	}

	/* destroy cond */
	pthread_cond_destroy(&(params->cond));
	pthread_mutex_destroy(&(params->mutex_cond));
	free(params);

	/* run planner */
	pthread_mutex_lock(&list_mutex);
	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) {
		my_thread->state = READY;
	}
	planner(my_id, &currentTime, &timeQuantum, threadList, 0);
	pthread_mutex_lock(&list_mutex);
	if(my_thread != NULL) {
		my_thread->state = RUNNING;
	}
	pthread_mutex_unlock(&list_mutex);

	return newThread;
}

int so_wait(unsigned int event_no) {
	tid_t my_id = pthread_self();
	thread *my_thread = get_thread(my_id, threadList);
	int prio;
	int chosen;

	if(my_thread != NULL) {
		prio = my_thread->priority;
	}

	calledFunction = 1;
	
	if((int)event_no >= io_dev) {

		pthread_mutex_lock(&list_mutex);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = READY;
		}
		planner(my_id, &currentTime, &timeQuantum, threadList, 0);
		pthread_mutex_lock(&list_mutex);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = RUNNING;
		}
		pthread_mutex_unlock(&list_mutex);

		return ERROR;
	}

	pthread_mutex_lock(&(io_mutex[event_no]));

	pthread_mutex_lock(&bl_mutex[event_no]);
	blocked[event_no]++;
	pthread_mutex_unlock(&bl_mutex[event_no]);

	pthread_mutex_lock(&list_mutex);
	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) {
		my_thread->state = WAITING;
	}
	planner(my_id, &currentTime, &timeQuantum, threadList, 0);
	pthread_cond_wait(&(io_cond[event_no]), &(io_mutex[event_no]));
	pthread_mutex_unlock(&(io_mutex[event_no]));

	my_thread->state = READY;
	pthread_mutex_lock(&bl_mutex[event_no]);
	blocked[event_no]--;
	if(blocked[event_no] == 0) {
		chosen = 1;
	}
	else {
		chosen = 0;
	}
	pthread_mutex_unlock(&bl_mutex[event_no]);

	if(my_thread != NULL) {
		if(chosen) {
			pthread_mutex_lock(&list_mutex);
			my_thread = get_thread(my_id, threadList);
			if(my_thread != NULL) {
				my_thread->state = READY;
			}
			planner(my_id, &currentTime, &timeQuantum, threadList, 0);
			pthread_mutex_lock(&list_mutex);
			if(my_thread != NULL) {
				my_thread->state = RUNNING;
			}
			pthread_mutex_unlock(&list_mutex);
		}
		else {
			pthread_mutex_lock(&(my_thread->mutex_cond));

			my_thread->state = READY;
			my_thread->blocked = 1;
			pthread_cond_wait(&(my_thread->cond), &(my_thread->mutex_cond));
			pthread_mutex_unlock(&(my_thread->mutex_cond));
		}
	}
	else {
		pthread_mutex_lock(&bl_mutex[event_no]);
		blocked[event_no]--;
		pthread_mutex_unlock(&bl_mutex[event_no]);
	}	

	return SUCCESS;
}

int so_signal(unsigned int event_no) {
	tid_t my_id = pthread_self();
	int no_threads;
	thread *my_thread = get_thread(my_id, threadList);
	int prio, retVal;

	if(my_thread != NULL) {
		prio = my_thread->priority;
	}

	calledFunction = 1;

	if((int)event_no < io_dev && blocked[event_no] > 0) {

		retVal = blocked[event_no];

		pthread_mutex_lock(&(io_mutex[event_no]));
		pthread_cond_signal(&(io_cond[event_no]));
		pthread_mutex_unlock(&(io_mutex[event_no]));

		pthread_mutex_lock(&(io_mutex[event_no]));
		pthread_cond_signal(&(io_cond[event_no]));
		pthread_mutex_unlock(&(io_mutex[event_no]));


		if(my_thread != NULL) {
			pthread_mutex_lock(&(my_thread->mutex_cond));
			my_thread->state = READY;
			my_thread->blocked = 1;
			pthread_cond_wait(&(my_thread->cond), &(my_thread->mutex_cond));
			pthread_mutex_unlock(&(my_thread->mutex_cond));
			pthread_mutex_lock(&list_mutex);
			if(my_thread != NULL) {
				my_thread->state = RUNNING;
			}
			pthread_mutex_unlock(&list_mutex);
		}

		return retVal;
	}
	else if((int)event_no >= io_dev) {
		retVal = ERROR;

		pthread_mutex_lock(&list_mutex);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = READY;
		}
		planner(my_id, &currentTime, &timeQuantum, threadList, 0);
		pthread_mutex_lock(&list_mutex);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = RUNNING;
		}
		pthread_mutex_unlock(&list_mutex);

		return retVal;
	}
	else if(blocked[event_no] == 0) {
		
		retVal = 0;

		pthread_mutex_lock(&list_mutex);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = READY;
		}
		planner(my_id, &currentTime, &timeQuantum, threadList, 0);
		pthread_mutex_lock(&list_mutex);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = RUNNING;
		}
		pthread_mutex_unlock(&list_mutex);

		return retVal;
	}

	return SUCCESS;
}

void so_exec() {
	tid_t my_id = pthread_self();
	thread *my_thread;

	calledFunction = 1;

	int prio;
	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) prio = my_thread->priority;

	pthread_mutex_lock(&list_mutex);
	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) {
		my_thread->state = READY;
	}
	planner(my_id, &currentTime, &timeQuantum, threadList, 0);
	pthread_mutex_lock(&list_mutex);
	if(my_thread != NULL) {
		my_thread->state = RUNNING;
	}
	pthread_mutex_unlock(&list_mutex);
}

void so_end() {
	thread *current = threadList, *aux;
	int i;

	if(calledFunction) {
		pthread_mutex_lock(&all_done);
	}

	while(current != NULL) {
		if(pthread_join(current->thread_id, NULL)) {
			fprintf(stderr, "Error joining\n");
			exit(1);
		}
		current = current->next_thread;
	}

	while(current != NULL) {
		aux = current;
		current = current->next_thread;
		free(aux);
	}

	for(i = 0; i < io_dev; i++) {
		pthread_mutex_destroy(&(io_mutex[i]));
		pthread_cond_destroy(&(io_cond[i]));
		pthread_mutex_destroy(&bl_mutex[i]);
	}

	free(io_mutex);
	free(io_cond);
	free(bl_mutex);
	free(blocked);

	pthread_mutex_destroy(&list_mutex);
	pthread_mutex_destroy(&all_done);
	timeQuantum = -1;
}
