#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

struct frame *head;
struct frame *tail;

/* Page to evict is chosen using the accurate LRU algorithm.
* Returns the page frame number (which is also the index in the coremap)
* for the page that is to be evicted.
*/

int lru_evict() {
	// Get the frame number
	int frame_num = head->pte->frame >> PAGE_SHIFT;
	// If last in queue
	if (head == tail) {
		tail = NULL;
	}
	// Mark the frame as not in queue
	coremap[frame_num].referenced = 0;
	// Evict head
	head->next->prev = NULL;
	head = head->next;

	return frame_num;
}

/* This function is called on each access to a page to update any information
* needed by the lru algorithm.
* Input: The page table entry for the page that is being accessed.
*/
void lru_ref(pgtbl_entry_t *p) {
	// Get frame number
	unsigned int frame_num = p->frame >> PAGE_SHIFT;
	// If in the queue
	if (coremap[frame_num].referenced == 1) {
		// If in the middle of the queue
		if (coremap[frame_num].prev != NULL && coremap[frame_num].next != NULL) {
			// Close the gap
			coremap[frame_num].prev->next = coremap[frame_num].next;
			coremap[frame_num].next->prev = coremap[frame_num].prev;
		}
		// If at the edges or alone
		else {
			// If alone or at tail, stop.
			if (coremap[frame_num].prev != NULL){
				return;
			}
			// If at head
			else{
				head->next->prev = NULL;
				head = coremap[frame_num].next;
			}
		}
		// Throw it to the tail
		coremap[frame_num].next = NULL;
		coremap[frame_num].prev = tail;
		tail->next = &coremap[frame_num];
		tail = &coremap[frame_num];
	}
	// If not in the queue
	else {
		// Mark it as in the queue now
		coremap[frame_num].referenced = 1;
		coremap[frame_num].next = NULL;
		// If the queue was empty
		if (tail == NULL) {
			tail = &coremap[frame_num];
			head = tail;
		}
		// If the queue wasn't empty
		else {
			//Throw it to the back
			coremap[frame_num].prev = tail;
			tail->next = &coremap[frame_num];
			tail = &coremap[frame_num];
		}
	}
}

/* Initialize any data structures needed for this
* replacement algorithm
*/
void lru_init() {
	// Set head and tail, and for every frame in memory, set the values and node
	// appropriately
	head = NULL;
	tail = NULL;
	for (int i = 0; i < memsize; i++) {
		coremap[i].referenced = 0;
		coremap[i].next = NULL;
		coremap[i].prev = NULL;
	}
}
