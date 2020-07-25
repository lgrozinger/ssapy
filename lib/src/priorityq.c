#include <stdlib.h>
#include <stdio.h>
#include "llist.h"
#include "priorityq.h"


REACTION *reaction_make(INDEX number)
{
	REACTION *new = malloc(sizeof(REACTION));
	new->affects = NULL;
	new->number = number;
	new->heappos = -1;
	return new;
}


void reaction_free(REACTION *reaction)
{
	if (reaction->affects != NULL)
		llist_free(reaction->affects);
	
	free(reaction);
}


/* 
construct a new empty priority queue. allocates memory, so should be
paired with a call to pq_free
 */
PQ *pq_make()
{
	uint32_t m = HEAPINITSIZE;
	PQ *pq = malloc(sizeof(PQ));
	pq->heap = malloc(sizeof(REACTION *) * m);
	pq->arraysize = m;
	pq->size = 0;

	uint32_t i;
	for(i = 0; i < m; i++)
		pq->heap[i] = NULL;

	return pq;
}


/*
frees the memory associated with the priority queue 
*/
void pq_free(PQ *pq)
{
	uint32_t i;
	for(i = 0; i < pq->size; i++)
		reaction_free(pq->heap[i]);

	free(pq->heap);
	free(pq);
}


void expandheap(PQ *pq)
{
	uint32_t newsize = pq->arraysize * HEAPEXPANDFACTOR;
	REACTION **newheap = malloc(sizeof(REACTION *) * newsize);
	uint32_t i;
	for(i = 0; i < pq->arraysize; i++)
		newheap[i] = pq->heap[i];

	free(pq->heap);

	pq->heap = newheap;
	pq->arraysize = newsize;
}


REACTION *pq_min(PQ *pq)
{
	return pq->heap[0];
}


char pq_isempty(PQ *pq)
{
	return pq->size == 0;
}


int lt(REACTION* a, REACTION* b)
{
	return a->tau < b->tau;
}


void swap(PQ *pq, REACTION *r, REACTION *s)
{
	int32_t i = r->heappos;
	int32_t j = s->heappos;
	REACTION *tmp = pq->heap[i];
	pq->heap[i] = pq->heap[j];
	pq->heap[j] = tmp;
	pq->heap[i]->heappos = i;
	pq->heap[j]->heappos = j;
}


void bubbledown(PQ * pq, REACTION *r)
{
	REACTION *lc = NULL;
	REACTION *rc = NULL;
	if(LEFTCHILD(r->heappos) < pq->size)
		lc = pq->heap[LEFTCHILD(r->heappos)];

	if(RIGHTCHILD(r->heappos) < pq->size)
		rc = pq->heap[RIGHTCHILD(r->heappos)];


	REACTION *minchild = lc;
	if(rc != NULL)
		minchild = lt(lc, rc) ? lc : rc;

	if(minchild != NULL && lt(minchild, r)){
		swap(pq, minchild, r);
		bubbledown(pq, r);
	}

}


void bubbleup(PQ *pq, REACTION *r)
{
	uint32_t i = r->heappos;
	while(i > 0 && lt(pq->heap[i], pq->heap[PARENT(i)])){
		swap(pq, pq->heap[i], pq->heap[PARENT(i)]);
		i = PARENT(i);
	}
}


REACTION *pq_insert(PQ *pq, REACTION *reaction)
{
	if(pq->size == pq->arraysize)
		expandheap(pq);
	reaction->heappos = pq->size;
	pq->heap[pq->size] = reaction;
	pq->size += 1;
	bubbleup(pq, reaction);
	return reaction;
}


REACTION *pq_delete(PQ *pq, REACTION *victim)
{
	int32_t victimpos = victim->heappos;
	if (pq->size > 1 && victimpos != pq->size - 1) {
		swap(pq, victim, pq->heap[pq->size - 1]);
		pq->size -= 1;
		pq->heap[pq->size] = NULL;
		bubbledown(pq, pq->heap[victimpos]);
		victim->heappos = -1;
	} else {
		pq->size -= 1;
		pq->heap[pq->size] = NULL;
		victim->heappos = -1;
	}
	return victim;
}


REACTION *pq_deletemin(PQ *pq)
{
	return pq_delete(pq, pq->heap[0]);
}


void pq_update(PQ *pq, REACTION *r, double tau)
{
	r->tau = tau;
	int32_t i = r->heappos;
	REACTION *parent = pq->heap[PARENT(i)];
	if(r->tau < parent->tau)
		bubbleup(pq, r);
	else
		bubbledown(pq, r);
}


void printqueue(PQ *pq)
{
	printf("HEAP: ");
	uint32_t i;
	for(i = 0; i < pq->size; i++){
		printf(" %f", pq->heap[i]->tau);
	}

	printf("\n");
}
