/*
Date: 30/07/2026
Author: David Martin
Usage:

Implement a docker slurm cluster from https://github.com/giovtorres/slurm-docker-cluster
Install and load mpich with spack:
spack install mpich
spack load mpich

Ensure a minimum of 5 CPU nodes are avaliable with
make scale-cpu-workers N=5 - note may have to run more than once to achieve desired workers
get a shell by running make shell
ensure 5 workers are available with the sinfo command on the docker cluster

allocate nodes with salloc -N5

Either copy this file's contents and paste into vim or get the file with wget
compile file with:
mpicc mpi_math.c -o mpi_math

run with srun --mpi=pmi2 -N5 -n5 ./mpi_math

results are approximate as my math is not great, however it is a proof of concept in how MPI works in a slurm cluster / HPC not a math lesson
*/

#include <mpi.h>
#include <stdio.h>
// include required libraries

int main(int argc, char **argv) // Main C with arguments
{

        MPI_Init(&argc, &argv); // Required to initialise MPI

        int rank; // Create MPI Rank var
        MPI_Comm_rank(MPI_COMM_WORLD, &rank); // shows rank of calling process in communicator

  // define base variables for MPI receive
        double e; 
        double sqrt_2;
        double golden_ratio;
        double pi;

  // first compute - Euler's Number
        if(rank == 0){

                e = 1.0;
                double factorial = 1.0;
                for (int i = 1; i < 15; i++){
                        factorial *= i;
                        e += 1.0 / factorial;
                }

          // Send result to final print process in format of
          /*
          variable buffer (for process caluclating, it is the address of the value, for reciever, it is most likely empty, but refers to a variable which has allocated memory)
          count
          data type
          destination (Rank)
          tag (identifier)
          mpi comm
          */
                MPI_Send(
                        &e,
                        1,
                        MPI_DOUBLE,
                        4,
                        100,
                        MPI_COMM_WORLD
                        );
        }

        // Second compute - square root of 2
        if(rank == 1){

                sqrt_2 = 1.0;
                for (int i = 1; i < 10; i++){
                        sqrt_2 = (sqrt_2 + 2.0 / sqrt_2) / 2.0;
                }

          // Send result to final print process
                MPI_Send(
                        &sqrt_2,
                        1,
                        MPI_DOUBLE,
                        4,
                        101,
                        MPI_COMM_WORLD
                        );
        }
        // Third compute - Golden Ratio
        if(rank == 2){

                double a = 1.0, b = 1.0, temp;
                golden_ratio = 0.0;
                for (int i = 0; i < 20; i++){

                        temp = b;
                        b = a + b;
                        a = temp;
                        golden_ratio = b / a;
                }

          // Send result to final print process
                MPI_Send(
                        &golden_ratio,
                        1,
                        MPI_DOUBLE,
                        4,
                        102,
                        MPI_COMM_WORLD
                        );
        }

        // Fourth Compute - Pi
        if(rank == 3){

                pi = 0.0;
                int sign = 1;
                for (int i = 0; i < 1000000; i++){

                        pi += sign * (4.0 / (2 * i + 1));
                        sign *= -1;
                }

          // Send result to final print process
                MPI_Send(
                        &pi,
                        1,
                        MPI_DOUBLE,
                        4,
                        103,
                        MPI_COMM_WORLD
                        );
        }

        // Fifth compute - Compiling results and printing
        if (rank == 4){

          // Receive result from Rank 0 in the form of
          /*
          Buffer - In this instance the empty variable address defined at the start, in this process it will be an empty location in memory
          Count
          Data Type
          Source Rank
          Tag Identifier
          Comm
          Status
          */
                MPI_Recv(
                        &e,
                        1,
                        MPI_DOUBLE,
                        0,
                        100,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE
                        );
          // Receive result from Rank 1
                MPI_Recv(
                        &sqrt_2,
                        1,
                        MPI_DOUBLE,
                        1,
                        101,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE
                        );

          // Receive result from Rank 2
                MPI_Recv(
                        &golden_ratio,
                        1,
                        MPI_DOUBLE,
                        2,
                        102,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE
                        );

          // Receive result from Rank 3
                MPI_Recv(
                        &pi,
                        1,
                        MPI_DOUBLE,
                        3,
                        103,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE
                        );

            // print results
                printf("\n\n Approximate Calculations: \n\n Eulers Number = %.10f\n Square root of 2 = %.10f\n Golden ratio = %.10f\n Pi = %.10f \n\n", e, sqrt_2, golden_ratio, pi);



        }

        // finalize MPI before exiting the program.
        MPI_Finalize();

        return 0;

}
