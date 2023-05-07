#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define SIZE 3

// Fill the key matrix with ASCII values of "Mohammad" string
void fill_key(int* key) {
    char* string = "Mohammad";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            key[i * SIZE + j] = string[(i + j) % strlen(string)];
        }
    }
}

_global_ void encrypt(char* input, int* key, char* output, int n) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    while (tid < n) {
        int i = tid / SIZE;
        int j = tid % SIZE;
        int sum = 0;
        for (int k = 0; k < SIZE; k++) {
            sum += input[i * SIZE + k] * key[k * SIZE + j];
        }
        output[tid] = sum % 26 + 'A'; // Convert back to a letter
        tid += blockDim.x * gridDim.x;
    }
}

int main() {

    FILE *fp = fopen("input.txt", "r");
    FILE *out = fopen("output.txt", "w");

    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Read the input file
    fseek(fp, 0L, SEEK_END);
    int n = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    char* input = (char*)malloc(n * sizeof(char));
    fread(input, sizeof(char), n, fp);

    // Allocate memory on device
    char *d_input, *d_output;
    cudaMalloc((void**)&d_input, n * sizeof(char));
    cudaMalloc((void**)&d_output, n * sizeof(char));
    int *d_key;
    cudaMalloc((void**)&d_key, SIZE * SIZE * sizeof(int));

    // Fill the key matrix with ASCII values of "Mohammad" string
    int* key = (int*)malloc(SIZE * SIZE * sizeof(int));
    fill_key(key);

    // Copy input and key to device
    cudaMemcpy(d_input, input, n * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_key, key, SIZE * SIZE * sizeof(int), cudaMemcpyHostToDevice);

    // Encrypt the message on device
    clock_t start = clock();
    encrypt<<<1024, 1024>>>(d_input, d_key, d_output, n);
    cudaDeviceSynchronize();
    clock_t end = clock();

    // Copy output from device to host
    char* output = (char*)malloc(n * sizeof(char));
    cudaMemcpy(output, d_output, n * sizeof(char), cudaMemcpyDeviceToHost);

    // Write the output to file
    fwrite(output, sizeof(char), n, out);

    // Print execution time
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", time_taken);

    // Free memory
    free(input);
    free(output);
    free(key);
    cudaFree(d_input);
    cudaFree(d_output);
    cudaFree(d_key);
    fclose(fp);
    fclose(out);
}
