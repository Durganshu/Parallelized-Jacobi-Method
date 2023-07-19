/*
 * heat.h
 *
 * Global definitions for the iterative solver
 */

#ifndef JACOBI_H_INCLUDED
#define JACOBI_H_INCLUDED

#include <stdio.h>
#include <mpi.h>

#ifdef HYBRID
#include <omp.h>
#endif
// configuration

typedef struct
{
    float posx;
    float posy;
    float range;
    float temp;
}
heatsrc_t;

typedef struct
{
    unsigned maxiter;       // maximum number of iterations
    unsigned act_res_x;
    unsigned act_res_y;
    unsigned global_res;
    unsigned max_res;       // spatial resolution
    unsigned initial_res;
    unsigned res_step_size;
    unsigned global_visres;        // visualization resolution
    unsigned local_visres_x;
    unsigned local_visres_y;
    unsigned x_start;
    unsigned y_start;
    unsigned x_end;
    unsigned y_end;
    

    int my_rank, top_rank, bottom_rank, left_rank, right_rank, extra_x, extra_y, x_rnk, y_rnk;     
  
    double *u, *uhelp;
    double *uvis;

    unsigned   numsrcs;     // number of heat sources
    heatsrc_t *heatsrcs;

    unsigned num_process_x; // num processors in x direction
    unsigned num_process_y;
}
algoparam_t;


// function declarations

// misc.c
int initialize( algoparam_t *param );
int finalize( algoparam_t *param );
void write_image( FILE * f, double *u,
		  unsigned sizex, unsigned sizey );
int coarsen(double *uold, unsigned oldx, unsigned oldy ,
	    double *unew, unsigned newx, unsigned newy );

// Gauss-Seidel: relax_gauss.c
double residual_gauss( double *u, double *utmp,
		       unsigned sizex, unsigned sizey );
void relax_gauss( double *u, 
		  unsigned sizex, unsigned sizey  );

// Jacobi: relax_jacobi.c
double residual_jacobi( double *u,
			unsigned sizex, unsigned sizey );

#ifdef NON_BLOCKING
double inner_relax_jacobi( algoparam_t *param, double **u, double **utmp,
		   unsigned sizex, unsigned sizey ); 

double outer_relax_jacobi( algoparam_t *param, double **u, double **utmp,
		   unsigned sizex, unsigned sizey ); 
#else
double relax_jacobi( double **u, double **utmp,
		   unsigned sizex, unsigned sizey ); 
#endif




#endif // JACOBI_H_INCLUDED
