/* Heap based indexed priority queue as described by Gibson and Bruck
   2000. The heap is a minheap, and the values of tau are keys. */

#ifndef PRIORITYQ
#define PRIORITYQ 1


/* heap parameters */
#define HEAPINITSIZE 128
#define HEAPEXPANDFACTOR 2

#include <stdint.h>

#include "llist.h"
#include "ssa.h"


/* convenience for heap array implementation level numbering these do
   not check is safe indexing operations */
#define LEFTCHILD(I)  (I * 2 + 1)
#define RIGHTCHILD(I) (I * 2 + 2)
#define PARENT(I)     ((I - 1) / 2)


/* Each node in the heap stores the tau value for the time of the next
   reaction, and the index of the reaction. */
typedef struct reaction{
	double tau;
        INDEX number;
        int32_t heappos;
	LLIST *affects;
        LLIST *creates;
        LLIST *destroys;
} REACTION;


typedef struct priorityqueue{
	REACTION **heap;
	uint32_t arraysize;
	uint32_t size;
} PQ;


PQ *pq_make();
void pq_free(PQ*);
REACTION *reaction_make(INDEX number);
void reaction_free(REACTION*);

/* attributes */
char pq_isempty(PQ*);
REACTION *pq_min(PQ*);

/*  operations */
REACTION *pq_insert(PQ*, REACTION*);
REACTION *pq_deletemin(PQ*);
REACTION *pq_delete(PQ*, REACTION*);
void pq_update(PQ*, REACTION*, double);

void printqueue(PQ*);

#endif
