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

void update_reaction
(PQ *pq,
 REACTION *r,
 INDEX **R,
 double *propensities,
 double *k,
 COUNT *steps,
 COUNT *x,
 INDEX n,
 double t)
{
	double newp = ssa_h(R[r->number], x, n) * k[r->number];
	double oldp = propensities[r->number];

	if (newp > 0.0) {
		if (oldp > 0.0) {
			pq_update(pq, r, (oldp / newp) * (r->tau - t) + t);
		} else {
			r->tau = gsl_ran_gamma(SSARNG, steps[r->number], 1 / newp) + t;
			pq_insert(pq, r);
		}
	} else if (oldp > 0.0) {
		pq_delete(pq, r);
	}

	propensities[r->number] = newp;
}


/* 				  void update_reaction(PQ *pq, REACTION *r, INDEX **R, COUNT *x, INDEX n, double t) */
/* { */
/* 	double p = ssa_h(r, x, n) * r->rate; */
/* 	if (p > 0.0) { */
/* 		if (r->propensity > 0.0) { */
/* 			r->propensity = p; */
/* 			pq_update(pq, r, (r->propensity / p) * (r->tau - t) + t); */
/* 		} else { */
/* 			r->propensity = p; */
/* 			r->tau = gsl_ran_gamma(SSARNG, r->steps, 1 / p) + t; */
/* 			pq_insert(pq, r); */
/* 		} */
/* 	} else if (r->propensity > 0.0) { */
/* 		pq_delete(pq, r); */
/* 	} */
/* } */


double ssa_nrmstep
(INDEX n,
 INDEX m,
 INDEX **R,
 INDEX **P,
 double *propensities,
 double *k,
 COUNT *steps,
 COUNT *x,
 PQ *pq)
{

	REACTION *r = pq_min(pq);
        ssa_doreaction(R[r->number], P[r->number], x, n);
	double t = r->tau;
	
	propensities[r->number] = ssa_h(R[r->number], x, n) * k[r->number];
	if (propensities[r->number] > 0.0)
		pq_update(pq, r, gsl_ran_gamma(SSARNG, steps[r->number], 1 / propensities[r->number]) + t);
	else
		pq_delete(pq, r);
	
	LLIST *head = r->affects;
	while (head != NULL) {
		REACTION *update = (REACTION *) head->data;
		update_reaction(pq, update, R, propensities, k, steps, x, n, t);
		head = head->next;
	}

	return t;
}


REACTION **setup_reactions(INDEX **R, INDEX **P, COUNT *steps, double *k, INDEX n, INDEX m)
{
	REACTION **reactions = malloc(sizeof(REACTION*) * m);
	char **adjmat = adjacencymatrix(R, P, n, m);
	
	INDEX i, j;
	for (i = 0; i < m; i++) {
		REACTION *reaction = reaction_make(i);
		reactions[i] = reaction;
	}
	
	for (i = 0; i < m; i++) {
		REACTION *r = reactions[i];
		for (j = 0; j < m; j++)
			if (i != j && adjmat[i][j] != 0)
				r->affects = llist_push(r->affects, reactions[j]);
	}

	for (i = 0; i < m; i++)
		free(adjmat[i]);
	
	free(adjmat);
	return reactions;
}


void ssa_nrm(INDEX **R, INDEX **P, INDEX n, INDEX m, double *k, COUNT *x, COUNT *steps, double T)
{
	gsl_rng_env_setup();
	SSARNGT = gsl_rng_default;
	SSARNG = gsl_rng_alloc(SSARNGT);
	gsl_rng_set(SSARNG, time(NULL));

	REACTION **reactions = setup_reactions(R, P, steps, k, n, m);
	double *propensities = malloc(sizeof(double) * m);
	
	PQ *pq = pq_make();
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
	
	while(!pq_isempty(pq) && pq_min(pq)->tau < T){
		double t = ssa_nrmstep(n, m, R, P, propensities, k, steps, x, pq);
		ssa_printstate(t, x, n);
	}

	pq_free(pq);
	gsl_rng_free(SSARNG);
}
