/* Implementation for next reaction method */

#include <math.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "ssa.h"
#include "nrm.h"
#include "priorityq.h"
#include "depgraph.h"


const gsl_rng_type *SSARNGT;
gsl_rng *SSARNG;


void update_reaction(REACTION *r, SYSTEM *s, double t)
{
	double newp = ssa_h(s->R[r->number], s->x, s->n) * s->k[r->number];
	double oldp = s->propensities[r->number];

	if (newp > 0.0) {
		if (oldp > 0.0) {
			pq_update(s->pq, r, (oldp / newp) * (r->tau - t) + t);
		} else {
			r->tau = gsl_ran_gamma(SSARNG, s->steps[r->number], 1 / newp) + t;
			pq_insert(s->pq, r);
		}
	} else if (oldp > 0.0) {
		pq_delete(s->pq, r);
	}

	s->propensities[r->number] = newp;
}


double ssa_nrmstep(SYSTEM *s)
{
	REACTION *r = pq_min(s->pq);
        ssa_doreaction(s->R[r->number], s->P[r->number], s->x, s->n);
	double t = r->tau;
	
	s->propensities[r->number] = ssa_h(s->R[r->number], s->x, s->n) * s->k[r->number];
	if (s->propensities[r->number] > 0.0)
		pq_update(s->pq, r, gsl_ran_gamma(SSARNG, s->steps[r->number], 1 / s->propensities[r->number]) + t);
	else
		pq_delete(s->pq, r);
	
	LLIST *head = r->affects;
	while (head != NULL) {
		REACTION *update = (REACTION *) head->data;
		update_reaction(update, s, t);
		head = head->next;
	}

	return t;
}


REACTION **setup_reactions(SYSTEM *s)
{
	REACTION **reactions = malloc(sizeof(REACTION*) * s->m);
	char **adjmat = adjacencymatrix(s->R, s->P, s->n, s->m);
	
	INDEX i, j;
	for (i = 0; i < s->m; i++) {
		REACTION *reaction = reaction_make(i);
		reactions[i] = reaction;
	}
	
	for (i = 0; i < s->m; i++) {
		REACTION *r = reactions[i];
		for (j = 0; j < s->m; j++)
			if (i != j && adjmat[i][j] != 0)
				r->affects = llist_push(r->affects, reactions[j]);
	}

        for (i = 0; i < s->m; i++) {
		REACTION *r = reactions[i];
		for (j = 0; j < s->m; j++) {
			if (s->creates[i][j] != 0)
				r->creates = llist_push(r->creates, reactions[j]);
                        if (s->destroys[i][j] != 0)
                                r->destroys = llist_push(r->destroys, reactions[j]);
                }
	}

	for (i = 0; i < s->m; i++)
		free(adjmat[i]);
	
	free(adjmat);
	return reactions;
}


void ssa_nrm(INDEX **R, INDEX **P, INDEX n, INDEX m, double *k, COUNT *x, COUNT *steps, INDEX **creates, INDEX **destroys, double T)
{
	gsl_rng_env_setup();
	SSARNGT = gsl_rng_default;
	SSARNG = gsl_rng_alloc(SSARNGT);
	gsl_rng_set(SSARNG, time(NULL));

        SYSTEM *s = malloc(sizeof(SYSTEM));
	s->m = m;
        s->n = n;
        s->R = R;
        s->P = P;
        s->creates = creates;
        s->destroys = destroys;

	REACTION **reactions = setup_reactions(s);
	double *propensities = malloc(sizeof(double) * m);
	PQ *pq = pq_make();

        s->pq = pq;
        s->x = x;
        s->steps = steps;
        s->k = k;
        s->propensities = propensities;
	
	INDEX i;
	for (i = 0; i < m; i++) {
		REACTION *r = reactions[i];
		propensities[i] = ssa_h(R[i], x, n) * k[i];
		if (propensities[i] > 0.0) {
			r->tau = gsl_ran_gamma(SSARNG, steps[i], 1 / propensities[i]);
			pq_insert(pq, r);
		}
	}

	ssa_printstate(0.0, x, n);
	
	while(!pq_isempty(pq) && pq_min(pq)->tau < T)
		ssa_printstate(ssa_nrmstep(s), x, n);

	pq_free(pq);
	gsl_rng_free(SSARNG);
}
