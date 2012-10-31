#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdint>

#define _USE_MATH_DEFINES
#include <cmath>

#include <omp.h>



int main(int argc, const char *argv[])
{
        double start_time = omp_get_wtime();

        uint64_t nIterations = 10000000;
        if (argc == 2) {
                std::istringstream iss(argv[1]);
                iss >> nIterations;
                if (iss.fail()) {
                        std::cout << "Failed to parse number of iterations" << std::endl;
                        exit(1);
                }
        }


        long double step = (long double)1 / (long double)nIterations;
        long double pi = 0;
        long double sum = 0;

        for (uint64_t i = 0; i < nIterations; i++) {
                double x = (i + 0.5) * step;
                sum += 4 / (1 + x * x);
        }
        
        pi = sum * step;

        double end_time = omp_get_wtime();

        //std::cout << "After " << nIterations << " steps (in " << end_time - start_time << " sec),"
                  //<< "\n\t my pi   = " << std::setprecision(18) << pi
                  //<< "\n\t real pi = " << std::setprecision(18) << M_PI << std::endl;
        
        std::cout << "{ \"nIterations\": " << nIterations
                  << ", \"time\": " << end_time - start_time
                  << ", \"pi\": " << std::setprecision(18) << pi << "}" << std::endl;
        return 0;
}
