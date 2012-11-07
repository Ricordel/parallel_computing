#include "utils.hpp"
#include <iostream>
#include <cstdlib>
#include <omp.h>


#define handle_ret(ret) if (ret < 0) goto err


#define free_mat(mat, size) \
{\
        if (mat != NULL) { \
                for (unsigned int i = 0; i < size; i++) { \
                        delete [] mat[i]; \
                } \
                delete [] mat; \
        } \
} \


template<typename T>
static inline T abs(T n) {
        if (n > 0) {
                return n;
        } else {
                return -n;
        }
}


void dump_mat(double **mat, unsigned int size)
{
        for (unsigned int i = 0; i < size; i++) {
                for (unsigned int j = 0; j < size; j++) {
                        std::cout << mat[i][j] << " ";
                }
                std::cout << std::endl;
        }
}


bool mat_equal(double ** a, double ** b, unsigned int size)
{
        // We are dealing with doubles, to let's take equality to 10-5
        for (unsigned int i = 0; i < size; i++) {
                for (unsigned int j = 0; j < size; j++) {
                        if (abs(a[i][j] - b[i][j]) > 10e-5) {
                                return false;
                        }
                }
        }

        return true;
}



int mat_mult(double ** mat1,
             double ** mat2,
             unsigned int size,
             double *** mat_rslt)
{
        double **rslt = NULL;

        try {
                rslt = new double*[size];
                for (unsigned int i = 0; i < size; i++) {
                        rslt[i] = new double[size];
                }
        } catch (std::bad_alloc&) {
                std::cerr << "No more memory" << std::endl;
                throw;
        }


        for (unsigned int i = 0; i < size; i++) {
                for (unsigned int j = 0; j < size; j++) {
                        double acc = 0;
#pragma omp parallel for reduction(+:acc)
                        for (unsigned int k = 0; k < size; k++) {
                                acc += mat1[i][k] * mat2[k][j];
                        }
                        rslt[i][j] = acc;
                }
        }


        *mat_rslt = rslt;
        return 0;
}



int main(int argc, char *argv[])
{
        int ret;
        double start_time, end_time;

        if (argc != 4) {
                std::cout << "Usage: " << argv[0]
                          << " mat1_filename mat2_filename awaited_result" 
                          << std::endl;
                exit(1);
        }
        
        double **mat1 = NULL;
        double **mat2 = NULL;
        double **correct_rslt = NULL;
        double ** mat_rslt = NULL;
        unsigned int size_1, size_2, size_correct_rslt;

        ret = matrix_from_file(argv[1], &mat1, &size_1);
        handle_ret(ret);

        ret = matrix_from_file(argv[2], &mat2, &size_2);
        handle_ret(ret);

        ret = matrix_from_file(argv[3], &correct_rslt, &size_correct_rslt);
        handle_ret(ret);

        if (size_1 != size_correct_rslt) {
                std::cerr << "Both matrices must have the same size." << std::endl;
                std::cerr << "From " << argv[1] << ": " << size_1 << std::endl;
                std::cerr << "From " << argv[2] << ": " << size_2 << std::endl;
                goto err;
        }

        if (size_1 != size_correct_rslt) {
                std::cerr << "Result matrix must be the same size" << std::endl;
                std::cerr << "From " << argv[3] << ": " << size_correct_rslt << std::endl;
                goto err;
        }
                        
        start_time = omp_get_wtime();
        ret = mat_mult(mat1, mat2, size_1, &mat_rslt);
        end_time = omp_get_wtime();

        handle_ret(ret);
                
        if (! mat_equal(mat_rslt, correct_rslt, size_1)) {
                std::cerr << "Bad multiplication result" << std::endl;
                std::cerr << "Correct result: " << std::endl;
                dump_mat(correct_rslt, size_1);
                std::cerr << std::endl << "Actual result: " << std::endl;
                dump_mat(mat_rslt, size_1);
        } else {
                std::cout << "{\"time\": " << end_time - start_time << "}" << std::endl;
        }

        return 0;

err:
        free_mat(mat1, size_1);
        free_mat(mat2, size_2);
        free_mat(correct_rslt, size_correct_rslt);
        free_mat(mat_rslt, size_1);

        return -1;
}
