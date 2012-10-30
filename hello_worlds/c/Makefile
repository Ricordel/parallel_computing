CC = gcc
CFLAGS = -pthread -Wall -Wextra -g -std=c99

EXE = hello_world

all: $(EXE)

run: $(EXE)
	./$(EXE)

debug: $(EXE)
	gdb ./$(EXE)

clean:
	@rm -f $(EXE)
	@rm -f *.o

.PHONY: clean run debug
