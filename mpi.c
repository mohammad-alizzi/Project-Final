#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define SIZE 100

void encrypt(char matrix[][SIZE], int key[][SIZE], char result[][SIZE], int start_row, int end_row) {
    int i, j, k;
    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (k = 0; k < SIZE; k++) {
                result[i][j] += matrix[i][k] * key[k][j];
            }
            result[i][j] = result[i][j] % 26 + 'A'; // Convert back to a letter
        }
    }
}

void fill(int k[SIZE][SIZE]) {
    char * string = "Mohammad";
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            k[i][j] = (string[(i+j)%sizeof(string)]) % 26;
        }
    }
}

int main(int argc, char** argv){

    int rank, size, tag=1;
    double start_time, end_time, total_time = 0.0;
    int start_row, end_row, n_rows_per_process;
    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    start_time = MPI_Wtime();

    int key[SIZE][SIZE];
    fill(key);

    if (rank == 0) {
        FILE *fp = fopen("text.txt", "r");
        FILE *out = fopen("output.txt", "w");

        if (fp == NULL) {
            printf("Error opening file.\n");
            exit(1);
        }

        char result[SIZE][SIZE]; 
        char plaintext[SIZE][SIZE];
        char c; int k=0;

        int num_rows = 0;

        do {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (c != EOF) {
                        c = fgetc(fp);
                        plaintext[i][j] = c;
                    }
                }
            }
            num_rows++;

            // Send a row of plaintext to each process
            for (int dest = 1; dest < size; dest++) {
                start_row = (dest - 1) * n_rows_per_process;
                end_row = start_row + n_rows_per_process;
                if (dest == size - 1) {
                    end_row = num_rows;
                }
                MPI_Send(&start_row, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
                MPI_Send(&end_row, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
                MPI_Send(&plaintext[start_row][0], SIZE * (end_row - start_row), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
            }

            // Compute the first part of the ciphertext in the master process
            start_row = 0;
            end_row = n_rows_per_process;
            encrypt(plaintext, key, result, start_row, end_row);




    } while (c != EOF);


    end_time = MPI_Wtime(); // Record the end time
    double average_time;

    total_time = end_time - start_time; // Accumulate the time
    printf("\n total execution time= %f", total_time);
    
    if (rank == 0) {
        average_time /= (size * 20.0); 
        printf("\nAverage execution time: %f seconds\n", average_time);
    }
    
    MPI_Finalize();

}}
