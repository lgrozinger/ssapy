#ifndef LLISTH
#define LLISTH 1


typedef struct llist{
	struct llist *next;
	void *data;
} LLIST;


LLIST *llist_push(LLIST *, void *);
void llist_delete(LLIST **, void *);
LLIST *llist_search(LLIST *, void *);
void llist_free(LLIST *);

#endif
