#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANGE 100.0

double** random_matrix(int n) {
    double** matrix = malloc(n * sizeof(double*));
    for (int i = 0; i < n; ++i) {
        matrix[i] = malloc(n * sizeof(double));
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = RANGE * ((double)rand() / RAND_MAX);
        }
    }
    return matrix;
}

void display_matrix(double** matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrix_product_1(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
        //printf("%d\n", i + 1);
    }
}

void matrix_product_2(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            for (int j = 0; j < n; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
        //printf("%d\n", i + 1);
    }
}

void time_n(int n) {
	clock_t start, end;
	double cpu_time_used;
	double** A = random_matrix(n);
	double** B = random_matrix(n);
	
	double** C = malloc(n * sizeof(double*));
	for (int i = 0; i < n; ++i) {
		C[i] = malloc(n * sizeof(double));
	}	

	start = clock();
	matrix_product_1(A, B, C, n);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("matrix_product_1() took %f seconds to execute for an entry n = %d\n", cpu_time_used, n);
	
	start = clock();
	matrix_product_2(A, B, C, n);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("matrix_product_2() took %f seconds to execute for an entry n = %d\n", cpu_time_used, n);
}

void test() {
	for (int i = 100; i <= 1000; i += 100) {
		time_n(i);
		printf("\n");
	}
}	

int main() {
    srand(time(NULL));

	test();

    return EXIT_SUCCESS;
}
