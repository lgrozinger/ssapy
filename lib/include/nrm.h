#ifndef NEXTREACTIONM
#define NEXTREACTIONM 1

#include <stdlib.h>
#include "ssa.h"
#include "priorityq.h"


void ssa_nrm(INDEX **R,
	     INDEX **P,
	     INDEX n,
	     INDEX m,
	     double *k,
	     COUNT *x,
	     COUNT *steps,
	     double T);

#endif
