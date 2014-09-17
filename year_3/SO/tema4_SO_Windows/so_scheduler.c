/* 
 * Papa Florin, 334CA
 * Tema 4 SO
 */

#include "so_scheduler.h"
#include "utils.h"
#include <string.h>

int timeQuantum = -1;
int currentTime;
int io_dev;
HANDLE *io_cond;
HANDLE *io_mutex;
HANDLE *bl_mutex;
thread *threadList;

int *blocked;
int calledFunction = 0;

/* init function */
int so_init(unsigned int quantum, unsigned int io_devices) {
	int i;

	/* double init */
	if(timeQuantum != -1) {
		return ERR;
	}

	list_mutex = CreateMutex(NULL, FALSE, NULL);
	if(list_mutex == NULL) {
		return ERR;
	}
	all_done = CreateMutex(NULL, FALSE, NULL);
	if(all_done == NULL) {
		return ERR;
	}
	
	WaitForSingleObject(all_done, INFINITE);

	currentTime = quantum;
	timeQuantum = quantum;
	if(io_devices > MAX_IO || (int)io_devices < 0 || (int)quantum <= 0) {
		return ERR;
	}
	io_dev = io_devices;
	io_cond = malloc(io_devices * sizeof(HANDLE));
	io_mutex = malloc(io_devices * sizeof(HANDLE));
	bl_mutex = malloc(io_devices * sizeof(HANDLE));
	blocked = malloc(io_devices * sizeof(int));
	memset(blocked, 0, io_devices * sizeof(int));

	for(i = 0; i < io_devices; i++) {
		io_cond[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		if(io_cond[i] == NULL) {
			return ERR;
		}
		
		io_mutex[i] = CreateMutex(NULL, FALSE, NULL);
		if(io_mutex[i] == NULL) {
			return ERR;
		}
		bl_mutex[i] = CreateMutex(NULL, FALSE, NULL);
		if(bl_mutex[i] == NULL) {
			return ERR;
		}
	}
	threadList = (thread*)NULL;

	return SUCCESS;
}

DWORD WINAPI start_thread(LPVOID params) {
	tid_t my_id = GetCurrentThreadId();
	parameters *p = (parameters*)params;
	thread *next_to_run;
	unsigned int prio = p->priority;
	so_handler *h = p->handler;

	/* create thread structure */
	thread *newThread = create_struct_thread(p->priority, my_id);
	
	WaitForSingleObject(list_mutex, INFINITE);
	threadList = add_thread(newThread, threadList);
	p->newThread = newThread;
	ReleaseMutex(list_mutex);

	/* notify getting ready */
	while(p->blocked != 1) {
		/* wait */
	}

	PulseEvent(p->cond);

	/* wait to be scheduled */
	WaitForSingleObject(newThread->mutex_cond, INFINITE);
	newThread->blocked = 1;
	ReleaseMutex(newThread->mutex_cond);
	
	WaitForSingleObject(newThread->cond, INFINITE);
	
	/* call handler function */
	h(prio);


	/* mark process as terminated */
	WaitForSingleObject(list_mutex, INFINITE);
	newThread->state = TERMINATED;
	newThread->next_to_run = 0;
	planner(my_id, &currentTime, &timeQuantum, threadList, 1);

	return SUCCESS;
}

tid_t so_fork(so_handler *handler, unsigned int priority) {
	parameters *params = malloc(sizeof(parameters));
	thread *my_thread;
	tid_t my_id = GetCurrentThreadId();
	tid_t newID;
	int prio;

	calledFunction = 1;

	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) prio = my_thread->priority;

	/* init params structure */
	params->priority = priority;
	params->handler = handler;
	params->blocked = 0;

	params->cond = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(params->cond == NULL) {
		fprintf(stderr, "Error initializing condition\n");
		exit(1);
	}
	params->mutex_cond = CreateMutex(NULL, FALSE, NULL);
	if(params->mutex_cond == NULL) {
		fprintf(stderr, "Error initializing mutex\n");
		exit(1);
	}

	/* create new thread */
	if(handler == NULL) {
		newID = INVALID_TID;
	}
	else {
		HANDLE hThread;
		hThread = CreateThread(NULL,                    /* default security attributes */
			      0,                                   		/* default stack size */
			      start_thread, 							/* routine to execute */
			      params,                              		/* no thread parameter */
			      0,                                   		/* immediately run the thread */
			      &newID);                          		/* thread id */
		if(hThread == NULL) {
			fprintf(stderr, "Error starting new thread\n");
			exit(1);
		}

		/* wait to get READY */
		params->blocked = 1;
		WaitForSingleObject(params->cond, INFINITE);
		params->newThread->h = hThread;
	}

	/* destroy cond */
	CloseHandle(params->cond);
	CloseHandle(params->mutex_cond);
	free(params);
	
	/* run planner */
	WaitForSingleObject(list_mutex, INFINITE);
	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) {
		my_thread->state = READY;
	}
	planner(my_id, &currentTime, &timeQuantum, threadList, 0);
	WaitForSingleObject(list_mutex, INFINITE);
	if(my_thread != NULL) {
		my_thread->state = RUNNING;
	}
	ReleaseMutex(list_mutex);

	return newID;
}

int so_wait(unsigned int event_no) {
	tid_t my_id = GetCurrentThreadId();
	thread *my_thread = get_thread(my_id, threadList);
	int prio;
	int chosen;

	if(my_thread != NULL) {
		prio = my_thread->priority;
	}

	calledFunction = 1;
	
	if((int)event_no >= io_dev) {

		WaitForSingleObject(list_mutex, INFINITE);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = READY;
		}
		planner(my_id, &currentTime, &timeQuantum, threadList, 0);
		WaitForSingleObject(list_mutex, INFINITE);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = RUNNING;
		}
		ReleaseMutex(list_mutex);

		return ERR;
	}

	WaitForSingleObject(bl_mutex[event_no], INFINITE);
	blocked[event_no]++;
	ReleaseMutex(bl_mutex[event_no]);

	WaitForSingleObject(list_mutex, INFINITE);
	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) {
		my_thread->state = WAITING;
	}
	planner(my_id, &currentTime, &timeQuantum, threadList, 0);
	WaitForSingleObject(io_cond[event_no], INFINITE);

	my_thread->state = READY;
	WaitForSingleObject(bl_mutex[event_no], INFINITE);
	blocked[event_no]--;
	if(blocked[event_no] == 0) {
		chosen = 1;
	}
	else {
		chosen = 0;
	}
	ReleaseMutex(bl_mutex[event_no]);

	if(my_thread != NULL) {
		if(chosen) {
			WaitForSingleObject(list_mutex, INFINITE);
			my_thread = get_thread(my_id, threadList);
			if(my_thread != NULL) {
				my_thread->state = READY;
			}
			planner(my_id, &currentTime, &timeQuantum, threadList, 0);
			WaitForSingleObject(list_mutex, INFINITE);
			if(my_thread != NULL) {
				my_thread->state = RUNNING;
			}
			ReleaseMutex(list_mutex);
		}
		else {
			my_thread->state = READY;
			my_thread->blocked = 1;
			WaitForSingleObject(my_thread->cond, INFINITE);
		}
	}
	else {
		WaitForSingleObject(bl_mutex[event_no], INFINITE);
		blocked[event_no]--;
		ReleaseMutex(bl_mutex[event_no]);
	}	

	return SUCCESS;
}

int so_signal(unsigned int event_no) {
	tid_t my_id = GetCurrentThreadId();
	int no_threads;
	thread *my_thread = get_thread(my_id, threadList);
	int prio, retVal;

	if(my_thread != NULL) {
		prio = my_thread->priority;
	}

	calledFunction = 1;

	if((int)event_no < io_dev && blocked[event_no] > 0) {

		retVal = blocked[event_no];

		PulseEvent(io_cond[event_no]);

		if(my_thread != NULL) {
			my_thread->state = READY;
			my_thread->blocked = 1;
			WaitForSingleObject(my_thread->cond, INFINITE);
			WaitForSingleObject(list_mutex, INFINITE);
			if(my_thread != NULL) {
				my_thread->state = RUNNING;
			}
			ReleaseMutex(list_mutex);
		}

		return retVal;
	}
	else if((int)event_no >= io_dev) {
		retVal = ERR;

		WaitForSingleObject(list_mutex, INFINITE);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = READY;
		}
		planner(my_id, &currentTime, &timeQuantum, threadList, 0);
		WaitForSingleObject(list_mutex, INFINITE);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = RUNNING;
		}
		ReleaseMutex(list_mutex);

		return retVal;
	}
	else if(blocked[event_no] == 0) {
		
		retVal = 0;

		WaitForSingleObject(list_mutex, INFINITE);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = READY;
		}
		planner(my_id, &currentTime, &timeQuantum, threadList, 0);
		WaitForSingleObject(list_mutex, INFINITE);
		my_thread = get_thread(my_id, threadList);
		if(my_thread != NULL) {
			my_thread->state = RUNNING;
		}
		ReleaseMutex(list_mutex);

		return retVal;
	}

	return SUCCESS;
}

void so_exec() {
	tid_t my_id = GetCurrentThreadId();
	thread *my_thread;
	int prio;
	
	calledFunction = 1;
	
	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) prio = my_thread->priority;

	WaitForSingleObject(list_mutex, INFINITE);
	my_thread = get_thread(my_id, threadList);
	if(my_thread != NULL) {
		my_thread->state = READY;
	}
	planner(my_id, &currentTime, &timeQuantum, threadList, 0);
	WaitForSingleObject(list_mutex, INFINITE);
	if(my_thread != NULL) {
		my_thread->state = RUNNING;
	}
	ReleaseMutex(list_mutex);
}

void so_end() {
	thread *current = threadList, *aux;
	DWORD dwReturn;
	int i;

	if(calledFunction) {
		WaitForSingleObject(all_done, INFINITE);
	}

	while(current != NULL) {
		dwReturn = WaitForSingleObject(current->h, INFINITE);
		if(dwReturn == WAIT_FAILED) {
			fprintf(stderr, "error WaitForSingleObject\n");
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
		CloseHandle(io_mutex[i]);
		CloseHandle(io_cond[i]);
		CloseHandle(bl_mutex[i]);
	}

	free(io_mutex);
	free(io_cond);
	free(bl_mutex);
	free(blocked);

	CloseHandle(list_mutex);
	CloseHandle(all_done);
	timeQuantum = -1;
}
