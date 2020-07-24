#include <stdlib.h>
#include "llist.h"


LLIST *llist_push(LLIST *head, void *data)
{
	LLIST *newnode = malloc(sizeof(LLIST));
	newnode->data = data;
	newnode->next = head;
	return newnode;
}


LLIST *llist_search(LLIST *head, void *data)
{
	LLIST *node = head;
	while (node != NULL && node->data != data)
		node = node->next;

	return node;
}


void llist_delete(LLIST **head, void *data)
{
	LLIST *prev = NULL;
	LLIST *walk = *head;

	while (walk != NULL && walk->data != data) {
		prev = walk;
		walk = walk->next;
	}

	if (prev == NULL && walk != NULL) {
		LLIST *tmp = walk->next;
		if (tmp != NULL){
			walk->data = tmp->data;
			walk->next = tmp->next;
			free(tmp);
		}
		else {
			free(*head);
			*head = NULL;
		}
	}
	else if (walk != NULL) {
		prev->next = walk->next;
		free(walk);
	}
}


void llist_free(LLIST *head)
{
	LLIST *walk = head;

	while (walk != NULL){
		LLIST *freed = walk;
		walk = walk->next;
		free(freed);
	}
}
