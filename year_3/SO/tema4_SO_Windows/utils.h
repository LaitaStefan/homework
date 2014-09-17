/* 
 * Papa Florin, 334CA
 * Tema 4 SO
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "so_scheduler.h"

#define MAX_IO 	256
#define ERR 	-1
#define SUCCESS 0

enum STATE {
	NEW,
	READY,
	RUNNING,
	WAITING,
	TERMINATED
};

typedef struct _thread {
	HANDLE h;
	int state;
	unsigned int priority;
	int next_to_run;
	int blocked;
	tid_t thread_id;
	HANDLE cond;
	HANDLE mutex_cond;
	struct _thread *next_thread;
} thread;

typedef struct parameters {
	unsigned int priority;
	so_handler *handler;
	thread *newThread;
	int blocked;
	HANDLE cond;
	HANDLE mutex_cond;
} parameters;

HANDLE list_mutex;
HANDLE all_done;

thread* create_struct_thread(int prio, tid_t thread_id);
thread* add_thread(thread *t, thread *list);
thread* get_thread(tid_t id, thread *list);
thread* get_next_thread_to_run(thread *list);
void plan_Round_Robin(tid_t thread_id, thread *list);
void planner(tid_t thread_id, int *current_time, int *quantum, thread *list, int fin);
