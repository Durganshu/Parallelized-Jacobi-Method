#include <stdio.h>
#include <mpi.h>

#define MASTER 0
#define N 1000

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
    int extra = N % size;
    int local_size = (rank < extra) ? (N/size + 1) : (N/size);
    int A[local_size];
    int local_sum = 0;
    int neighbour_sum = 0;
    for (unsigned int i = 0; i < local_size; i++){
        A[i] = (rank < extra) ? (rank * (N/size + 1) + i) : (rank * (N/size) + i + extra);
        local_sum += A[i];
    }
    
    int i = 0;
    int temp = size;
    
    while (temp > 1){
        
        temp = temp >> 1;

        if (rank % (1 << (i + 1)) == 0){
                MPI_Recv(&neighbour_sum, 1, MPI_INT, rank + (1 << i), 999, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_sum += neighbour_sum;            
        }
        else if(rank % (1 << (i + 1)) ==  (1 << i)){
                MPI_Send(&local_sum, 1, MPI_INT, rank - (1 << i), 999, MPI_COMM_WORLD);
        }
        i++;
    }
    
    if (rank == 0){
        printf("%d \n", local_sum);
    }
        
    MPI_Finalize();
}