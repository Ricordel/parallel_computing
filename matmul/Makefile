EXE = naive_monothread
CXXFILES = $(wildcard *.cpp)
OFILES = $(patsubst %.cpp, %.o, $(CXXFILES))
LD = g++
CXXFLAGS=-O0 -g

main:$(EXE)

$(EXE): $(OFILES)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	@rm -f $(EXE)
	@rm -f *.o

.PHONY: clean
