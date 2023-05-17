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
	double diff, unew = 0.0, sum = 0.0;

	int block_size = (sizex / 2) + 1;
	//block_size = 6 = 10/2 + 1

	for (int i = 0; i < (sizey-2); i = i + block_size - 2) {
		for (int j = 0; j < (sizex-2); j = j + block_size - 2) {
			for (int ib = 1; ib < block_size - 1; ib++) {
				for (int jb = 1; jb < block_size - 1; jb++) {
		
					(*utmp)[(i+ib) * sizex + (j+jb)] = 0.25 * ((*u)[(i+ib) * sizex + (j+jb - 1)] +  // left
						(*u)[(i+ib) * sizex + (j+jb + 1)] +  // right
						(*u)[(i+ib - 1) * sizex + j+jb] +  // top
						(*u)[(i+ib + 1) * sizex + j+jb]); // bottom
				}
			}
		}
	}

	double* temp = *u;
    *u = *utmp;
    *utmp = temp;
	// for (i = 1; i < sizey - 1; i++) {
	// 	for (j = 1; j < sizex - 1; j++) {
			
	// 		u[i * sizex + j] = 0.25 * (utmp[i * sizex + (j - 1)] +  // left
	// 					utmp[i * sizex + (j + 1)] +  // right
	// 					utmp[(i - 1) * sizex + j] +  // top
	// 					utmp[(i + 1) * sizex + j]); // bottom
			
	// 		diff = u[i * sizex + j] - utmp[i * sizex + j];
	// 		sum += diff * diff;
	// 	}
	// }
	// // copy from utmp to u
	// for (i = 1; i < sizey - 1; i++) {
	// 	for (j = 1; j < sizex - 1; j++) {
		
	// 		u[i * sizex + j] = utmp[i * sizex + j];
	// 	}
	// }
	//return sum;
}
