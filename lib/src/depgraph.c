#include <stdlib.h>
#include <stdio.h>
#include "ssa.h"

char** adjacencymatrix(INDEX **R, INDEX **P, INDEX n, INDEX m)
{
	char** adjmat = malloc(sizeof(char *) * m);
	INDEX i, j, k;
	for (i = 0; i < m; i++)
		adjmat[i] = malloc(sizeof(char) * m);

	for (i = 0; i < m; i++)
		for (j = 0; j < m; j++)
			adjmat[i][j] = 0;

	for(j = 0; j < m; j++)
		for(i = 0; i < m; i++)
			for (k = 0; k < n; k++)
				adjmat[i][j] |= (R[i][k] || P[i][k]) && R[j][k];
	
	return adjmat;
}
