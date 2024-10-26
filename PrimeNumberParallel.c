#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define TERMINATOR -1

int main(int argc, char** argv) {
    int rank, size, n;

    // Initializing MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Start timing
    double start_time = MPI_Wtime();

    if (rank == 0) {
        // Get the upper limit n from command line
        if (argc != 2) {
            printf("Usage: mpirun -np <processes> %s <n>\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        n = atoi(argv[1]);

        // Send first prime number (2) to first process
        int first = 2;
        MPI_Send(&first, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // Generate and send all numbers from 3 to n and skip even numbers since all even numbers are non-prime numbers
        for (int num = 3; num <= n; num += 2) { 
            MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }

        // Send terminator in order to specify that numbers are done
        int terminator = TERMINATOR;
        MPI_Send(&terminator, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    // Processes in between look for the prime numbers and send them to their neighbor processes 
    else if (rank < size - 1) {
        int prime, number;
        MPI_Status status;

        // Receive the prime number this process will use for filtering
        MPI_Recv(&prime, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);

        // Process numbers while the processes are still receiving valid numbers
        while (1) {
            MPI_Recv(&number, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
            
            if (number == TERMINATOR) {
                // Forward terminator to next process
                MPI_Send(&number, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
                // Break the while(true) loop since the processes received the terminator
                break;
            }

            // If the number is not divisible by prime, pass it to next process
            if (number % prime != 0) {
                MPI_Send(&number, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
            }
        }
    }
    // Last process collects and prints the prime numbers
    else {
        int number;
        MPI_Status status;

        printf("Prime numbers found:\n");
        // First prime (2) is handled separately since we skip even numbers
        printf("2\n");

        while (1) {
            MPI_Recv(&number, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
            
            if (number == TERMINATOR) {
                break;
            }
            
            // If a number reaches the last process, it's prime
            printf("%d\n", number);
        }
    }

    // End timing
    double end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Execution time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}