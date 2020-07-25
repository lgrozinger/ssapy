#include <stdio.h>
#include <stdint.h>

#include "ssa.h"


void ssa_printstate(double t, COUNT *x, INDEX n)
{
	printf("%f", t);
	INDEX i;
	for(i = 0; i < n; i++)
		printf(" %d", x[i]);
	printf("\n");
}

/* mutate `x`, the vector of molecule counts, according to `v`, the
stoichiometry vector of the reaction to be performed. */
void ssa_doreaction(INDEX *r, INDEX *p, COUNT *x, INDEX n)
{
	INDEX i;
	for (i = 0; i < n; i++)
		x[i] = x[i] + p[i] - r[i];
}


/* compute the number of ways the reactants in `r` can combine, based
on their counts in `x` */
COUNT ssa_h(INDEX *r, COUNT *x, INDEX n)
{
	COUNT c = 1;
	INDEX i, j;
	for(i = 0; i < n; i++)
		for(j = 0; j < r[i]; j++)
			c *= x[i] - j;
	return c;
}

