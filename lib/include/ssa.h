#ifndef SSA
#define SSA 1

#include <stdint.h>

typedef unsigned int INDEX;
typedef unsigned int COUNT;

void ssa_printstate(double, COUNT*, INDEX);

/* mutate `x`, the vector of molecule counts, according to `v`, the
stoichiometry vector of the reaction to be performed. */
void ssa_doreaction(INDEX *r, INDEX *p, COUNT *x, INDEX n);

/* compute the number of ways the reactants in `r` can combine, based
on their counts in `x` */
COUNT ssa_h(INDEX *r, COUNT *x, INDEX n);

#endif
