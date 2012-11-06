CXX = g++
CXXFLAGS = -std=c++11 -pthread -g -O2 -I../../utils/ -Wall -Wextra -fopenmp

EXE = pi_omp 

all: $(EXE)

run: $(EXE)
	./$(EXE)

clean:
	@rm -f *.o
	@rm -f $(EXE)


.PHONY: clean run
