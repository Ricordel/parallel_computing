#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <future>
#include <vector>
#include <cstdint>

#define _USE_MATH_DEFINES /* defines non standard macros like M_PI */
#include <cmath>

#include "my_utils.h"



static inline void usage(const char *progName)
{
        std::cout << "Usage: " << progName << "[num iterations] [num threads]" << std::endl;
}


/// Compute the part of the sum between from (included) and to (excluded)
static long double partial_sum(uint64_t from,
                               uint64_t to,
                               uint64_t nIterations)
{
        long double step = (long double)1 / (long double)nIterations;
        long double sum = 0;

        for (uint64_t i = from; i < to; i++) {
                long double x = (i + 0.5) * step;
                sum += 4 / (1 + x * x);
        }

        return sum;
}


static long double compute_pi(uint64_t nThreads, uint64_t nIterations)
{
        std::vector<std::future<long double>> partialSums_fut;

        // Each thread will to iterations_by_thread, except for the last one
        // which will do at most as much, probably less.
        uint64_t iterations_by_thread = nIterations / (nThreads - 1);
        long double step = (long double)1 / (long double)nIterations;
        long double sum = 0;

        uint64_t from = 0;
        uint64_t to = 0;

        // Create the anynchronous tasks, force them on different threads (it's not
        // automatically threaded although it's really faster when it is, the automatic
        // decision seems not great for the moment)
        for (uint64_t i = 0; i < nThreads; i++) {
                from = to;
                to = from + iterations_by_thread;
                if (to > nIterations)
                        to = nIterations;

                partialSums_fut.push_back(std::async(
                                          std::launch::async,
                                          partial_sum, from, to, nIterations));
        }


        // Gather the results
        for (auto& fut : partialSums_fut) {
                sum += fut.get();
        }

        return sum * step;
}



int main(int argc, const char *argv[])
{
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

        std::cout << "After " << nIterations << " steps on " << nThreads << " threads,"
                  << "\n\t my pi   = " << std::setprecision(18) << pi
                  << "\n\t real pi = " << std::setprecision(18) << M_PI << std::endl;
        
        return 0;
}
