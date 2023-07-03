# Group 6: Heat parallelization

The code consists of Pure MPI (with and without blocking) and Hybrid parallelization (MPI + OpenMP)

Use the provided `Makefile` to compile the code according to the desired MPI variant.

## Building the code

### For pure MPI version (blocking): 

```shell
make 
```

### For non-blocking MPI version: 

```shell
make VARIANT=NON_BLOCKING 
```

### For Hybrid MPI + OpenMP version:
```shell
make VARIANT=HYBRID
```

## Running the code

Run these commands in the interactive shell of batch node. These can also be run in the batch script.

### For pure MPI version (blocking): 

```shell
mpirun -np <number of processses> <options> ./heat test.date <number of processses in x> <number of processses in y>
```

### For non-blocking MPI version: 

```shell
mpirun -np <number of processses> <options> ./heat test.date <number of processses in x> <number of processses in y>
```

### For Hybrid MPI + OpenMP version:
```shell
export OMP_NUM_THREADS=<number of omp threads>
mpirun -np <number of processses> <options> ./heat test.date <number of processses in x> <number of processses in y>
```
<options> may include options for binding, affinity or number of tasks per node


