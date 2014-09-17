/* 
 * Papa Florin, 334CA
 * Tema 4 SO
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "so_scheduler.h"

enum STATE {
	NEW,
	READY,
	RUNNING,
	WAITING,
	TERMINATED
};

typedef struct _thread {
	int state;
	unsigned int priority;
	int next_to_run;
	int blocked;
	tid_t thread_id;
	pthread_cond_t cond;
	pthread_mutex_t mutex_cond;
	struct _thread *next_thread;
} thread;

typedef struct parameters {
	unsigned int priority;
	so_handler *handler;
	int blocked;
	pthread_cond_t cond;
	pthread_mutex_t mutex_cond;
} parameters;

pthread_mutex_t list_mutex;
pthread_mutex_t all_done;

thread* create_struct_thread(int prio, tid_t thread_id);
thread* add_thread(thread *t, thread *list);
thread* get_thread(tid_t id, thread *list);
thread* get_next_thread_to_run(thread *list);
void plan_Round_Robin(tid_t thread_id, thread *list);
void planner(tid_t thread_id, int *current_time, int *quantum, thread *list, int fin);
