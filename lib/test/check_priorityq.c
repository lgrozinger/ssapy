#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <time.h>

#include "priorityq.h"


START_TEST(check_heapsort)
{
	PQ *pq = pq_make();
	int i;
	for(i = 2048; i > 0; i--){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}

	for(i = 0; i < pq->size; i++){
		if(LEFTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[LEFTCHILD(i)]->tau);
		if(RIGHTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[RIGHTCHILD(i)]->tau);
	}

	pq_free(pq);
}
END_TEST


START_TEST(check_heappos)
{
	PQ *pq = pq_make();
	int i;
	for(i = 2048; i > 0; i--){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}
	for(i = 0; i < pq->size; i++)
		ck_assert(i == pq->heap[i]->heappos);

	
	pq_free(pq);
}
END_TEST


START_TEST(check_min)
{
	PQ *pq = pq_make();
	int i;
	for(i = 2048; i > 0; i--){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
		ck_assert(pq_min(pq)->tau == (double) i);
	}

	pq_free(pq);

}
END_TEST


START_TEST(check_decrease_last)
{
	PQ *pq = pq_make();
	int i;
	for(i = 2048; i > 0; i--){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}

	pq_update(pq, pq->heap[pq->size - 1], 0.5);
	ck_assert(pq_min(pq)->tau == 0.5);

	for(i = 0; i < pq->size; i++){
		if(LEFTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[LEFTCHILD(i)]->tau);
		if(RIGHTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[RIGHTCHILD(i)]->tau);
	}

	pq_free(pq);
}
END_TEST


START_TEST(check_increase_last)
{
	PQ *pq = pq_make();
	int i;
	for(i = 2048; i > 0; i--){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}
	pq_update(pq, pq->heap[pq->size - 1], 2049.0);

	for(i = 0; i < pq->size; i++){
		if(LEFTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[LEFTCHILD(i)]->tau);
		if(RIGHTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[RIGHTCHILD(i)]->tau);
	}

	pq_free(pq);
}
END_TEST

START_TEST(check_decrease_root)
{
	PQ *pq = pq_make();
	int i;
	for(i = 2048; i > 0; i--){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}

	pq_update(pq, pq->heap[0], 0.5);
	ck_assert(pq_min(pq)->tau == 0.5);

	for(i = 0; i < pq->size; i++){
		if(LEFTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[LEFTCHILD(i)]->tau);
		if(RIGHTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[RIGHTCHILD(i)]->tau);
	}

	pq_free(pq);
}
END_TEST


START_TEST(check_random_update)
{
	srand(time(NULL));
	PQ *pq = pq_make();
	int i;
	for(i = 2048; i > 0; i--){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}

	for(i = 0; i < pq->size; i++){
		int r = rand() % pq->size;
		double newvalue = (double) rand();
		pq_update(pq, pq->heap[r], newvalue);
	}

	for(i = 0; i < pq->size; i++){
		if(LEFTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[LEFTCHILD(i)]->tau);
		if(RIGHTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[RIGHTCHILD(i)]->tau);
	}
}
END_TEST


START_TEST(check_increase_root)
{
	PQ *pq = pq_make();
	int i;
	for(i = 2048; i > 0; i--){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}
	
	pq_update(pq, pq->heap[0], 2049.0);

	for(i = 0; i < pq->size; i++){
		if(LEFTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[LEFTCHILD(i)]->tau);
		if(RIGHTCHILD(i) < pq->size)
			ck_assert(pq->heap[i]->tau < pq->heap[RIGHTCHILD(i)]->tau);
	}

	pq_free(pq);
}
END_TEST


START_TEST(check_parent)
{
	ck_assert(PARENT(0) == 0);
}
END_TEST


START_TEST(check_delete_min)
{
	PQ *pq = pq_make();
	int i;
	for(i = 0; i < 2048; i++){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}

	for(i = 0; i < 2048; i++){
		REACTION *minimum = pq_deletemin(pq);
		ck_assert(((int) minimum->tau) == i);
		reaction_free(minimum);
	}

	pq_free(pq);
}
END_TEST


START_TEST(check_delete_last)
{
	PQ *pq = pq_make();
	int i;
	for(i = 0; i < 2048; i++){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}

	pq_delete(pq, pq->heap[pq->size - 1]);

	pq_free(pq);
}
END_TEST


START_TEST(check_delete_random)
{
	PQ *pq = pq_make();
	int i;
	for(i = 0; i < 2048; i++){
		REACTION *r = reaction_make(i);
		r->tau = (double) i;
		pq_insert(pq, r);
	}

	int size = pq->size;
	for(i = 0; i < size; i++){
		int r = rand() % pq->size;
		pq_delete(pq, pq->heap[r]);

		for(i = 0; i < pq->size; i++) {
			if(LEFTCHILD(i) < pq->size)
				ck_assert(pq->heap[i]->tau < pq->heap[LEFTCHILD(i)]->tau);
			if(RIGHTCHILD(i) < pq->size)
				ck_assert(pq->heap[i]->tau < pq->heap[RIGHTCHILD(i)]->tau);
		}
	}

	pq_free(pq);
}
END_TEST


Suite *pq_suite()
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("pq");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, check_heapsort);
	tcase_add_test(tc_core, check_min);
	tcase_add_test(tc_core, check_decrease_last);
	tcase_add_test(tc_core, check_increase_last);
	tcase_add_test(tc_core, check_decrease_root);
	tcase_add_test(tc_core, check_increase_root);
	tcase_add_test(tc_core, check_random_update);
	tcase_add_test(tc_core, check_parent);
	tcase_add_test(tc_core, check_heappos);
	tcase_add_test(tc_core, check_delete_min);
	tcase_add_test(tc_core, check_delete_random);
	tcase_add_test(tc_core, check_delete_last);
	suite_add_tcase(s, tc_core);

	return s;
}


int main(void)
{
	int number_failed;  
	Suite *s;  
	SRunner *sr;  

	s = pq_suite();  
	sr = srunner_create(s);  

	srunner_run_all(sr, CK_NORMAL);  
	number_failed = srunner_ntests_failed(sr);  
	srunner_free(sr);  
	return (number_failed == 0) ? 0 : 1;  
}
