ifndef OCCA_DIR
ERROR:
	@echo "Error, environment variable [OCCA_DIR] is not set"
endif

include ${OCCA_DIR}/scripts/Makefile

# define variables
HDRDIR = 

# set options for this machine
# specify which compilers to use for c, fortran and linking
cc	= gcc
FC	= g77  
#-std=legacy -fdefault-real-8 -fdefault-double-8 
CC	= g++
LD	= g++

NVCC	= nvcc

# compilerFlags += 

nvccFlags = -I./ -g


# s-I$(ALMONDDIR)
# compiler flags to be used (set to compile with debugging on)
CFLAGS = -I. $(compilerFlags) $(flags) -I$(HDRDIR) -I$(OGSDIR)  -D DFUN='"${CURDIR}"' $(LIBP_OPT_FLAGS) 

# link flags to be used
LDFLAGS	=  $(compilerFlags) $(LIBP_OPT_FLAGS)

# libraries to be linked in
LIBS	=  -L$(OCCA_DIR)/lib -locca $(linkerFlags)

#-L$(PARMETISDIR) -lparmetis -L$(METISDIR) -lmetis  -L$(ALMONDDIR) -lparAlmond

INCLUDES = 

DEPS = $(INCLUDES) 

# types of files we are going to construct rules for
.SUFFIXES: .c .f .cu .cpp

# rule for .c files
.c.o: $(DEPS)
	$(CC) $(CFLAGS) -o $*.o -c $*.c $(paths)

.cpp.o: $(DEPS)
	$(CC) $(CFLAGS) -o $*.o -c $*.cpp $(paths)

.cu.o: $(DEPS)
	$(NVCC) $(nvccFlags) -o $*.o -c $*.cu $(paths)

.f.o: $(DEPS)
	$(FC) $(CFLAGS) -o $*.o -c $*.f $(paths)


# library objects
LOBJS = 

main: $(LOBJS) ./src/main.o
	$(LD)  $(LDFLAGS)  -o main ./src/main.o $(LOBJS) $(paths) $(LIBS)

all: main 

# what to do if user types "make clean"
clean:
	rm src/*.o main 


