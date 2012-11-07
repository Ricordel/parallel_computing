// A function that reads a matrix from a file
// Format of the file:
// size_of_squared_matrix
// line of size_of_squared_matrix elements
// ...
// line of size_of_squared_matrix

#include <string>

// On fait les crasses avec du double **, mais on tentera les conteneurs
// standards C++ après
int matrix_from_file(const char * filename, double ***out_matrix, unsigned int *mat_size);
