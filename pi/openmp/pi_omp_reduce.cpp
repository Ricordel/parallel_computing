#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <cstdint>
#include <omp.h>
#include <thread>

#define _USE_MATH_DEFINES /* defines non standard macros like M_PI */
#include <cmath>

#include "my_utils.h"



static inline void usage(const char *progName)
{
        std::cout << "Usage: " << progName << "[num iterations] [num threads]" << std::endl;
}


static inline long double compute_one_value(uint64_t i, long double step)
{
        long double x = (i + 0.5) * step;
        return 4 / (1 + x * x);
}




static long double compute_pi(uint64_t nThreads, uint64_t nIterations)
{
        long double sum = 0;
        long double step = (long double)1 / (long double)nIterations;

//#pragma omp parallel for reduction (+:sum) num_threads(nThreads)
#pragma omp parallel for reduction (+:sum)
        for (uint64_t i = 0; i < nIterations; i++) {
                sum += compute_one_value(i, step);
        }

        return sum * step;
}




int main(int argc, const char *argv[])
{
        double start_time = omp_get_wtime();

        uint64_t nIterations = 10000000;
        uint64_t nThreads = std::thread::hardware_concurrency();

        if (argc > 3) {
                usage(argv[0]);
                exit(1);
        }

        if (argc >= 2) {
                std::istringstream iss(argv[1]);
                iss >> nIterations;
                if (iss.fail()) {
                        std::cout << "Failed to parse number of iterations" << std::endl;
                        exit(1);
                }
        }

        if (argc == 3) {
                std::istringstream iss(argv[2]);
                iss >> nThreads;
                if (iss.fail()) {
                        std::cout << "Failed to parse number of threads" << std::endl;
                        exit(1);
                }
        }

        nThreads = min(nThreads, nIterations);

        long double pi = compute_pi(nThreads, nIterations);

        double end_time = omp_get_wtime();

        std::cout << "{ \"nIterations\": " << nIterations
                  << ", \"time\": " << end_time - start_time
                  << ", \"pi\": " << std::setprecision(18) << pi << "}" << std::endl;
        
        
        return 0;
}
