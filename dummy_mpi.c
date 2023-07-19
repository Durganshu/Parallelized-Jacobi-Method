#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100
#define MAX_ITER 100

int main(int argc, char *argv[]) {
    int rank, size;
    int i, j, iter;
    int top_rank, bottom_rank, left_rank, right_rank;
    double grid[N + 2][N + 2], new_grid[N + 2][N + 2];
    double diff, max_diff, local_diff, global_diff;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Create a Cartesian communicator
    int dims[2] = {6, 1};  // Dimensions of the virtual grid
    int periods[2] = {0, 0};  // Periodicity of the grid
    MPI_Comm cart_comm;
    //MPI_Dims_create(size, 2, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);

    // Determine the process coordinates in the grid
    int coords[2];
    MPI_Cart_coords(cart_comm, rank, 2, coords);
    int row = coords[0];
    int col = coords[1];

    // Determine the ranks of neighboring processes
    MPI_Cart_shift(cart_comm, 0, 1, &bottom_rank, &top_rank);
    MPI_Cart_shift(cart_comm, 1, 1, &left_rank, &right_rank);
    // MPI_Cart_shift(cart_comm, 0, 1, &left_rank, &right_rank);
    // MPI_Cart_shift(cart_comm, 1, 1, &bottom_rank, &top_rank);

    

    int extra_x = N % dims[0];
    int local_size_x = (rank < extra_x) ? (N/dims[0] + 1) : (N/dims[0]);
    int extra_y = N % dims[1];
    int local_size_y = (rank < extra_y) ? (N/dims[1] + 1) : (N/dims[1]);
    //int A[local_size];
    //int local_sum = 0;
    //int neighbour_sum = 0;

    printf(" rank = %d, local size_x = %d, local_size y = %d\n", rank, local_size_x, local_size_y);
    // Initialize grid
    // for (i = 0; i < N + 2; i++) {
    //     for (j = 0; j < N + 2; j++) {
    //         grid[i][j] = 0.0;
    //         new_grid[i][j] = 0.0;
    //     }
    // }

    // // Main Jacobi loop
    // for (iter = 0; iter < MAX_ITER; iter++) {
    //     // Exchange data with neighboring processes
    //     MPI_Sendrecv(&grid[1][1], N, MPI_DOUBLE, top_rank, 0,
    //                  &grid[N + 1][1], N, MPI_DOUBLE, bottom_rank, 0,
    //                  cart_comm, MPI_STATUS_IGNORE);
    //     MPI_Sendrecv(&grid[N][1], N, MPI_DOUBLE, bottom_rank, 0,
    //                  &grid[0][1], N, MPI_DOUBLE, top_rank, 0,
    //                  cart_comm, MPI_STATUS_IGNORE);
    //     MPI_Sendrecv(&grid[1][1], 1, MPI_DOUBLE, left_rank, 0,
    //                  &grid[1][N + 1], 1, MPI_DOUBLE, right_rank, 0,
    //                  cart_comm, MPI_STATUS_IGNORE);
    //     MPI_Sendrecv(&grid[1][N], 1, MPI_DOUBLE, right_rank, 0,
    //                  &grid[1][0], 1, MPI_DOUBLE, left_rank, 0,
    //                  cart_comm, MPI_STATUS_IGNORE);

    //     // Compute new values using Jacobi method
    //     max_diff = 0.0;
    //     for (i = 1; i <= N; i++) {
    //         for (j = 1; j <= N; j++) {
    //             new_grid[i][j] = 0.25 * (grid[i-1][j] + grid[i+1][j] +
    //                                      grid[i][j-1] + grid[i][j+1]);
    //             diff = new_grid[i][j] - grid[i][j];
    //             if (diff > max_diff)
    //                 max_diff = diff;
    //         }
    //     }

    //     // Compute the maximum difference in the local grid
    //     local_diff = max_diff;

    //     // Compute the global maximum difference
    //     MPI_Allreduce(&local_diff, &global_diff, 1, MPI_DOUBLE, MPI_MAX, cart_comm);

    //     // Update the grid with the new values
    //     for (i = 1; i <= N; i++) {
    //         for (j = 1; j <= N; j++) {
    //             grid[i][j] = new_grid[i][j];
    //         }
    //     }

    //     // Check convergence
    //     if (global_diff < 0.0001)
    //         break;
    // }

    // // Gather final grid data to process 0
    // MPI_Gather(&grid[1][1], N * N, MPI_DOUBLE, &grid[1][1], N * N, MPI_DOUBLE, 0, cart_comm);

    // // Print the final grid from process 0
    // if (rank == 0) {
    //     printf("Final grid:\n");
    //     for (i = 1; i <= N; i++) {
    //         for (j = 1; j <= N; j++) {
    //             printf("%f ", grid[i][j]);
    //         }
    //         printf("\n");
    //     }
    // }

    MPI_Finalize();

    return 0;
}
