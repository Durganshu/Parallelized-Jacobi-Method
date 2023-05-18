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
	 int i, j;
    double diff, sum = 0.0;
    double *temp = *u;
    
    int block_size = (64 / sizeof(double))/4;

    for (i = 1; i < sizey - 1; i += block_size) {
        for (j = 1; j < sizex - 1; j += block_size) {
            for (int ib = i; ib < i + block_size; ib++) {
                for (int jb = j; jb < j + block_size; jb++) {
                    (*utmp)[ib * sizex + jb] = 0.25 * ((*u)[ib * sizex + (jb - 1)] +  // left
                                                       (*u)[ib * sizex + (jb + 1)] +  // right
                                                       (*u)[(ib - 1) * sizex + jb] +  // top
                                                       (*u)[(ib + 1) * sizex + jb]); // bottom
                }
            }
        }
    }
    
    // Swap pointers to update the main array
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
