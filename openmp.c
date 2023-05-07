#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define SIZE 100


void encrypt(char matrix[][SIZE], int key[][SIZE], char result[][SIZE], FILE * fo) {
    int i, j, k;
    #pragma omp parallel for shared(matrix, key, result) private(i, j, k)
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            #pragma omp parallel for reduction(+:result[i][j])
            for (k = 0; k < SIZE; k++) {
                result[i][j] += matrix[i][k] * key[k][j];
            }
            result[i][j] = result[i][j] % 26 + 'A'; 
            fprintf(fo, "%c", result[i][j]);
        }
    }
}

void fill(int k[SIZE][SIZE]) {
    char * string = "Mohammad";
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            k[i][j] = ((int) string[(i+j)%sizeof(string)]) % 26;
        }
    }
}

int main() {
    int total = 0;
    double start_time, end_time, total_time = 0.0;
    start_time = omp_get_wtime(); 
    int key[SIZE][SIZE];
    fill(key);
    FILE *fp = fopen("input10.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        //sprintf(out_file, "output.txt", tid);
        FILE *out = fopen("open.txt", "w");
        char result[SIZE][SIZE]; 
        char plaintext[SIZE][SIZE];
	char c; int k=0;
	int i,j;


	do {
 	   for (i = 0; i < SIZE; i++) {
	        for (j = 0; j < SIZE; j++) {
	            if (c != EOF) {
	                c = fgetc(fp);
	                plaintext[i][j] = c;
	            }
	        }
	    }


    	encrypt(plaintext, key, result, out);

    	} while (c != EOF);
	
	end_time = omp_get_wtime(); 

	total_time = end_time - start_time;

	printf("Total execution time = %f seconds.\n", total_time);

	fclose(fp);
	fclose(out);
	}
}
