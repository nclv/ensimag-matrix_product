#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANGE 100.0

double** init_matrix(long unsigned int n) {
    double** matrix = malloc(n * sizeof(double*));
    for (size_t i = 0; i < n; ++i) {
        matrix[i] = malloc(n * sizeof(double));
    }
    return matrix;
}

void free_matrix(double **matrix, long unsigned int n) {
	for (size_t i = 0; i < n; ++i) {
        free(matrix[i]);
    }
	free(matrix);
}

double** random_matrix(long unsigned int n) {
    double** matrix = init_matrix(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
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

void matrix_product_1(double** A, double** B, double** C, long unsigned int n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
        //printf("%d\n", i + 1);
    }
}

void matrix_product_2(double** A, double** B, double** C, long unsigned int n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = 0; k < n; ++k) {
            for (size_t j = 0; j < n; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
        //printf("%d\n", i + 1);
    }
}

void function_execution_time(double** A, double** B, double** C,
                             long unsigned int n, const char* function_name,
                             void (*matrix_product)(double**, double**, double**, long unsigned int)) {
    static clock_t start, end;
    static double cpu_time_used;

    start = clock();
    (*matrix_product)(A, B, C, n);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("%s took %f seconds to execute for an entry n = %ld\n", function_name, cpu_time_used, n);
}

void time_n(long unsigned int n) {
    double** A = random_matrix(n);
    double** B = random_matrix(n);

    double** C = init_matrix(n);

    const char* functions[2] = {"matrix_product_1()", "matrix_product_2()"};

    function_execution_time(A, B, C, n, functions[0], matrix_product_1);
    function_execution_time(A, B, C, n, functions[1], matrix_product_2);

	free_matrix(A, n);
	free_matrix(B, n);
	free_matrix(C, n);
}

void test(void) {
    for (size_t i = 100; i <= 1000; i += 100) {
        time_n(i);
        printf("\n");
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    test();

    return EXIT_SUCCESS;
}
