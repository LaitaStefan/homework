/*
 * Papa Florin, 334CA
 * Tema 3 SO
 */

#include "vmsim.h"
#include "util.h"
#include "helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NAME_SIZE 20

// list of structures that keep track of all memory allocated using vm_alloc
// initial size is 0 and it will be reallocated on demand
struct mem_alloc *virtual_mem = NULL;
w_ptr_t exception_handler;


//allocate structure keeping track of allocated memory
void alloc_virt_mem(w_size_t num_pages,
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
		current_mem->pages[i].start = (char*)map->start + p_sz * i;
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

		if(p >= (char*)map->start && p < (char*)((char*)map->start + size)) {
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
		return PAGE_NOACCESS;
	}
	else if(p == PROTECTION_READ) {
		return PAGE_READONLY;
	}
	else {
		return PAGE_READWRITE;
	}
}

//returns the access rights
int get_rights(int prot) {

	if(prot == PAGE_READONLY) {
		return FILE_MAP_READ;
	}
	else {
		return FILE_MAP_ALL_ACCESS;
	}
}

//get a free RAM page, swap if necessary
int get_free_ram_page(struct mem_alloc *mem) {
	int i, size, rd;
	char *buffer;
	enum page_state prev_state = STATE_NOT_ALLOC;
	void *ret;
	struct page_table_entry *pte;

	size = w_get_file_size_by_handle(mem->vm_map->ram_handle) / p_sz;

	for(i = 0; i < size; i++) {
		if(mem->frames[i].pte == NULL) {
			return i;
		}
	}

	//no free page was found, swapping necessary
	pte = mem->frames[0].pte;
	prev_state = pte->prev_state;

	if(prev_state == STATE_NOT_ALLOC || pte->dirty == TRUE) {

		buffer = malloc(p_sz);

		if(! w_set_file_pointer(mem->vm_map->ram_handle, 0)) {
			DIE(TRUE, "Unable to seek in ram\n");
		}
		if(! w_read_file(mem->vm_map->ram_handle, buffer, p_sz)) {
			DIE(TRUE, "Unable to write to swap\n");
		}

		if(! w_set_file_pointer(mem->vm_map->swap_handle, pte->index * p_sz)) {
			DIE(TRUE, "Unable to seek in swap\n");
		}
		if(! w_write_file(mem->vm_map->swap_handle, buffer, p_sz)) {
			DIE(TRUE, "Unable to write to swap\n");
		}

		free(buffer);
	}

	pte->prev_state = pte->state;
	pte->state = STATE_IN_SWAP;
	pte->dirty = FALSE;
	pte->frame = NULL;

	//unmap memory
	rd = UnmapViewOfFile(pte->start);
	DIE(rd == 0, "Error unmapping\n");

	ret = VirtualAlloc(pte->start, p_sz, MEM_RESERVE, PAGE_NOACCESS);
	DIE(ret == NULL, "Error mapping\n");

	pte = NULL;

	return 0;
}

//SIGSEGV exception handler
LONG vmsim_exc_handler(PEXCEPTION_POINTERS ExceptionInfo) {

	struct mem_alloc *loc;
	EXCEPTION_RECORD *er = ExceptionInfo->ExceptionRecord;
	void *addr = (void*)er->ExceptionInformation[1];
	int virt_index, physical_index, rd, prot, access;
	void *ret;	

	if (er->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		return EXCEPTION_CONTINUE_SEARCH;

	loc = find_page_fault((char*)addr);
	DIE(loc == NULL, "Error page not found\n");

	virt_index = get_virtual_page_index(addr, loc);

	//check mappings

	//case not mapped
	if(loc->pages[virt_index].state == STATE_NOT_ALLOC) {
		HANDLE fmapping;
		enum page_state new_prev;
		DWORD file_offset, page_size;
		
		page_size = p_sz;
		physical_index = get_free_ram_page(loc);
		file_offset = physical_index * p_sz;
		
		//map physical space
		fmapping = CreateFileMapping(loc->vm_map->ram_handle,
									NULL,
									(DWORD) PAGE_READONLY,
									0, 
									0, 
									NULL);
		DIE(fmapping == NULL, "Error creating file mapping\n");
		
		rd = VirtualFree(loc->pages[virt_index].start, 0, MEM_RELEASE);
		DIE(rd == 0, "Error unmapping\n");
		
		ret = MapViewOfFileEx(fmapping,
							FILE_MAP_READ,
							0,
							file_offset,
							page_size,
							loc->pages[virt_index].start);
		DIE(ret == NULL, "Error mapping\n");

		//change page table entry
		new_prev = loc->pages[virt_index].state;
		loc->pages[virt_index].prev_state = new_prev;
		loc->pages[virt_index].state = STATE_IN_RAM;
		loc->pages[virt_index].protection = PROTECTION_READ;
		loc->pages[virt_index].frame = &(loc->frames[physical_index]);

		//change frame info
		loc->frames[physical_index].pte = &(loc->pages[virt_index]);
	}

	//case in swap
	else if(loc->pages[virt_index].state == STATE_IN_SWAP) {
		HANDLE fmapping;
		enum page_state new_prev;
		char *buffer = malloc(p_sz);

		physical_index = get_free_ram_page(loc);

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
		rd = VirtualFree(loc->pages[virt_index].start, 0, MEM_RELEASE);
		DIE(rd == 0, "Error unmapping\n");

		prot = get_protection(loc->pages[virt_index].protection);
		access = get_rights(prot);

		fmapping = CreateFileMapping(loc->vm_map->ram_handle,
									NULL,
									(DWORD)prot,
									0,
									p_sz,
									NULL);
		ret = MapViewOfFileEx(fmapping,
							access,
							0,
							physical_index * p_sz,
							p_sz,
							loc->pages[virt_index].start);
		DIE(ret == NULL, "Error mapping\n");

		//change page table entry
		new_prev = loc->pages[virt_index].state;

		loc->pages[virt_index].prev_state = new_prev;
		loc->pages[virt_index].state = STATE_IN_RAM;
		loc->pages[virt_index].frame = &(loc->frames[physical_index]);

		//change frame info
		loc->frames[physical_index].pte = &(loc->pages[virt_index]);
	}

	//case in ram
	else if(loc->pages[virt_index].state == STATE_IN_RAM) {
		HANDLE fmapping;
		enum page_state new_prev;

		physical_index = (loc->pages[virt_index].frame)->index;

		//map physical space
		rd = UnmapViewOfFile(loc->pages[virt_index].start);
		DIE(rd == 0, "Error unmapping\n");
		
		fmapping = CreateFileMapping(loc->vm_map->ram_handle,
									NULL,
									(DWORD)PAGE_READWRITE,
									0,
									0,
									NULL);
		ret = MapViewOfFileEx(fmapping,
							FILE_MAP_ALL_ACCESS,
							0,
							physical_index * p_sz,
							p_sz,
							loc->pages[virt_index].start);
		DIE(ret == NULL, "Error mapping\n");

		//change page table entry info
		new_prev = loc->pages[virt_index].state;
		loc->pages[virt_index].prev_state = new_prev;
		loc->pages[virt_index].protection = PROTECTION_WRITE;
		loc->pages[virt_index].dirty = TRUE;
	}

	return EXCEPTION_CONTINUE_EXECUTION;
}

w_boolean_t vmsim_init(void) {
	
	exception_handler = w_add_exception_handler(vmsim_exc_handler);
	
	if(exception_handler == NULL) {
		return FALSE;
	}
	
	return TRUE;
}

w_boolean_t vmsim_cleanup(void) {

	return w_remove_exception_handler(exception_handler);
}

w_boolean_t vm_alloc(w_size_t num_pages, w_size_t num_frames, vm_map_t *map) {

	char ram_pref[] = "RAM";
	char swap_pref[] = "SWP";
	char ram_name[MAX_PATH];
	char swap_name[MAX_PATH];
	int rc, i;
	void *ret;
	LARGE_INTEGER ram_size, swap_size;

	if(num_frames > num_pages) {
		return FALSE;
	}
	
	//create new file for RAM and SWAP
	rc = GetTempFileName(".", ram_pref, 0, ram_name);
	DIE(rc == 0, "Unable to create ram name\n");
	
	rc = GetTempFileName(".", swap_pref, 0, swap_name);
	DIE(rc == 0, "Unable to create ram name\n");
	
	map->ram_handle = w_open_file(ram_name, MODE_FULL_OPEN);
	DIE(map->ram_handle == INVALID_HANDLE_VALUE, "Unable to open ram\n");
	
	map->swap_handle = w_open_file(swap_name, MODE_FULL_OPEN);
	DIE(map->swap_handle == INVALID_HANDLE_VALUE, "Unable to open swap\n");

	//truncate files to given size
	ram_size.QuadPart = p_sz * num_frames;
	if (SetFilePointerEx(map->ram_handle, ram_size, NULL, FILE_BEGIN) == 0 ||
	    SetEndOfFile(map->ram_handle) == 0) {
		DIE(1, "Unable to truncate ram\n");
	}
	
	swap_size.QuadPart = p_sz * num_pages;
	if (SetFilePointerEx(map->swap_handle, swap_size, NULL, FILE_BEGIN) == 0 ||
	    SetEndOfFile(map->swap_handle) == 0) {
		DIE(1, "Unable to truncate swap\n");
	}

	//allocate memory
	map->start = VirtualAlloc(NULL,
							num_pages * p_sz,
							MEM_RESERVE,
							PAGE_NOACCESS);
	if(map->start == NULL) {
		return FALSE;
	}
	rc = VirtualFree(map->start, 0, MEM_RELEASE);
	if(rc == -1) {
		return FALSE;
	}

	for(i = 0; i < num_pages; i++) {
		ret = VirtualAlloc((char*)map->start + i * p_sz,
							p_sz,
							MEM_RESERVE,
							PAGE_NOACCESS);
		if(ret == NULL) {
			return FALSE;
		}
	}

	//allocate internal tracking mechanism
	alloc_virt_mem(num_pages, num_frames, map, ram_name, swap_name);

	return TRUE;
}

//free all memory, close and delete files
w_boolean_t vm_free(w_ptr_t start) {

	struct mem_alloc *current_mem = virtual_mem, *aux;
	int size, i;

	if(start == NULL) {
		return FALSE;
	}

	while(current_mem != NULL) {

		if(current_mem->vm_map->start == start) {
			vm_map_t *map = current_mem->vm_map;
			int rc;

			//free memory and delete files
			size = w_get_file_size_by_handle(map->swap_handle) / p_sz;

			for(i = 0; i < size; i++) {
				rc = VirtualFree((char*)map->start + i * p_sz, 0, MEM_RELEASE);
				if(rc == -1) {
					return FALSE;
				}
			}
	
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
