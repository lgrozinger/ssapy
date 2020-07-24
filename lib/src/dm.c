/* Implementation for Gillespie's direct method */

#include <math.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "ssa.h"
#include "dm.h"


const gsl_rng_type *SSARNGT;
gsl_rng *SSARNG;


void dm_doreaction(INDEX **R, INDEX **P, COUNT *x, INDEX r, INDEX n)
{
	int i;
	for(i = 0; i < n; i++)
		x[i] = x[i] + P[r][i] - R[r][i];
}


uint32_t dm_h(INDEX *r, COUNT *x, INDEX n)
{
	uint32_t c = 1;
	INDEX i, j;
	for(i = 0; i < n; i++)
		for(j = 0; j < r[i]; j++)
			c *= x[i] - j;
	return c;
}


INDEX nextreaction(double *propensities, INDEX m)
{
	double r1 = gsl_ran_flat(SSARNG, 0.0, 1.0) * propensities[m - 1];
	INDEX i = 0;
	for(i = 0; i < m; i++)
		if(r1 <= propensities[i])
			return i;
	return -1;
}


/* 
Perform one step of the Gillespie direct method, selecting a
reaction to perform and altering molecule counts and time
accordingly.
*/
void ssa_dmstep(INDEX n, INDEX m, double *propensities, INDEX **R, INDEX **P, double *k, COUNT *x, double *t)
{
	propensities[0] = dm_h(R[0], x, n) * k[0];
	INDEX i;
	for (i = 1; i < m; i++) {
		propensities[i] = dm_h(R[i], x, n) * k[i] + propensities[i - 1];
	}

	if (propensities[m - 1] > 0.0) {
		*t += gsl_ran_exponential(SSARNG, 1 / propensities[m - 1]);
		INDEX next = nextreaction(propensities, m);
		dm_doreaction(R, P, x, next, n);
	}
}


void ssa_dm(INDEX **R, INDEX **P, INDEX n, INDEX m, double *k, COUNT *x, double T)
{
	gsl_rng_env_setup();
	SSARNGT = gsl_rng_default;
	SSARNG = gsl_rng_alloc(SSARNGT);
	gsl_rng_set(SSARNG, time(NULL));

        double t = 0.0;
        double *propensities = malloc(sizeof(double) * m);

	do {
		ssa_dmstep(n, m, propensities, R, P, k, x, &t);
		printf("%f", t);
		INDEX i;
		for(i = 0; i < n; i++)
			printf(" %d", x[i]);
		printf("\n");
	} while(t < T && propensities[m - 1] > 0.0);

	free(propensities);
}
