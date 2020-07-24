#ifndef DIRECTM
#define DIRECTM 1

#include <stdlib.h>

#include "ssa.h"

#ifndef RANDDBL
#define RANDDBL (rand() / (double) RAND_MAX)
#endif


void ssa_dm(INDEX **R,
	    INDEX **P,
	    INDEX n,
	    INDEX m,
	    double *k,
	    COUNT *x,
	    double T);

#endif
