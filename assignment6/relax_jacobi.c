/*
 * relax_jacobi.c
 *
 * Jacobi Relaxation
 *
 */

#include "heat.h"

double relax_jacobi( double **u1, double **utmp1,
         unsigned sizex, unsigned sizey )
{
  int i, j;
  double *help,*u, *utmp,factor=0.5;

  utmp=*utmp1;
  u=*u1;
  double unew, diff, sum=0.0;

// i = 1 to 20
// j = 1 to 20

  for( i=1; i<sizey-1; i++ ) {
  	int ii=i*sizex;
  	int iim1=(i-1)*sizex;
  	int iip1=(i+1)*sizex;

    for( j=1; j<sizex-1; j++ ){
       utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
        		            u[ ii+(j+1) ]+
        		            u[ iim1+j ]+
        		            u[ iip1+j ]);
		    diff = utmp[ii + j] - u[ii + j];
		    // utmp[ii+j] = unew;
		    sum += diff * diff;

       }
    }

  *u1=utmp;
  *utmp1=u;
  return(sum);
}


double inner_relax_jacobi( algoparam_t *param, double **u1, double **utmp1,
		   unsigned sizex, unsigned sizey )
{
  int i, j;
  double *help,*u, *utmp,factor=0.5;

  utmp=*utmp1;
  u=*u1;
  double unew, diff, sum=0.0;

int istart = 2, iend = sizey - 2;
int jstart = 2, jend = sizex - 2;
// printf("rank = %d, left-rank = %d, right-rank = %d, top-rank = %d, bottom_rank = %d\n", param->my_rank, param->left_rank, param->right_rank, param->top_rank, param->bottom_rank);
//printf("Inner Jacobi: rank = %d, istart = %d, iend = %d, jstart = %d, jend = %d\n", param->my_rank, istart, iend, jstart, jend);
// printf("istart = %d, iend = %d, jstart = %d, jend = %d\n", istart, iend, jstart, jend);
  for( i=istart; i< iend; i++ ) {
  	int ii=i*sizex;
  	int iim1=(i-1)*sizex;
  	int iip1=(i+1)*sizex;

    for( j=jstart; j< jend; j++ ){
       utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
        		            u[ ii+(j+1) ]+
        		            u[ iim1+j ]+
        		            u[ iip1+j ]);
		    diff = utmp[ii + j] - u[ii + j];
		    sum += diff * diff;
        // printf("%lf ", utmp[ ii+ j]);
       }
      //  printf("\n");
  }
  // if(sizey == 22){
  //   printf(" Old_values: \n");
  //   for(int i=0; i < sizey; i++){
  //     for(int j=0; j < sizex;j++){
  //       printf("%lf ", (*utmp1)[i*sizex + j]);
  //     }
  //     printf("\n");
  //   }
  //   printf(" New_values: \n");
  //   for(int i=0; i < sizey; i++){
  //     for(int j=0; j < sizex;j++){
  //       printf("%lf ", utmp[i*sizex + j]);
  //     }
  //     printf("\n");
  //   }
  // }
  // *u1=utmp;
  // *utmp1=u;

  return(sum);
}

double outer_relax_jacobi( algoparam_t *param, double **u1, double **utmp1,
         unsigned sizex, unsigned sizey )
{
  int i, j;
  double *help,*u, *utmp,factor=0.5;

  utmp=*utmp1;
  u=*u1;
  double unew, diff, sum=0.0;

  i = 1;
  int ii=i*sizex;
  int iim1=(i-1)*sizex;
  int iip1=(i+1)*sizex;
  //top row
  for( j=1; j< sizex -1; j++ )
	{
      utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
                        u[ ii+(j+1) ]+
                        u[ iim1+j ]+
                        u[ iip1+j ]);
        diff = utmp[ii + j] - u[ii + j];
        sum += diff * diff;
  }

  //bottom row
  i = sizey - 2;
  ii=i*sizex;
  iim1=(i-1)*sizex;
  iip1=(i+1)*sizex;
  for( j=1; j< sizex -1; j++ )
	{
      utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
                        u[ ii+(j+1) ]+
                        u[ iim1+j ]+
                        u[ iip1+j ]);
        diff = utmp[ii + j] - u[ii + j];
        sum += diff * diff;
  }

  //left col
  for( i=2; i< sizey -2; i++ )
	{
      j = 1;
      ii=i*sizex;
      iim1=(i-1)*sizex;
      iip1=(i+1)*sizex;
      utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
                        u[ ii+(j+1) ]+
                        u[ iim1+j ]+
                        u[ iip1+j ]);
      diff = utmp[ii + j] - u[ii + j];
      sum += diff * diff;
  }

  //right col
  for( i=2; i< sizey -2; i++ )
	{
      j = sizex - 2;
      ii=i*sizex;
      iim1=(i-1)*sizex;
      iip1=(i+1)*sizex;
      utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
                        u[ ii+(j+1) ]+
                        u[ iim1+j ]+
                        u[ iip1+j ]);
      diff = utmp[ii + j] - u[ii + j];
      sum += diff * diff;
  }
  // if(sizey == 22){
    // printf("sizey=%d\n", sizey);
    // printf(" Old_values: \n");
    // for(int i=0; i < sizey; i++){
    //   for(int j=0; j < sizex;j++){
    //     printf("%lf ", (*utmp1)[i*sizex + j]);
    //   }
    //   printf("\n");
    // }
   
  // }
  *u1=utmp;
  *utmp1=u;
  
  return(sum);
}


