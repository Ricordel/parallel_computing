#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdint>

#define _USE_MATH_DEFINES
#include <cmath>




int main(int argc, const char *argv[])
{
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
        std::cout << "After " << nIterations << " steps,"
                  << "\n\t my pi   = " << std::setprecision(18) << pi
                  << "\n\t real pi = " << std::setprecision(18) << M_PI << std::endl;
        
        return 0;
}
