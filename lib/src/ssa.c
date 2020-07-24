#include <stdio.h>
#include <stdint.h>

#include "ssa.h"


void printstate(double t, COUNT *x, INDEX n)
{
	printf("%f", t);
	int i;
	for(i = 0; i < n; i++)
		printf(" %d", x[i]);
	printf("\n");
}
