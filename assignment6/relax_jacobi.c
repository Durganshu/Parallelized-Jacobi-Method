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

// i = 2 to 19
// j = 2 to 19
int istart = 2, iend = sizey - 2;
int jstart = 2, jend = sizex - 2;
// printf("rank = %d, left-rank = %d, right-rank = %d, top-rank = %d, bottom_rank = %d\n", param->my_rank, param->left_rank, param->right_rank, param->top_rank, param->bottom_rank);
//if there's no left rank
if(param->left_rank < 0){
//printf("Came inside left-rank <0, rank = %d\n", param->my_rank);
  if(param->right_rank < 0){
    
    jstart = 1;
    jend = sizex - 2;
    
    if(param->top_rank < 0 && param->bottom_rank < 0){
      istart = 1;
      iend = sizey - 2;
    }
    else if(param->top_rank < 0 && param->bottom_rank >= 0){
      istart = 1;
      iend = sizey - 3;
    }
    else if(param->bottom_rank < 0 && param->top_rank >= 0){
      istart = 2;
      iend = sizey - 2;
    }

  }
  else {
    //printf("Came inside positive right rank\n");
    jstart = 1;
    jend = sizex - 3;
    
    if(param->top_rank < 0 && param->bottom_rank < 0){
      istart = 1;
      iend = sizey - 2;
    }
    else if(param->top_rank < 0 && param->bottom_rank >= 0){
      istart = 1;
      iend = sizey - 3;
    }
    else if(param->bottom_rank < 0 && param->top_rank >= 0){
      istart = 2;
      iend = sizey - 2;
    }
  }

}

// If there's left rank
else if (param->left_rank >= 0){
  //printf("Came inside left-rank >0, rank = %d\n", param->my_rank);
  if(param->right_rank < 0){
    //printf("Came inside negative right rank\n");
    jstart = 2;
    jend = sizex - 2;
    
    if(param->top_rank < 0 && param->bottom_rank < 0){
      istart = 1;
      iend = sizey - 2;
    }
    else if(param->top_rank < 0 && param->bottom_rank >= 0){
      istart = 1;
      iend = sizey - 3;
    }
    else if(param->bottom_rank < 0 && param->top_rank >= 0){
      istart = 2;
      iend = sizey - 2;
    }
  }
}

//printf("Inner Jacobi: rank = %d, istart = %d, iend = %d, jstart = %d, jend = %d\n", param->my_rank, istart, iend, jstart, jend);
// printf("istart = %d, iend = %d, jstart = %d, jend = %d\n", istart, iend, jstart, jend);
  for( i=istart; i<= iend; i++ ) {
  	int ii=i*sizex;
  	int iim1=(i-1)*sizex;
  	int iip1=(i+1)*sizex;

    for( j=jstart; j<=jend; j++ ){
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

  int istart = 1, iend = sizey - 2;
  int jstart = 1, jend = sizex - 2;

  bool run = 0;

  //If no left rank exists
  if(param->left_rank < 0)
  {
    
    if(param->right_rank >= 0){
      run = 1;
      jstart = sizex-2;
      jend = jstart;
    
      if (param->top_rank >= 0){
        istart = 1; 
        iend = sizey-2;
      }
      if (param->bottom_rank >= 0){
        istart = sizey-2;
        iend = istart;
      }
    }
    else{
      if (param->top_rank >= 0){
        istart = 1;
        iend = istart;
        jstart = 1; 
        jend = sizex-2;
        run = 1;
      }
    
      if (param->bottom_rank >= 0){
        istart = sizey-2;
        iend = istart;
        jstart = 1; 
        jend = sizex-2;
        run = 1;
      }
    }

    if(run){
      for( i=istart; i<= iend; i++ ) {
      int ii=i*sizex;
      int iim1=(i-1)*sizex;
      int iip1=(i+1)*sizex;

      for( j=jstart; j<=jend; j++ ){
        utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
                          u[ ii+(j+1) ]+
                          u[ iim1+j ]+
                          u[ iip1+j ]);
          diff = utmp[ii + j] - u[ii + j];
          sum += diff * diff;
        }
      }
    }
  }

  // If there's a left rank
  else if(param->left_rank >= 0)
  {
    
    if(param->right_rank < 0){
      run = 1;
      jstart = 1;
      jend = jstart;
    
      if (param->top_rank >= 0){
        istart = 1; 
        iend = sizey-2;
      }
      if (param->bottom_rank >= 0){
        istart = sizey-2;
        iend = istart;
      }
    }
    // else{
    //   if (param->top_rank > 0){
    //     istart = 1;
    //     iend = istart;
    //     jstart = 1; 
    //     jend = sizex-1;
    //     run = true;
    //   }
    
    //   if (param->bottom_rank > 0){
    //     istart = sizey-2;
    //     iend = istart;
    //     jstart = 1; 
    //     jend = sizex-1;
    //     run = true;
    //   }
    // }

    if(run){
      for( i=istart; i<= iend; i++ ) {
      int ii=i*sizex;
      int iim1=(i-1)*sizex;
      int iip1=(i+1)*sizex;

      for( j=jstart; j<=jend; j++ ){
        utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
                          u[ ii+(j+1) ]+
                          u[ iim1+j ]+
                          u[ iip1+j ]);
          diff = utmp[ii + j] - u[ii + j];
          sum += diff * diff;
        }
      }
    }
  }
  // printf("Outer Jacobi: rank = %d, istart = %d, iend = %d, jstart = %d, jend = %d\n", param->my_rank, istart, iend, jstart, jend);
 
  // for( i=1; i< sizey-1; i++ ) {
  // 	int ii=i*sizex;
  // 	int iim1=(i-1)*sizex;
  // 	int iip1=(i+1)*sizex;
  //   j = 1;
  //   utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
  //    		            u[ ii+(j+1) ]+
  //    		            u[ iim1+j ]+
  //    		            u[ iip1+j ]);
	// 	diff = utmp[ii + j] - u[ii + j];
	// 	sum += diff * diff;
  // }
  


  // // for rightmost elements
  // // i = 1 to 20
  // // j = 20
  // for( i=1; i< sizey-1; i++ ) {
  // 	int ii=i*sizex;
  // 	int iim1=(i-1)*sizex;
  // 	int iip1=(i+1)*sizex;
  //   j = sizex-2;
  //   utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
  //    		            u[ ii+(j+1) ]+
  //    		            u[ iim1+j ]+
  //    		            u[ iip1+j ]);
	// 	diff = utmp[ii + j] - u[ii + j];
	// 	sum += diff * diff;
  // }

  // // for topmost elements
  // // i = 1
  // // j = 1 to 20
  // for( j=1; i< sizex-1; j++ ) {
  //   i = 1;
  // 	int ii=i*sizex;
  // 	int iim1=(i-1)*sizex;
  // 	int iip1=(i+1)*sizex;
  //   utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
  //    		            u[ ii+(j+1) ]+
  //    		            u[ iim1+j ]+
  //    		            u[ iip1+j ]);
	// 	diff = utmp[ii + j] - u[ii + j];
	// 	sum += diff * diff;
  // }

  // // for bottommost elements
  // // i = 20
  // // j = 1 to 20
  // for( j=1; j< sizex-1; j++ ) {
  //   i = sizey-2;
  // 	int ii=i*sizex;
  // 	int iim1=(i-1)*sizex;
  // 	int iip1=(i+1)*sizex;
  //   utmp[ ii+ j] = 0.25 * (u[ ii+(j-1) ]+
  //    		            u[ ii+(j+1) ]+
  //    		            u[ iim1+j ]+
  //    		            u[ iip1+j ]);
	// 	diff = utmp[ii + j] - u[ii + j];
	// 	sum += diff * diff;
  // }


  *u1=utmp;
  *utmp1=u;

  return(sum);
}


