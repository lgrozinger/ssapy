#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <time.h>

#include "llist.h"


START_TEST(check_push)
{
	LLIST *ll = NULL;
	int data[1024];
	int i;
	for (i = 0; i < 1024; i++) {
		data[i] = i;
		ll = llist_push(ll, data + i);
		ck_assert(*((int *) ll->data) == i);
	}

	LLIST *walk = ll;
	i = 1023;
	while (i >= 0) {
		ck_assert(*((int *)walk->data) == i);
		walk = walk->next;
		i--;
	}
}
END_TEST


START_TEST(check_free)
{
	LLIST *ll = NULL;
	int data[1024];
	int i;
	for (i = 0; i < 1024; i++) {
		data[i] = i;
		ll = llist_push(ll, data + i);
	}

	llist_free(ll);
}
END_TEST


START_TEST(check_search)
{
	LLIST *ll = NULL;
	int data[1024];
	int i;
	for (i = 0; i < 1024; i++) {
		data[i] = i;
		ll = llist_push(ll, data + i);
	}

	for (i = 0; i < 1024; i++) {
		LLIST *found = llist_search(ll, data + i);
		ck_assert(*((int *)found->data) == data[i]);
	}

}
END_TEST


START_TEST(check_delete_head)
{
	LLIST *ll = NULL;
	int data[1024];
	int i;
	for (i = 0; i < 1024; i++) {
		data[i] = i;
		ll = llist_push(ll, data + i);
	}

	for (i = 1023; i >= 0; i--) {
		ck_assert(*((int *) ll->data) == i);
		llist_delete(&ll, data + i);
	}

	llist_free(ll);
}
END_TEST


START_TEST(check_delete)
{
	LLIST *ll = NULL;
	int data[1024];
	int i;
	for (i = 0; i < 1024; i++) {
		data[i] = i;
		ll = llist_push(ll, data + i);
	}

	srand(time(NULL));
	int r = rand() % 1024;
	
	llist_delete(&ll, data + r);
	ck_assert(llist_search(ll, data + r) == NULL);

	llist_free(ll);
}
END_TEST


START_TEST(check_delete_last)
{
	LLIST *ll = NULL;
	int data[1024];
	int i;
	for (i = 0; i < 1024; i++) {
		data[i] = i;
		ll = llist_push(ll, data + i);
	}

	llist_delete(&ll, data + 1023);
	ck_assert(llist_search(ll, data + 1023) == NULL);

	llist_free(ll);
}
END_TEST


Suite *llist_suite()
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("llist");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, check_push);
	tcase_add_test(tc_core, check_free);
	tcase_add_test(tc_core, check_search);
	tcase_add_test(tc_core, check_delete);
	tcase_add_test(tc_core, check_delete_head);
	tcase_add_test(tc_core, check_delete_last);
	suite_add_tcase(s, tc_core);

	return s;
}


int main(void)
{
	int number_failed;  
	Suite *s;  
	SRunner *sr;  

	s = llist_suite();  
	sr = srunner_create(s);  

	srunner_run_all(sr, CK_NORMAL);  
	number_failed = srunner_ntests_failed(sr);  
	srunner_free(sr);  
	return (number_failed == 0) ? 0 : 1;  
}
