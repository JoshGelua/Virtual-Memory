#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

static int clock;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict() {
	// Loop until we find an index s.t. the frame ref is 0
	while (coremap[clock].referenced != 0){
		coremap[clock].referenced = 0;
		clock = (clock + 1) % memsize;
	}
	int val = clock;
	clock = (clock + 1) % memsize;
	return val;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	// Get frame number from page table and set its ref to 1
	unsigned int frame_num = p->frame >> PAGE_SHIFT;
	coremap[frame_num].referenced = 1;
	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm.
 */
void clock_init() {
	clock = 0;
}
