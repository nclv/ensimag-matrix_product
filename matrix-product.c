#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANGE 100.0

/* Fonctions d'allocation et de libération de la mémoire */

double** allocate_matrix(long unsigned int n) {
    double** matrix = malloc(n * sizeof(double*));
    size_t i;

    for (i = 0; i < n; ++i) {
        matrix[i] = malloc(n * sizeof(double));
    }
    return matrix;
}


/*
 * This way you get data locality and its associated cache/memory access benefits, 
 * and you can treat the array as a double ** or a flattened 2D array (array[i * NumCols + j]) 
 * interchangeably. You also have fewer calloc/free calls (2 versus NumRows + 1).
*/
double** allocate_matrix_continuous(long unsigned int n) {
    double** matrix;
    size_t i;

    matrix = (double**)malloc(n * sizeof(double*));
    matrix[0] = (double*)calloc(n * n, sizeof(double));
    // single contiguous memory allocation for entire array
    for (i = 1; i < n; ++i) {
        matrix[i] = matrix[i - 1] + n;
    }
    return matrix;
}

void free_matrix(double** matrix, long unsigned int n) {
    for (size_t i = 0; i < n; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

void free_matrix_continuous(double** matrix, long unsigned int n) {
    (void)n;
    free(matrix[0]);
    free(matrix);
}

void random_matrix2d(double** matrix, long unsigned int n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            matrix[i][j] = RANGE * ((double)rand() / RAND_MAX);
        }
    }
}

void random_matrix1d(double* array, long unsigned int n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            array[i * n + j] = RANGE * ((double)rand() / RAND_MAX);
        }
    }
}

void display_matrix(double** matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

/* Matrix multiplication functions */

void matrix2d_product_ijk(double** A, double** B, double** C, long unsigned int n) {
    size_t i, j, k;
    double sum;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            sum = 0;
            for (k = 0; k < n; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void matrix2d_product_ikj(double** A, double** B, double** C, long unsigned int n) {
    size_t i, j, k;
    double r;
    for (i = 0; i < n; ++i) {
        for (k = 0; k < n; ++k) {
            r = A[i][k];
            for (j = 0; j < n; ++j) {
                C[i][j] += r * B[k][j];
            }
        }
    }
}

void matrix1d_product_ijk(double* A, double* B, double* C, long unsigned int n) {
    size_t i, j, k;
    double* ptC = C; // the value of C is equivalent to &(C[0])
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j, ++ptC) {
            double* ptB = &B[0 * n + j];
            double* ptA = &A[i * n + 0];
            for (k = 0; k < n; ++k, ++ptA, ptB += n) {
                *ptC += *ptA * *ptB;
            }
        }
    }
}

void matrix1d_product_ikj(double* A, double* B, double* C, long unsigned int n) {
    size_t i, j, k;
    double* ptA = A;
    for (i = 0; i < n; ++i) {
        for (k = 0; k < n; ++k, ++ptA) {
            double* ptB = &B[k * n + 0];
            double* ptC = &C[i * n + 0];
            for (j = 0; j < n; ++j, ++ptC, ++ptB) {
                *ptC += *ptA * *ptB;
            }
        }
    }
}

void matrix2d_functions_execution_time(double** A, double** B, double** C,
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

void matrix1d_functions_execution_time(double* A, double* B, double* C,
                                       long unsigned int n, const char* function_name,
                                       void (*matrix_product)(double*, double*, double*, long unsigned int)) {
    static clock_t start, end;
    static double cpu_time_used;

    start = clock();
    (*matrix_product)(A, B, C, n);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("%s took %f seconds to execute for an entry n = %ld\n", function_name, cpu_time_used, n);
}

void time_n_2darray(long unsigned int n,
                    double** (*allocate_matrix_function)(long unsigned int),
                    void (*free_matrix_function)(double**, long unsigned int)) {
    double** A = allocate_matrix_function(n);
    double** B = allocate_matrix_function(n);
    random_matrix2d(A, n);
    random_matrix2d(B, n);

    double** C = allocate_matrix_function(n);
    // double (*C)[n] = malloc(sizeof(double[n][n]));
    // free(C);

    const char* functions[2] = {"matrix2d_product_ijk()", "matrix2d_product_ikj()"};

    // matrix2d_functions_execution_time(A, B, C, n, functions[0], matrix2d_product_ijk);
    matrix2d_functions_execution_time(A, B, C, n, functions[1], matrix2d_product_ikj);

    free_matrix_function(A, n);
    free_matrix_function(B, n);
    free_matrix_function(C, n);
}

void time_n_1darray(long unsigned int n) {
    double* A;  // the type is a pointer to an int (the bucket type)
    double* B;
    A = (double*)malloc(sizeof(double) * n * n);
    B = (double*)malloc(sizeof(double) * n * n);
    random_matrix1d(A, n);
    random_matrix1d(B, n);

    double* C;
    C = (double*)malloc(sizeof(double) * n * n);

    const char* functions[2] = {"matrix1d_product_ijk()", "matrix1d_product_ikj()"};

    matrix1d_functions_execution_time(A, B, C, n, functions[0], matrix1d_product_ijk);
    matrix1d_functions_execution_time(A, B, C, n, functions[1], matrix1d_product_ikj);

    free(A);
    free(B);
    free(C);
}

void test(void) {
    for (size_t i = 100; i <= 800; i += 100) {
        time_n_2darray(i, allocate_matrix, free_matrix);
        printf("\n");
    }
}

void test_allocation(void) {
    for (size_t i = 1000; i <= 1000; i += 1000) {
        time_n_2darray(i, allocate_matrix, free_matrix);
        // time_n_2darray(i, allocate_matrix_continuous, free_matrix_continuous);
        // time_n_1darray(i);
        printf("\n");
    }
}

int main(void) {
    // To gather from command line
    // size_t n = (argc == 2 ) ? atoi(argv[1]) : 100;

    srand((unsigned int)time(NULL));

    // test();

    test_allocation();

    return EXIT_SUCCESS;
}
