/* Implementation of PSO using OpenMP.
 *
 * Author: Naga Kandasamy
 * Date: May 10, 2023
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "pso.h"
#include <omp.h>

int optimize_using_omp(char *function, int dim, int swarm_size, 
                       float xmin, float xmax, int num_iter, int num_threads)
{
int i, j, iter;
float w, c1, c2;
float r1, r2;
float curr_fitness;
particle_t *particle, *gbest;
int g = -1;
float fitness = INFINITY;


    /* The pso_init() function */
swarm_t* swarm = (swarm_t *)malloc(sizeof(swarm_t));
swarm->num_particles = swarm_size;
swarm->particle = (particle_t *)malloc(swarm_size*sizeof(particle_t)); 
for (i = 0; i < swarm->num_particles; i++) {
    particle = &swarm->particle[i];
    particle->dim = dim;

    /* Generate random particle position */
    particle->x = (float *)malloc(dim * sizeof(float)); 

    for (j = 0; j < dim; j++)
        particle->x[j] = uniform(xmin, xmax); /* Generate random particle velocity */
        
    particle->v = (float *)malloc(dim * sizeof(float));

            for (j = 0; j < dim; j++)
                particle->v[j] = uniform(-fabsf(xmax - xmin),fabsf(xmax - xmin));

    /* Initialize best position for particle */
    particle->pbest = (float *)malloc(dim * sizeof(float)); for (j = 0; j < dim; j++)
    particle->pbest[j] = particle->x[j];
/* Initialize particle fitness */ 
pso_eval_fitness(function, particle, &fitness); 
particle->fitness = fitness;
particle->g = -1;
}
/* Get index of particle with best fitness */
g = pso_get_best_fitness(swarm);
for (i = 0; i < swarm->num_particles; i++) {
    particle_t *particle = &swarm->particle[i];
    particle->g = g;
}


/* The pso_solve() function */
w = 0.79;
c1 = c2 = 1.49;
iter = 0;
#pragma omp parallel num_threads(num_threads) default(shared)

while (iter < num_iter) {
//#pragma omp for private(i, j, r1, r2, particle, gbest, curr_fitness)
for (i = 0; i < swarm->num_particles; i++) { 
    particle = &swarm->particle[i];
    gbest = &swarm->particle[particle->g]; 
    for (j = 0; j < particle->dim; j++) {
        r1 = (float)rand()/(float)RAND_MAX; 
        r2 = (float)rand()/(float)RAND_MAX; /* Update particle velocity */ 
        particle->v[j] = w * particle->v[j] + c1 * r1 * (particle->pbest[j] - particle->x[j])
                             + c2 * r2 * (gbest->x[j] - particle->x[j]); 
        if ((particle->v[j] < -fabsf(xmax - xmin)) || (particle->v[j] > fabsf(xmax - xmin)))
            particle->v[j] = uniform(-fabsf(xmax - xmin), fabsf(xmax - xmin));
            /* Update particle position */

        particle->x[j] = particle->x[j] + particle->v[j]; if (particle->x[j] > xmax)
        particle->x[j] = xmax; if (particle->x[j] < xmin) particle->x[j] = xmin;
} 

/* Update state for each particle */ 
pso_eval_fitness(function, particle, &curr_fitness);
    if (curr_fitness < particle->fitness) { /* Update pbest */
        particle->fitness = curr_fitness; 
        for (j = 0; j < particle->dim; j++)
                particle->pbest[j] = particle->x[j];
}
}
 /* Particle loop */
//#pragma omp single
g = pso_get_best_fitness(swarm); /* Update best particle */ 

//#pragma omp for private(i, particle)
for (i = 0; i < swarm->num_particles; i++) {
        particle = &swarm->particle[i];
        particle->g = g;
    }

//#pragma omp single
iter++;
} /* End of iteration */

srand(time(NULL));
swarm = pso_init(function, dim, swarm_size, xmin, xmax); 
//int g;
g = pso_solve_gold(function, swarm, xmax, xmin, num_iter);
if (g >= 0) {
    fprintf(stderr, "Solution:\n");
    pso_print_particle(&swarm->particle[g]);
}
pso_free(swarm);

return g;
}
