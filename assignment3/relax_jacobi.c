/*
 * relax_jacobi.c
 *
 * Jacobi Relaxation
 *
 */

#include "heat.h"

/*
 * Residual (length of error vector)
 * between current solution and next after a Jacobi step
 */
double residual_jacobi(double *u, unsigned sizex, unsigned sizey) {
	unsigned i, j;
	double unew, diff, sum = 0.0;

	for (i = 1; i < sizey - 1; i++) {
		for (j = 1; j < sizex - 1; j++) {
		
			unew = 0.25 * (u[i * sizex + (j - 1)] +  // left
						u[i * sizex + (j + 1)] +  // right
						u[(i - 1) * sizex + j] +  // top
						u[(i + 1) * sizex + j]); // bottom

			diff = unew - u[i * sizex + j];
			sum += diff * diff;
		}
	}

	return sum;
}

/*
 * One Jacobi iteration step
 */
double relax_jacobi( double **u, double **utmp, unsigned sizex, unsigned sizey ){
	double diff, sum = 0.0;
	double *temp = *u;
	for (int i = 1; i < sizey - 1; i++) {
		for (int j = 1; j < sizex - 1; j++) {
			(*utmp)[i * sizex + j] = 0.25 * ((*u)[i * sizex + (j - 1)] +  // left
						(*u)[i * sizex + (j + 1)] +  // right
						(*u)[(i - 1) * sizex + j] +  // top
						(*u)[(i + 1) * sizex + j]); // bottom

		}
	}
	*u = *utmp;
  	*utmp = temp;
}
