#include <fstream>
#include <iostream>


int matrix_from_file(const char * filename, double ***out_matrix, unsigned int *mat_size)
{
        unsigned int size;
        std::ifstream mat_file;
        double ** matrix = NULL;

        mat_file.open(filename);
        if (!mat_file.good()) {
                std::cerr << "There was an error while opening the file" << std::endl;
                goto err;
        }


        mat_file >> size;
        if (mat_file.fail()) {
                std::cerr << "Error while reading size of matrix" << std::endl;
                goto err;
        }
        *mat_size = size;


        // Allocate the matrix. Ugly C style, we'll try with containers later to compare performaces.
        try {
                matrix = new double*[size];
                for (unsigned int i = 0; i < size; i++) {
                        matrix[i] = new double[size];
                }
        } catch (std::bad_alloc&) {
                std::cerr << "No more memory" << std::endl;
                throw;
        }

        for (unsigned int i = 0; i < size; i++) {
                for (unsigned int j = 0; j < size; j++) {
                        double val;
                        mat_file >> val;
                        if (mat_file.fail()) {
                                std::cerr << "Failed to parse double at line " << i << ", col " << j << std::endl;
                                goto err;
                        }
                        matrix[i][j] = val;
                }
        }

        *out_matrix = matrix;
        return 0;

err:
        if (mat_file.is_open()) {
                mat_file.close();
        }
        if (matrix) {
                for (unsigned int i = 0; i < size; i++) {
                        delete [] matrix[i];
                }
                delete[] matrix;
        }

        return -1;
}

