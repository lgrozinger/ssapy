git commit -m 'ssa frequency analysis'
git config --global --edit
git push
(expt 2 15)
(expt 2 12)
(expt 2 13)
cd my/phd/
ls
pdflatex formulae.tex 
pdflatex formulae.tex 
pdflatex formulae.tex 
pdflatex formulae.tex 
pdflatex formulae.tex 
pdflatex formulae.tex 
pdflatex formulae.tex 
pdflatex formulae.tex 
pdflatex formulae.tex 
pdflatex formulae.tex 
wget igema.sevahub.es
cd /etc/
ls
ls resolv.conf
find-file resolv.conf
cd /ssh:b8051106@linux.cs.ncl.ac.uk:~
ls
ssh b8051106@cypher214.ncl.ac.uk
scp b8051106@cypher214.ncl.ac.uk:~/my/src/python/interface/model.py .
ls
exit
which gfortran
cd my/src/fortran/
ls
cd euler/
ls
gfortran task1.f90 
ls
./a.out 
gfortran task1.f90 
./a.out 
gfortran task1.f90 
gfortran task1.f90 
./a.out 
gfortran task1.f90 
./a.out 
gfortran task1.f90 
./a.out 
gfortran task1.f90 
./a.out 
gfortran task1.f90 
./a.out 
gfortran task1.f90 
./a.out 
gfortran task1.f90 
./a.out 
gfortran task1.f90 
./a.out 
cd my/src/fortran/tsunami/
gfortran hello.f90 -o hello
./hello 
cd /ssh:b8051106@linux.cs.ncl.ac.uk:~/
scp b8051106@cypher214.ncl.ac.uk:~/my/src/python/quinones/main.py .
ls
ls
rm *.zip
ls
cd ~
ls
scp b8051106@linux.cs.ncl.ac.uk:~/main.py .
ls
cd my/src/python
ls
mkdir quinone
cd quinone/
ls
mv ~/main.py .
ls
python
python3
python3 -m pip install tkinter
python3 -m pip install tkinter --user
xmodmap ~/.xmodmapdefs 
cd my/src/python/
ls
cd pynterface/
ls
mkdir pynterface
touch pynterface/__init__.py
ls
mv kinetics.py ./pynterface/
ls
cat ~/scratch/simple_marcus.pdf | netcat -w 1 -h 10.66.178.114 -p 9100
cat ~/scratch/simple_marcus.pdf | nc -w 1 -h 10.66.178.114 -p 9100
cat ~/scratch/simple_marcus.pdf | nc -w 1 -h 10.66.178.114  9100
cat ~/scratch/simple_marcus.pdf | nc  10.66.178.114  9100 -w 1
cat ~/scratch/simple_marcus.pdf | nc -q 1 10.66.178.114 9100
which nc
(* 0.75 120)
(* 1.125 120)
rm -r my/media/Hero\ 2002/
rm -r my/media/Hero\ Theatrical\ Cut\ 2002/
rm -r my/media/Ying\ xiong\ aka\ Hero\ 2002/
mkdir my/sauce
mkdir my/sauce/biter
cd my/sauce/biter
ls
cd ..
rmdir biter/
mkdir coinbot
cd coinbot
mkdir conn
touch conn/__init__.py
cd conn
python3 -m pip install cbpro
python3 -m cbpro.orderbook
python3 -m cbpro.order_book
python3 -m cbpro.order_book
cd code
ls
git clone https://github.com/lgrozinger/ssapy
cd ssapy/
ls
cp ../ssa/src/ssa.py .
cd ssapy/
python3
ls
ls ..
cp ../../ssa/src/read.py .
ls
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

const INDEX N;
const INDEX M;

/* mutate `x`, the vector of molecule counts, according to `v`, the
stoichiometry vector of the reaction to be performed. */
void nrm_doreaction(INDEX *v, COUNT *x)
{
	INDEX i;
	for (i = 0; i < N; i++)
		x[i] += v[i];
}

/* compute the number of ways the reactants in `r` can combine, based
on their counts in `x` */
uint32_t nrm_h(INDEX *r, COUNT *x, INDEX n)
{
	uint32_t c = 1;
	INDEX i, j;
	for(i = 0; i < n; i++)
		for(j = 0; j < r->reactants[i]; j++)
			c *= x[i] - j;
	return c;
}

double calculate_new_tau(INDEX *r, 

void update_reaction(PQ *pq, REACTION *r, COUNT *x, INDEX n, double t)
{
	double p = nrm_h(r, x, n) * r->rate;
	if (p > 0.0) {
		if (r->propensity > 0.0) {
			r->propensity = p;
			pq_update(pq, r, (r->propensity / p) * (r->tau - t) + t);
		} else {
			r->propensity = p;
			r->tau = gsl_ran_gamma(SSARNG, r->steps, 1 / p) + t;
			pq_insert(pq, r);
		}
	} else if (r->propensity > 0.0) {
		pq_delete(pq, r);
	}
}


double ssa_nrmstep(INDEX n, INDEX m, INDEX **R, INDEX **P, COUNT *x, PQ *pq)
{
	REACTION *r = pq_min(pq);

        double t = r->tau;

        /* if (r->steps > 0) { */
        /*         REACTION *delayed = reaction_make(); */
        /*         delayed->propensity = r->delayrate; */
        /*         delayed->tau = gsl_ran_gamma(SSARNG, r->steps, r->delayrate) + t; */
        /*         delayed->steps = 1; */
        /*         delayed->products = r->products; */
        /*         delayed->reactants = malloc(sizeof(INDEX) * n); */
        /*         int i; */
        /*         for (i = 0; i < n; i++) */
        /*                 delayed->reactants[i] = 0; */
                
        /*         pq_insert(pq, delayed); */
        /* } else { */
        nrm_doreaction(r, x, n);
	r->propensity = nrm_h(r, x, n) * r->rate;
	if (r->propensity > 0.0)
		pq_update(pq, r, gsl_ran_gamma(SSARNG, r->propensity * r->steps, 1 / r->propensity) + t);
	else
		pq_delete(pq, r);
	

	LLIST *head = r->affects;
	while (head != NULL) {
		REACTION *update = (REACTION *) head->data;
		update_reaction(pq, update, x, n, t);
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
		reaction->rate = k[i];
		reaction->reactants = R[i];
		reaction->products = P[i];
                reaction->steps = steps[i];
		reactions[i] = reaction;
	}
	
	for (i = 0; i < m; i++) {
		REACTION *r = reactions[i];
		for (j = 0; j < m; j++)
			if (adjmat[i][j] != 0)
				r->affects = llist_push(r->affects, reactions[j]);
	}

	for (i = 0; i < m; i++)
		free(adjmat[i]);
	
	free(adjmat);
	return reactions;
}


void ssa_nrm(INDEX **R, INDEX **P, INDEX n, INDEX m, double *k, COUNT *x, COUNT *steps, double T)
{
        N = n;
        M = m;
	gsl_rng_env_setup();
	SSARNGT = gsl_rng_default;
	SSARNG = gsl_rng_alloc(SSARNGT);
	gsl_rng_set(SSARNG, time(NULL));

	REACTION **reactions = setup_reactions(R, P, steps, k, n, m);
	
	PQ *pq = pq_make();
	INDEX i;
	for (i = 0; i < m; i++) {
		REACTION *r = reactions[i];
		r->propensity = nrm_h(r, x, n) * k[i];
		if (r->propensity > 0.0) {
			r->tau = gsl_ran_gamma(SSARNG, steps[i], 1 / r->propensity);
			pq_insert(pq, r);
		}
	}

	printstate(0.0, x, n);
	
	while(!pq_isempty(pq) && pq_min(pq)->tau < T){
		double t = ssa_nrmstep(n, m, R, P, x, pq);
		printstate(t, x, n);
	}

	pq_free(pq);
	gsl_rng_free(SSARNG);
}