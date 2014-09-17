/*
 * Papa Florin, 334CA
 * Tema 3 SO
 */

#include "vmsim.h"
#include "util.h"
#include "common.h"
#include "string.h"
#include "helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NAME_SIZE 20

// list of structures that keep track of all memory allocated using vm_alloc
// initial size is 0 and it will be reallocated on demand
struct mem_alloc *virtual_mem = NULL;


//allocate structure keeping track of allocated memory
void alloc_virtual_mem(w_size_t num_pages,
					w_size_t num_frames,
					vm_map_t *map,
					char *ram_name,
					char *swap_name) {

	int pte_size = sizeof(struct page_table_entry);
	int struct_frame_size = sizeof(struct frame);
	int i;
	struct mem_alloc *current_mem = malloc(sizeof(struct mem_alloc));
	struct mem_alloc *crt_list;

	current_mem->vm_map = map;

	current_mem->ram_name = malloc(strlen(ram_name) + 1);
	memset(current_mem->ram_name, 0, strlen(ram_name) + 1);
	strcpy(current_mem->ram_name, ram_name);

	current_mem->swap_name = malloc(strlen(swap_name) + 1);
	memset(current_mem->swap_name, 0, strlen(swap_name) + 1);
	strcpy(current_mem->swap_name, swap_name);

	current_mem->frames = malloc(num_frames * struct_frame_size);
	DIE(current_mem->frames == NULL, "Error allocating memory\n");

	current_mem->pages = malloc(num_pages * pte_size);
	DIE(current_mem->pages == NULL, "Error allocating memory\n");

	//initialize structures
	for(i = 0; i < num_pages; i++) {
		current_mem->pages[i].index = i;
		current_mem->pages[i].state = STATE_NOT_ALLOC;
		current_mem->pages[i].prev_state = STATE_NOT_ALLOC;
		current_mem->pages[i].dirty = FALSE;
		current_mem->pages[i].protection = PROTECTION_NONE;
		current_mem->pages[i].start = map->start + p_sz * i;
		current_mem->pages[i].frame = NULL;
	}

	for(i = 0; i < num_frames; i++) {
		current_mem->frames[i].index = i;
		current_mem->frames[i].pte = NULL;
	}

	current_mem->next = NULL;

	//add current structure to the end of the list
	if(virtual_mem == NULL) {
		virtual_mem = current_mem;
	}
	else {
		crt_list = virtual_mem;

		while(crt_list->next != NULL) {
			crt_list = crt_list->next;
		}

		crt_list->next = current_mem;
	}
}

//function that returns the structure where the page fault occurred
struct mem_alloc* find_page_fault(char *p) {

	struct mem_alloc *current = virtual_mem;

	while(current != NULL) {
		vm_map_t *map = current->vm_map;
		int size = w_get_file_size_by_handle(map->swap_handle);

		if(p >= (char*)map->start && p < (char*)(map->start + size)) {
			return current;
		}

		current = current->next;
	}

	return NULL;
}

//returns the index of the page where the page fault occurred
int get_virtual_page_index(char *p, struct mem_alloc *mem) {

	return (unsigned long)(p - (unsigned long)mem->vm_map->start) / p_sz;
}

//returns the protection of a page
int get_protection(int p) {
	if(p == PROTECTION_NONE) {
		return PROT_NONE;
	}
	else if(p == PROTECTION_READ) {
		return PROT_READ;
	}
	else {
		return PROT_READ | PROT_WRITE;
	}
}

//get a free RAM page, swap if necessary
int get_free_ram_page(struct mem_alloc *mem) {
	int i, size, rd;
	char *buffer;
	enum page_state prev_state = STATE_NOT_ALLOC;
	void *ret;

	size = w_get_file_size_by_handle(mem->vm_map->ram_handle) / p_sz;

	for(i = 0; i < size; i++) {
		if(mem->frames[i].pte == NULL) {
			return i;
		}
	}

	//no free page was found, swapping necessary
	prev_state = (mem->frames[0].pte)->prev_state;

	if(prev_state == STATE_NOT_ALLOC || (mem->frames[0].pte)->dirty == TRUE) {

		buffer = malloc(p_sz);

		if(! w_set_file_pointer(mem->vm_map->ram_handle, 0)) {
			DIE(TRUE, "Unable to seek in ram\n");
		}
		if(! w_read_file(mem->vm_map->ram_handle, buffer, p_sz)) {
			DIE(TRUE, "Unable to write to swap\n");
		}

		if(! w_set_file_pointer(mem->vm_map->swap_handle, 
								(mem->frames[0].pte)->index * p_sz)) {
			DIE(TRUE, "Unable to seek in swap\n");
		}
		if(! w_write_file(mem->vm_map->swap_handle, buffer, p_sz)) {
			DIE(TRUE, "Unable to write to swap\n");
		}

		free(buffer);
	}

	(mem->frames[0].pte)->prev_state = (mem->frames[0].pte)->state;
	(mem->frames[0].pte)->state = STATE_IN_SWAP;
	(mem->frames[0].pte)->dirty = FALSE;
	(mem->frames[0].pte)->frame = NULL;

	//unmap memory
	rd = munmap((mem->frames[0].pte)->start, p_sz);
	DIE(rd == -1, "Error unmapping\n");

	ret = mmap((mem->frames[0].pte)->start, p_sz, PROT_NONE,
				MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	DIE(ret == MAP_FAILED, "Error mapping\n");

	mem->frames[0].pte = NULL;

	return 0;
}

//SIGSEGV exception handler
void vmsim_exception_handler(int sig, siginfo_t *siginfo, void *aux) {

	if(sig == SIGSEGV) {
		struct mem_alloc *loc;
		int virt_index, physical_index, rd, prot;
		void *ret;	

		loc = find_page_fault(siginfo->si_addr);
		DIE(loc == NULL, "Unable to locate memory structure\n");

		virt_index = get_virtual_page_index(siginfo->si_addr, loc);

		//check mappings

		//case not mapped
		if(loc->pages[virt_index].state == STATE_NOT_ALLOC) {

			physical_index = get_free_ram_page(loc);

			//map physical space
			rd = munmap(loc->pages[virt_index].start, p_sz);
			DIE(rd == -1, "Error unmapping\n");

			ret = mmap(loc->pages[virt_index].start, p_sz, PROT_READ,
					MAP_SHARED, loc->vm_map->ram_handle, physical_index * p_sz);
			DIE(ret == MAP_FAILED, "Error mapping\n");

			//change page table entry
			enum page_state new_prev = loc->pages[virt_index].state;
			loc->pages[virt_index].prev_state = new_prev;
			loc->pages[virt_index].state = STATE_IN_RAM;
			loc->pages[virt_index].protection = PROTECTION_READ;
			loc->pages[virt_index].frame = &(loc->frames[physical_index]);

			//change frame info
			loc->frames[physical_index].pte = &(loc->pages[virt_index]);
		}

		//case in swap
		else if(loc->pages[virt_index].state == STATE_IN_SWAP) {

			physical_index = get_free_ram_page(loc);

			//swap in
			char *buffer = malloc(p_sz);

			if(! w_set_file_pointer(loc->vm_map->swap_handle, virt_index)) {
				DIE(TRUE, "Unable to seek in swap\n");
			}
			if(! w_read_file(loc->vm_map->swap_handle, buffer, p_sz)) {
				DIE(TRUE, "Unable to write to swap\n");
			}

			if(! w_set_file_pointer(loc->vm_map->ram_handle, 
									physical_index * p_sz)) {
				DIE(TRUE, "Unable to seek in ram\n");
			}
			if(! w_write_file(loc->vm_map->ram_handle, buffer, p_sz)) {
				DIE(TRUE, "Unable to write to ram\n");
			}

			free(buffer);

			//map physical space
			rd = munmap(loc->pages[virt_index].start, p_sz);
			DIE(rd == -1, "Error unmapping\n");

			prot = get_protection(loc->pages[virt_index].protection);

			ret = mmap(loc->pages[virt_index].start, p_sz, prot, MAP_SHARED,
					loc->vm_map->ram_handle, physical_index * p_sz);
			DIE(ret == MAP_FAILED, "Error mapping\n");

			//change page table entry
			enum page_state new_prev = loc->pages[virt_index].state;

			loc->pages[virt_index].prev_state = new_prev;
			loc->pages[virt_index].state = STATE_IN_RAM;
			loc->pages[virt_index].frame = &(loc->frames[physical_index]);

			//change frame info
			loc->frames[physical_index].pte = &(loc->pages[virt_index]);
		}

		//case in ram
		else if(loc->pages[virt_index].state == STATE_IN_RAM) {

			physical_index = (loc->pages[virt_index].frame)->index;

			//map physical space
			rd = munmap(loc->pages[virt_index].start, p_sz);
			DIE(rd == -1, "Error unmapping\n");

			ret = mmap(loc->pages[virt_index].start, p_sz, 
				PROT_READ | PROT_WRITE, MAP_SHARED,
				loc->vm_map->ram_handle, physical_index * p_sz);
			DIE(ret == MAP_FAILED, "Error mapping\n");

			//change page table entry info
			enum page_state new_prev = loc->pages[virt_index].state;
			loc->pages[virt_index].prev_state = new_prev;
			loc->pages[virt_index].protection = PROTECTION_WRITE;
			loc->pages[virt_index].dirty = TRUE;
		}
	}
}

w_boolean_t vmsim_init(void) {

	return w_set_exception_handler(&vmsim_exception_handler);

}

w_boolean_t vmsim_cleanup(void) {

	w_exception_handler_t phandler;

	w_get_previous_exception_handler(&phandler);

	return w_set_exception_handler(phandler);
}

w_boolean_t vm_alloc(w_size_t num_pages, w_size_t num_frames, vm_map_t *map) {

	char file_ram[] = "RAMXXXXXX";
	char file_swap[] = "SWAPXXXXXX";
	int rc;

	if(num_frames > num_pages) {
		return FALSE;
	}
	
	//create new file for RAM and SWAP
	map->ram_handle = mkstemp(file_ram);
	DIE(map->ram_handle < 0, "Error opening RAM file\n");

	map->swap_handle = mkstemp(file_swap);
	DIE(map->swap_handle < 0, "Error opening SWAP file\n");

	//truncate files to given size
	rc = ftruncate(map->ram_handle, p_sz * num_frames);
	DIE(rc != 0, "Error truncating ram file\n");

	rc = ftruncate(map->swap_handle, p_sz * num_pages);
	DIE(rc != 0, "Error truncating swap file\n");

	//allocate memory
	map->start = mmap(NULL, num_pages * p_sz, PROT_NONE,
					MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(map->start == MAP_FAILED) {
		return FALSE;
	}

	//allocate internal tracking mechanism
	alloc_virtual_mem(num_pages, num_frames, map, file_ram, file_swap);

	return TRUE;
}

//free all memory, close and delete files
w_boolean_t vm_free(w_ptr_t start) {

	if(start == NULL) {
		return FALSE;
	}

	struct mem_alloc *current_mem = virtual_mem, *aux;
	int mem_size;

	while(current_mem != NULL) {

		if(current_mem->vm_map->start == start) {
			vm_map_t *map = current_mem->vm_map;
			int rc;

			//free memory and delete files
			mem_size = w_get_file_size_by_handle(map->swap_handle);

			if(w_close_file(map->swap_handle) == FALSE) {
				return FALSE;
			}
			if(w_close_file(map->ram_handle) == FALSE) {
				return FALSE;
			}
			
			if(w_delete_file(current_mem->ram_name) == FALSE) {
				return FALSE;
			}
			if(w_delete_file(current_mem->swap_name) == FALSE) {
				return FALSE;
			}

			rc = munmap(map->start, mem_size);
			if(rc == -1) {
				return FALSE;
			}

			free(current_mem->ram_name);
			free(current_mem->swap_name);
			free(current_mem->frames);
			free(current_mem->pages);
			free(current_mem);

			break;
		}

		current_mem = current_mem->next;
	}

	return TRUE;
}
