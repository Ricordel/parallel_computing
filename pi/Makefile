#CXX = /opt/gnu/gcc-cilk/bin/g++
CXX = g++
CXXFLAGS = -std=c++11 -pthread -g -O2 -I../utils/ -Wall -Wextra


EXE_seq = pi_seq
EXE_par = pi_parallel

all: $(EXE_seq) $(EXE_par)

clean:
	@rm -f *.o
	@rm -f $(EXE_seq)
	@rm -f $(EXE_par)


.PHONY: clean
