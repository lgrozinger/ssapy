#ifndef NEXTREACTIONM
#define NEXTREACTIONM 1

#include <stdlib.h>
#include "ssa.h"
#include "priorityq.h"


typedef struct system {
        PQ *pq;
        INDEX **R;
        INDEX **P;
        INDEX **creates;
        INDEX **destroys;
        INDEX n;
        INDEX m;
        COUNT *x;
        COUNT *steps;
        double *k;
        double *propensities;
} SYSTEM;


void ssa_nrm(INDEX **R,
	     INDEX **P,
	     INDEX n,
	     INDEX m,
	     double *k,
	     COUNT *x,
	     COUNT *steps,
             INDEX **creates,
             INDEX **destroys,
	     double T);

#endif
