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




/// Compute the part of the sum between from (included) and to (excluded)
static long double partial_sum(uint64_t from,
                               uint64_t to,
                               uint64_t nIterations)
{
        long double step = (long double)1 / (long double)nIterations;
        long double sum = 0;

        for (uint64_t i = from; i < to; i++) {
                sum += compute_one_value(i, step);
        }

        return sum;
}


static long double compute_pi(uint64_t nThreads, uint64_t nIterations)
{
        // Each thread will do iterations_by_thread, except for the last one
        // which will do at most as much, probably less.
        uint64_t iterations_by_thread = nIterations / nThreads;
        long double step = (long double)1 / (long double)nIterations;
        long double sum = 0;

#pragma omp parallel num_threads(nThreads)
        {
                int id = omp_get_thread_num();
                uint64_t from = id * iterations_by_thread;
                uint64_t to = min((id + 1) * iterations_by_thread, nIterations);
#pragma omp atomic // Be careful, sum is shared !
                sum += partial_sum(from, to, nIterations);
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
