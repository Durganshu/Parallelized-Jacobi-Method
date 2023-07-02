# Usage:
# For pure MPI version: make 
# For Hybrid MPI + OpenMP version: make VARIANT=HYBRID
# For non-blocking MPI version: make VARIANT=NON_BLOCKING

VARIANT :=

# CC=mpicc
# CFLAGS=-O3 -fopenmp 

CC = mpiicc 
CFLAGS = -O3 -fno-alias -xhost -fopenmp
ifeq ($(VARIANT),NON_BLOCKING)
    CFLAGS += -DNON_BLOCKING
else ifeq ($(VARIANT),HYBRID) 
	CFLAGS += -DHYBRID
else
	CFLAGS += -DPURE_MPI
endif
#-parallel -qopt-report-phase=par -qopt-report:5 
#-par-num-threads=2


all: heat 

heat : heat.o input.o misc.o timing.o relax_jacobi.o
	$(CC) $(CFLAGS) -o heat $+ -lm  

%.o : %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o heat *~ *.ppm

remake : clean all
