#include <iostream>
#include <vector>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank,size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int count = 100;
    double start_time, end_time, elapsed_time;

    for(int k = 0; k <= 24; k++)
    {
        int message_size = 1;
        std::vector<char> send, recv;
        if(k == 0){
            send.push_back(0);
            recv.push_back(0);
            message_size = 1;
        }
        else{
            send.resize(2 << (k - 1), 0);
            recv.resize(2 << (k - 1), 0);
            message_size = (2 << (k - 1));
        }
        
        start_time = MPI_Wtime();
        for(int i = 0;i < count; i++)
        {
            if(rank == 0)
            {
                MPI_Send(send.data(), message_size, MPI_CHAR, 1, 999, MPI_COMM_WORLD);
                MPI_Recv(recv.data(), message_size, MPI_CHAR, 1, 999, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                MPI_Recv(recv.data(), message_size, MPI_CHAR, 0, 999, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(send.data(), message_size, MPI_CHAR, 0, 999, MPI_COMM_WORLD);
            }
        }
        end_time = MPI_Wtime();

        auto transfer_time = (end_time - start_time) / (count * 2);
        double size_in_mb = static_cast<double>(message_size) * 1 / 1048576;

        if(rank == 0)
        {
            // std::cout << "Message size (MB): " << size_in_mb << "\n";
            // std::cout << "Message size: " << message_size << "\n";
            // std::cout << "Transfer time (sec): " << transfer_time << "\n";
            // std::cout << "Bandwidth (MB/s): " << size_in_mb / transfer_time << "\n";
            std::cout << "2^" << k << "," << size_in_mb / transfer_time << "\n"; 
        }
    }
        
    MPI_Finalize();
}