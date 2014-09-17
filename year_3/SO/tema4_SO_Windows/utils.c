/* 
 * Papa Florin, 334CA
 * Tema 4 SO
 */

#include "utils.h"

thread* create_struct_thread(int prio, tid_t thread_id) {
	thread *new_thread = malloc(sizeof(thread));

	new_thread->state = READY;
	new_thread->priority = prio;
	new_thread->thread_id = thread_id;
	new_thread->next_thread = NULL;
	new_thread->next_to_run = 0;
	new_thread->blocked = 0;
	
	new_thread->cond = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(new_thread->cond == NULL) {
		fprintf(stderr, "Error initializing condition\n");
		exit(1);
	}
	new_thread->mutex_cond = CreateMutex(NULL, FALSE, NULL);
	if(new_thread->mutex_cond == NULL) {
		fprintf(stderr, "Error initializing condition\n");
		exit(1);
	}

	return new_thread;
}

thread* add_thread(thread *t, thread *list) {
	int prev_threads = 0;
	thread *current = list;

	if(list == NULL) {
		t->next_to_run = 1;
		t->next_thread = NULL;
		return t;
	}

	while(current != NULL) {
		if(current->priority == t->priority && current->next_to_run == 1 && 
			(current->state == READY || current->state == RUNNING)) {
			prev_threads++;
		}
		if(current->next_thread == NULL) {
			current->next_thread = t;
			if(prev_threads == 0) {
				t->next_to_run = 1;
			}
			break;
		}
		current = current->next_thread;
	}

	return list;
}

void plan_Round_Robin(tid_t thread_id, thread *list) {
	thread *current = list, *initial;
	int done = 0;
	int priority;

	/* determine priority */
	while(current != NULL) {
		if(current->thread_id == thread_id) {
			priority = current->priority;
			current->next_to_run = 0;
			current = current->next_thread;
			break;
		}
		current = current->next_thread;
	}

	/* find next thread to plan */
	while(current != NULL) {
		if(current->priority == priority && current->state == READY) {
			current->next_to_run = 1;
			return;
		}
		current = current->next_thread;
	}

	/* if no threads were found, start over with search */
	current = list;
	while(current != NULL) {
		if(current->priority == priority && current->state == READY) {
			current->next_to_run = 1;
			return;
		}
		current = current->next_thread;
	}

	initial = get_thread(thread_id, list);
	if(initial != NULL) {
		initial->next_to_run = 1;
	}
}

thread* get_next_thread_to_run(thread *list) {
	thread *current = list;
	thread *result = NULL;
	int max_prio;

	if(current == NULL) {
		return NULL;
	}	
	max_prio = -80000;

	while(current != NULL) {
		if(current->state == READY && 
			current->next_to_run == 1 && 
			(int)current->priority >= (int)max_prio) {
			result = current;
			max_prio = current->priority;
		}
		current = current->next_thread;
	}

	return result;
}

thread* get_thread(tid_t thread_id, thread *list) {
	thread *current = list;

	while(current != NULL) {
		if(current->thread_id == thread_id) {
			return current;
		}
		current = current->next_thread;
	}

	return NULL;
}

void planner(tid_t thread_id, int *cTime, int *quantum, thread *list, int fin) {
	thread *next, *current;

	current = get_thread(thread_id, list);
	
	*cTime = *cTime - 1;

	if(*cTime == 0 || (current != NULL && current->state != READY)) {
		plan_Round_Robin(thread_id, list);
		*cTime = *quantum;
	}

	next = get_next_thread_to_run(list);
	if(next == NULL) {
		ReleaseMutex(all_done);
		ReleaseMutex(list_mutex);
		return;
	}
	else if(next->thread_id != thread_id) {
		*cTime = *quantum;

		/* notify thread to start */
		while(next->blocked != 1) {
			/* wait */
		}

		PulseEvent(next->cond);
		WaitForSingleObject(next->mutex_cond, INFINITE);
		next->blocked = 0;
		next->state = RUNNING;
		ReleaseMutex(next->mutex_cond);

		/* wait to be scheduled */
		if(current == NULL || fin == 1 || current->state != READY) {
			ReleaseMutex(list_mutex);
			return;
		}
		ReleaseMutex(list_mutex);
		WaitForSingleObject(current->mutex_cond, INFINITE);
		current->state = READY;
		current->blocked = 1;
		ReleaseMutex(current->mutex_cond);
		WaitForSingleObject(current->cond, INFINITE);
		
	}
	else {
		ReleaseMutex(list_mutex);
	}
}