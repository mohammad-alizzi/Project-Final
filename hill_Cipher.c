#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#define SIZE 100


void encrypt(char matrix[][SIZE], int key[][SIZE], char result[][SIZE], FILE *fp) {

    int i, j, k;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (k = 0; k < SIZE; k++) {
                result[i][j] += matrix[i][k] * key[k][j];
            }
            result[i][j] = result[i][j] % 26 + 'A'; // Convert back to a letter
            
            {
                fprintf(fp, "%c", result[i][j]);

            }
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

int main(){


	int total = 0;
	double start_time, end_time, total_time = 0.0;
        start_time = clock(); // Record the start time
	int key[SIZE][SIZE];
	fill(key);
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			printf("%d ",key[i][j]);
		}
		printf("\n");
	}
	
    FILE *fp = fopen("input5.txt", "r");
    FILE *out = fopen("output.txt", "w");

    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

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
    end_time = clock(); // Record the end time

    total_time = (end_time - start_time)/CLOCKS_PER_SEC; 
    printf("\n total execution time= %f", total_time);	
	
}
