# Parallelized Jacobi Method

The code implements the Jacobi method on a discretized 5-point stencil with shared and distributed memory parallelism.

Three flavors of parallelization are available:
1. Using MPI (with blocking communication)
2. Using MPI (with non-blocking communication)
3. Using Hybrid parallelization (MPI + OpenMP)

The code has been tested on [SuperMUC](https://doku.lrz.de/supermuc-ng-10745965.html).

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


### For pure MPI version (blocking): 

```shell
mpirun -np <number of processses> <options> ./heat test.date <number of processses in x> <number of processses in y>
```

Example usage:

```mpirun -np 4 ./heat test.dat 2 2```

### For non-blocking MPI version: 

```shell
mpirun -np <number of processses> <options> ./heat test.date <number of processses in x> <number of processses in y>
```
Example usage:
```mpirun -np 4 ./heat test.dat 2 2```

### For Hybrid MPI + OpenMP version:
```shell
export OMP_NUM_THREADS=<number of omp threads>
mpirun -np <number of processses> <options> ./heat test.date <number of processses in x> <number of processses in y>
```
`options` may include options for binding, affinity or number of tasks per node

Example usage for 16 hardware threads:

`export OMP_NUM_THREADS=4`

`mpirun -np 4 ./heat test.dat 2 2`
