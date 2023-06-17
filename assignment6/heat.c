#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "heat.h"
#include "timing.h"

double* time;

void usage(char *s) {
	fprintf(stderr, "Usage: %s <input file> [result file]\n\n", s);
}

int main(int argc, char *argv[]) {
	int i, j, k, ret;
	FILE *infile, *resfile;
	char *resfilename;
	int np_x, np_y, iter, chkflag;
	
	//*************************
	int rank, size;
    // double diff, max_diff, local_diff, global_diff;
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	// algorithmic parameters
	algoparam_t param;

	// timing
	double local_residual;
	double global_residual;

	// set the visualization resolution
	param.visres = 100;

	// check arguments
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}

	param.num_process_x = atoi(argv[2]);
	param.num_process_y = atoi(argv[3]);
	param.my_rank = rank;
	 // Create a Cartesian communicator
    int dims[2] = {param.num_process_y, param.num_process_x};  // Dimensions of the virtual grid
    int periods[2] = {0, 0};  // Periodicity of the grid
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);

	// Determine the process coordinates in the grid
    int coords[2];
    MPI_Cart_coords(cart_comm, rank, 2, coords);
    param.x_rnk = coords[1];
    param.y_rnk = coords[0];
	// printf("rank = %d, param.row_rnk = %d, param.col_rnk = %d\n", rank, param.x_rnk, param.y_rnk);

	MPI_Cart_shift(cart_comm, 0, -1, &(param.bottom_rank), &(param.top_rank));
    MPI_Cart_shift(cart_comm, 1, 1, &(param.left_rank), &(param.right_rank));
	printf ("rank = %d, left_rank = %d, right_rank= %d, top_rank= %d, bottom_rank= %d\n", rank, (param.left_rank), (param.right_rank), (param.top_rank) ,(param.bottom_rank));

	// check input file
	if (!(infile = fopen(argv[1], "r"))) {
		fprintf(stderr, "\nError: Cannot open \"%s\" for reading.\n\n", argv[1]);

		usage(argv[0]);
		return 1;
	}

	// check result file
	resfilename = (argc >= 3) ? argv[2] : "heat.ppm";

	if (!(resfile = fopen(resfilename, "w"))) {
		fprintf(stderr, "\nError: Cannot open \"%s\" for writing.\n\n", resfilename);

		usage(argv[0]);
		return 1;
	}

	// check input
	if (!read_input(infile, &param)) {
		fprintf(stderr, "\nError: Error parsing input file.\n\n");

		usage(argv[0]);
		return 1;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//print_params(&param);
	// printf("hello****87, res_step = %d\n", param.res_step_size);
	time = (double *) calloc(sizeof(double), (int) (param.max_res - param.initial_res + param.res_step_size) / param.res_step_size);
	// printf("hello****89\n");
	int exp_number = 0;

	for (param.global_res = param.initial_res; param.global_res <= param.max_res; param.global_res = param.global_res + param.res_step_size) {
		//printf("hello****92\n");
		
		param.x_start = (coords[1] *param.global_res)/dims[1];
		param.x_end = ((coords[1] + 1) *param.global_res)/ dims[1]+1;
		param.y_start = (coords[0] *param.global_res)/dims[0];
		param.y_end = ((coords[0] + 1) * param.global_res)/dims[0]+1;

		param.act_res_x = param.x_end - param.x_start - 1;
		param.act_res_y = param.y_end - param.y_start - 1;
		// param.extra_x = param.global_res % dims[1];
		// param.act_res_x = (param.col_rnk < param.extra_x) ? (param.global_res/dims[1] + 1) : (param.global_res/dims[1]);

		// param.extra_y = param.global_res % dims[0];
		// param.act_res_y = (param.row_rnk  < param.extra_y) ? (param.global_res/dims[0] + 1) : (param.global_res/dims[0]);
		//printf("hello****98\n");
		// if (rank == 1){
		// printf("param.act_res_x = %d, param.act_res_y = %d\n", param.act_res_x, param.act_res_y);
		// }
		if (rank == 0){
			printf("param.x_start0 = %d, param.x_end0 = %d\n", param.x_start, param.x_end);
		}
		if (rank == 1){
			printf("param.x_start1 = %d, param.x_end1 = %d\n", param.x_start, param.x_end);
		}
		if (rank == 2){
			printf("param.x_start2 = %d, param.x_end2 = %d\n", param.x_start, param.x_end);
		}
		if (!initialize(&param)) {
			fprintf(stderr, "Error in Jacobi initialization.\n\n");

			usage(argv[0]);
		}
		// printf("rank = %d, param->numsrcs = %d\n", param.my_rank, param.numsrcs);
  		// exit(0);
		//printf("hello****104\n");
		for (i = 0; i < param.act_res_y + 2; i++) {
			for (j = 0; j < param.act_res_x + 2; j++) {
				param.uhelp[i * (param.act_res_x + 2) + j] = param.u[i * (param.act_res_x + 2) + j];
			}
		}

		// starting time
		time[exp_number] = wtime();
		local_residual = 999999999;
		np_x = param.act_res_x + 2;
		np_y = param.act_res_y + 2;

		double buffer_send_col[np_y]; //double buffer_send_row[np_x];
		double buffer_recv_col[np_y]; //double buffer_recv_row[np_x];
		for (iter = 0; iter < param.maxiter; iter++) {

			local_residual = relax_jacobi(&(param.u), &(param.uhelp), np_x, np_y);

			MPI_Allreduce(&local_residual, &global_residual, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			
			if(param.my_rank == 0){
				printf("**************rank0 before\n");
				for (i=0;i<np_y;i++){
					for (j=0;j<np_x;j++){
						printf("%f, ",param.u[i*np_x+j]);
					}
					printf("\n");
				}
			}

			if(param.my_rank == 1){
				printf("**************rank1 before\n");
				for (i=0;i<np_y;i++){
					for (j=0;j<np_x;j++){
						printf("%f, ",param.u[i*np_x+j]);
					}
					printf("\n");
				}
			}
			//send right
			for(i=0; i< np_y; i++) { buffer_send_col[i] = param.u[np_x*i + np_x - 2];} 
			MPI_Sendrecv(&buffer_send_col, np_y, MPI_DOUBLE, param.right_rank, 99, &buffer_recv_col, np_y, MPI_DOUBLE, param.left_rank, 99, cart_comm, MPI_STATUS_IGNORE);
			for(i=0; i< np_y; i++) { param.u[np_x*i] = buffer_recv_col[i];}  
			if(param.my_rank == 0){
				printf("**************rank0 after\n");
				for (i=0;i<np_y;i++){
					for (j=0;j<np_x;j++){
						printf("%f, ",param.u[i*np_x+j]);
					}
					printf("\n");
				}
			}			
			if(param.my_rank == 1){
				printf("**************rank1 after\n");
				for (i=0;i<np_y;i++){
					for (j=0;j<np_x;j++){
						printf("%f, ",param.u[i*np_x+j]);
					}
					printf("\n");
				}
			}
			// //send left
			// for(i=0; i< np_y; i++) { buffer_send_col[i] = param.u[np_x*i + 1];}  
			// MPI_Sendrecv(&buffer_send_col, np_y, MPI_DOUBLE, param.left_rank, 99, &buffer_recv_col, np_y, MPI_DOUBLE, param.right_rank, 99, cart_comm, MPI_STATUS_IGNORE);
			// for(i=0; i< np_y; i++) { param.u[np_x*i + np_x - 1] = buffer_recv_col[i];} 

			// // //send top
			// MPI_Sendrecv(&param.u[np_x], np_x, MPI_DOUBLE, param.top_rank, 99, &param.u[np_x*(np_y - 1)], np_x, MPI_DOUBLE, param.bottom_rank, 99, cart_comm, MPI_STATUS_IGNORE);

			// // //send bottom
			// MPI_Sendrecv(&param.u[np_x*(np_y - 2)], np_x, MPI_DOUBLE, param.bottom_rank, 99, &param.u[0], np_x, MPI_DOUBLE, param.top_rank, 99, cart_comm, MPI_STATUS_IGNORE);

			// if(param.my_rank == 1){

			// for (i=0;i<np_y;i++){
			// 	for (j=0;j<np_x;j++){
			// 		printf("%f, ",param.u[i*np_x+j]);
			// 	}
			// 	printf("\n");
			// }
			// }
		}

		time[exp_number] = wtime() - time[exp_number];

		//printf("\n\nrank = %d, Resolution: %ux%u\n", rank, param.act_res_x, param.act_res_y);
		// printf("===================\n");
		// printf("Execution time: %f\n", time[exp_number]);
		// printf("Residual: %f\n\n", global_residual);

		// printf("megaflops:  %.1lf\n", (double) param.maxiter * (np_x - 2) * (np_y - 2) * 7 / time[exp_number] / 1000000);
		// printf("  flop instructions (M):  %.3lf\n", (double) param.maxiter * (np_x - 2) * (np_y - 2) * 7 / 1000000);

		exp_number++;
	}

	// param.act_res = param.act_res - param.res_step_size;

	// coarsen(param.u, param.act_res + 2, param.act_res + 2, param.uvis, param.visres + 2, param.visres + 2);

	// write_image(resfile, param.uvis, param.visres + 2, param.visres + 2);

	finalize(&param);
	return 0;
}
