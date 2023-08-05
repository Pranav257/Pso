# Pso
The program expects command-line arguments to be passed when executed.

The command-line arguments are as follows:

function-name: Name of the function to optimize.
dimension: Dimensionality of the search space (number of variables in the function).
swarm-size: Number of particles (individuals) in the swarm.
xmin, xmax: Lower and upper bounds on the search domain (constraints for the variables).
max-iter: Number of iterations to run the optimizer.
num-threads: Number of threads to create for running PSO using OpenMP.
The main function validates the input arguments and calls two optimization functions:

optimize_gold: This function optimizes the given function using the reference version (non-parallel) of PSO.
optimize_using_omp: This function is expected to be completed to perform PSO using OpenMP for parallel optimization.
After executing optimize_gold, the best-performing particle is printed to the standard output.

The program is designed to be run from the command line, and the results will be displayed there.

To use the program, you need to compile it with the provided Makefile by running make. If you make any changes to the header file .h, make sure to delete the existing executable and rebuild by running make clean && make.

As the comments indicate, the optimize_using_omp function is not yet implemented, and it's your task to complete it using OpenMP to achieve parallel optimization. You can refer to the provided reference for the original PSO algorithm implementation (non-parallel) to understand the logic and apply parallelization using OpenMP.
