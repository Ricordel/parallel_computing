# Compiler options
#CC = gcc
#CXX = g++
CFLAGS = -c -O3 -Wall -Wextra -Isrc -DNDEBUG -std=c99 -pedantic $(OPTFLAGS)
CXXFLAGS = -c -O3 -Wall -Wextra -Isrc -DNDEBUG  -std=c++11 -pedantic $(OPTFLAGS)
OPTFLAGS += -fopenmp

# Superset
LD = g++
LDFLAGS = $(OPTLIBS) -pg
OPTLIBS += -lgomp

# Project name
PROJECT = qsort_seq qsort_omp qsort_cilk

# Directories
OBJDIR = bin
SRCDIR = src


# Files and folders
SRCDIRS     = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
CFILES      = $(shell find $(SRCDIR) -name '*.c')
CPPFILES    = $(shell find $(SRCDIR) -name '*.cpp')
OBJSFROMC   = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CFILES))
OBJSFROMCPP = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(CPPFILES))
OBJS        = $(OBJSFROMC) $(OBJSFROMCPP)
DEPFILES    = $(patsubst %.o, %.d, $(OBJS))

# Targets
all: depends buildrepo $(PROJECT)

# project parts:
Quicksort: $(OBJDIR)/Quicksort.o
	$(LD) $(LDFLAGS) $< -o $@

qsort_omp: $(OBJDIR)/qsort_omp.o
	$(LD) $(LDFLAGS) $< -o $@

qsort_seq: $(OBJDIR)/qsort_seq.o
	$(LD) $(LDFLAGS) $< -o $@

qsort_cilk: $(OBJDIR)/qsort_cilk.o
	$(LD) -L/opt/gnu/gcc-cilk/lib -lcilkrts -ldl $(LDFLAGS) $< -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%_cilk.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -fcilkplus $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	

# dev is all + debug options: -g, -O0, -DDEBUG
dev: CFLAGS = -O0 -g -Wall -Wextra -Isrc -std=c99 -pedantic $(OPTFLAGS)
dev: CXXLAGS = -O0 -g -Wall -Wextra -Isrc  -std=c++11 -pedantic $(OPTFLAGS)
dev: CPPFLAGS += -DDEBUG
dev: all

test: depends $(OBJS)
	cd tests; $(MAKE) run


depends: buildrepo
	@./scripts/make_depends.sh $(SRCDIR) $(OBJDIR)


clean:
	rm $(PROJECT) $(OBJDIR) -Rf
	cd tests; $(MAKE) clean

cleanall: clean
	cd tests; $(MAKE) cleanall


# From Zed Shaw's Learn C the hard way, chap 29
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check-badfuncs:
	@echo "Files with potentially dangerous functions :"
	@egrep $(BADFUNCS) $(CFILES) $(CPPFILES) || true


.PHONY: clean check_badfuncs depends buildrepo depends test
	
buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef


-include $(DEPFILES)
