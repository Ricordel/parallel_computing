EXES = naive_monothread openmp_version

CXX=g++
CXXFLAGS=-c -O2 -g -Wall -Wextra -fopenmp

LD = g++
LDFLAGS = -lgomp

main: $(EXES)

naive_monothread: utils.o naive_monothread.o
	$(LD) $(LDFLAGS) $^ -o $@


openmp_version: utils.o openmp_version.o
	$(LD) $(LDFLAGS) $^ -o $@

%.o:%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	@rm -f $(EXES)
	@rm -f *.o

.PHONY: clean
