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
	param.global_visres = 100;

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
	int coords_baju[2];
    MPI_Cart_coords(cart_comm, rank, 2, coords);
    param.x_rnk = coords[1];
    param.y_rnk = coords[0];

	MPI_Cart_shift(cart_comm, 0, -1, &(param.bottom_rank), &(param.top_rank));
    MPI_Cart_shift(cart_comm, 1, 1, &(param.left_rank), &(param.right_rank));

	// check input file
	if (!(infile = fopen(argv[1], "r"))) {
		fprintf(stderr, "\nError: Cannot open \"%s\" for reading.\n\n", argv[1]);

		usage(argv[0]);
		return 1;
	}

	// check result file
	resfilename = "heat.ppm";

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
	time = (double *) calloc(sizeof(double), (int) (param.max_res - param.initial_res + param.res_step_size) / param.res_step_size);
	int exp_number = 0;

	//Calculating local visualization
	int vis_xstart = (coords[1] *param.global_visres)/dims[1];
	int vis_xend = ((coords[1] + 1) *param.global_visres)/ dims[1]+1;
	int vis_ystart = (coords[0] *param.global_visres)/dims[0];
	int vis_yend = ((coords[0] + 1) * param.global_visres)/dims[0]+1;

	param.local_visres_x = vis_xend - vis_xstart - 1;
	param.local_visres_y = vis_yend - vis_ystart - 1;

	for (param.global_res = param.initial_res; param.global_res <= param.max_res; param.global_res = param.global_res + param.res_step_size) {
		
		param.x_start = (coords[1] *param.global_res)/dims[1];
		param.x_end = ((coords[1] + 1) *param.global_res)/ dims[1]+1;
		param.y_start = (coords[0] *param.global_res)/dims[0];
		param.y_end = ((coords[0] + 1) * param.global_res)/dims[0]+1;

		
		param.act_res_x = param.x_end - param.x_start - 1;
		param.act_res_y = param.y_end - param.y_start - 1;
		
		if (!initialize(&param)) {
			fprintf(stderr, "Error in Jacobi initialization.\n\n");

			usage(argv[0]);
		}
	
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
		// printf("rank = %d, np_x = %d, np_y = %d\n", rank, np_x, np_y);
		double buffer_send_right_col[np_y]; //double buffer_send_row[np_x];
		double buffer_send_left_col[np_y]; //double buffer_send_row[np_x];
		double buffer_recv_right_col[np_y]; //double buffer_recv_row[np_x];
		double buffer_recv_left_col[np_y];
		for (iter = 0; iter < param.maxiter; iter++) {
			MPI_Status request_status;
			MPI_Request request_left_out, request_left_in;
  			MPI_Request request_right_out, request_right_in;
			MPI_Request request_top_out, request_top_in;
			MPI_Request request_bottom_out, request_bottom_in;

			// local_residual = relax_jacobi(&(param.u), &(param.uhelp), np_x, np_y);
			
			// printf("rank = %d, local_res =%lf\n", rank, local_residual);
			//send right
			for(i=0; i< np_y; i++) { buffer_send_right_col[i] = param.u[np_x*i + np_x - 2];} 
			
			// MPI_Sendrecv(&buffer_send_col, np_y, MPI_DOUBLE, param.right_rank, 99, &buffer_recv_col, np_y, MPI_DOUBLE, param.left_rank, 99, cart_comm, MPI_STATUS_IGNORE); 
			// Sending operation
			MPI_Isend(&buffer_send_right_col, np_y, MPI_DOUBLE, param.right_rank, 99, cart_comm, &request_right_out);

			// Receiving operation
			MPI_Irecv(&buffer_recv_left_col, np_y, MPI_DOUBLE, param.left_rank, 99, cart_comm, &request_right_in);

			// printf("done right\n");

			//send left
			for(i=0; i< np_y; i++) { buffer_send_left_col[i] = param.u[np_x*i + 1];}  
			//MPI_Sendrecv(&buffer_send_col, np_y, MPI_DOUBLE, param.left_rank, 99, &buffer_recv_col, np_y, MPI_DOUBLE, param.right_rank, 99, cart_comm, MPI_STATUS_IGNORE);
			
			// Sending operation
			MPI_Isend(&buffer_send_left_col, np_y, MPI_DOUBLE, param.left_rank, 99, cart_comm, &request_left_out);

			// Receiving operation
			MPI_Irecv(&buffer_recv_right_col, np_y, MPI_DOUBLE, param.right_rank, 99, cart_comm, &request_left_in);

			// // //send top
			// MPI_Sendrecv(&param.u[np_x], np_x, MPI_DOUBLE, param.top_rank, 99, &param.u[np_x*(np_y - 1)], np_x, MPI_DOUBLE, param.bottom_rank, 99, cart_comm, MPI_STATUS_IGNORE);

			// Sending the top data
			MPI_Isend(&param.u[np_x], np_x, MPI_DOUBLE, param.top_rank, 99, cart_comm, &request_top_out);

			// Receiving the bottom data
			MPI_Irecv(&param.u[np_x*(np_y - 1)], np_x, MPI_DOUBLE, param.bottom_rank, 99, cart_comm, &request_top_in);

			// // //send bottom
			// MPI_Sendrecv(&param.u[np_x*(np_y - 2)], np_x, MPI_DOUBLE, param.bottom_rank, 99, &param.u[0], np_x, MPI_DOUBLE, param.top_rank, 99, cart_comm, MPI_STATUS_IGNORE);

			// Sending the bottom data
			MPI_Isend(&param.u[np_x*(np_y - 2)], np_x, MPI_DOUBLE, param.bottom_rank, 99, cart_comm, &request_bottom_out);

			// Receiving the top data
			MPI_Irecv(&param.u[0], np_x, MPI_DOUBLE, param.top_rank, 99, cart_comm, &request_bottom_in);
			local_residual = inner_relax_jacobi(&param, &(param.u), &(param.uhelp), np_x, np_y);
			
			MPI_Wait(&request_right_out, &request_status);
      		MPI_Wait(&request_right_in, &request_status);			

			MPI_Wait(&request_left_out, &request_status);
			MPI_Wait(&request_left_in, &request_status);
			// printf("done left\n");

			MPI_Wait(&request_top_out, &request_status);
			MPI_Wait(&request_top_in, &request_status);

			MPI_Wait(&request_bottom_out, &request_status);
			MPI_Wait(&request_bottom_in, &request_status);

			if (param.right_rank >= 0){
				for(i=0; i< np_y; i++) {
					param.u[np_x*i + np_x - 1] = buffer_recv_right_col[i];
				} 	
			}
			if (param.left_rank >= 0){
				for(i=0; i< np_y; i++) { 
					param.u[np_x*i] = buffer_recv_left_col[i];
				}  
			}
			local_residual += outer_relax_jacobi(&param, &(param.u), &(param.uhelp), np_x, np_y);

			MPI_Allreduce(&local_residual, &global_residual, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			// if(rank == 1)
			// {
			// 	//  printf(" New_values: \n");
			// 	for(int i=0; i < np_y; i++){
			// 		for(int j=0; j < np_x;j++){
			// 		printf("%lf ", param.u[i*np_x + j]);
			// 		}
			// 		printf("\n");
			// 	}
			// }		
		}

		time[exp_number] = wtime() - time[exp_number];

		if (param.my_rank == 0){
			
			printf("\n\nResolution: %u\n", param.global_res);
			printf("===================\n");
			printf("Execution time: %f\n", time[exp_number]);
			printf("Residual: %f\n\n", global_residual);

			printf("megaflops:  %.1lf\n", (double) param.maxiter * (np_x - 2) * (np_y - 2) * 7 / time[exp_number] / 1000000);
			printf("  flop instructions (M):  %.3lf\n", (double) param.maxiter * (np_x - 2) * (np_y - 2) * 7 / 1000000);
			
		}
		

		exp_number++;
	}

	coarsen(param.u, param.act_res_x + 2, param.act_res_y + 2, param.uvis, param.local_visres_x + 2, param.local_visres_y + 2);
	
	double* global_uvis;
	global_uvis = (double*)malloc( sizeof(double)* (param.global_visres+2)*(param.global_visres+2));
	if(rank == 0){	
		int coordsx[size];
		int coordsy[size];
		int vis_xstart_ranks[size], vis_xend_ranks[size], vis_ystart_ranks[size], vis_yend_ranks[size], root_visres_x[size], root_visres_y[size];	
		
		int l,m;

		l = 0;
		for(int j = vis_ystart; j <= vis_yend; j++){	
			m = 0;
			for(int k = vis_xstart; k <= vis_xend; k++){
				global_uvis[j*(param.global_visres+2) + k] = param.uvis[l*(param.local_visres_x+2) + m];
				m++;
			}
			l++;
		}
		for(int i =1;i < size; i++){	
			MPI_Cart_coords(cart_comm, i, 2, coords);
			coordsx[i] = coords[1];
			coordsy[i] = coords[0];
			vis_xstart_ranks[i] = (coordsx[i] *param.global_visres)/dims[1];
			vis_xend_ranks[i] = ((coordsx[i] + 1) *param.global_visres)/ dims[1]+1;
			vis_ystart_ranks[i] = (coordsy[i] *param.global_visres)/dims[0];
			vis_yend_ranks[i] = ((coordsy[i] + 1) * param.global_visres)/dims[0]+1;

			root_visres_x[i] = vis_xend_ranks[i] - vis_xstart_ranks[i] + 1;
			root_visres_y[i] = vis_yend_ranks[i] - vis_ystart_ranks[i] + 1;
			
			if(size>1){
				double buffer[root_visres_x[i]*root_visres_y[i]];
				MPI_Recv(&buffer, root_visres_x[i]*root_visres_y[i], MPI_DOUBLE, i, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
				l = 0;
				for(int j = vis_ystart_ranks[i]; j <= vis_yend_ranks[i]; j++)
				{	
					m = 0;
					for(int k = vis_xstart_ranks[i]; k <= vis_xend_ranks[i]; k++){
						global_uvis[j*(param.global_visres+2) + k] = buffer[l*root_visres_x[i] + m];
						m++;
					}
					l++;
				}
				
			}
		}		
	}
	else{
		MPI_Send(param.uvis, (param.local_visres_x+2)*(param.local_visres_y+2), MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
		free(global_uvis);
	}

	if (rank == 0){

		write_image(resfile, global_uvis, param.global_visres+2, param.global_visres+2);
		free(global_uvis);
	}


	finalize(&param);
	MPI_Finalize();
	return 0;
}
