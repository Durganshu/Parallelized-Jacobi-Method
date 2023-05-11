# Intel compiler
CC =  icc
CFLAGS = -O2

#-xhost -I/dss/lrzsys/sys/spack/release/22.2.1/opt/skylake_avx512/likwid/5.2.2-intel-kajex7c/include -L/dss/lrzsys/sys/spack/release/22.2.1/opt/skylake_avx512/likwid/5.2.2-intel-kajex7c/lib/ -DLIKWID_PERFMON

MPICC = mpicc

all: heat

heat : heat.o input.o misc.o timing.o relax_gauss.o relax_jacobi.o
	$(CC) $(CFLAGS) -o $@ $+ -lm 
	
#-llikwid

%.o : %.c heat.h timing.h input.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o heat *~ *.ppm

remake : clean all
