#ifndef MOVE_PARTICLES_H
#define MOVE_PARTICLES_H

#include "constants.h"
#include "mesh.h"
#include "lattice.h"


/**
 * handle_boundary:
 *   Applies boundary conditions for a particle outside the domain.
 *   - OUTFLOW: remove particle (return 0)
 *   - REFLECTIVE or PERIODIC: adjust position and update blkID, return 1
 *
 *   NDIM           - dimensionality
 *   particle       - particle to update
 *   domainbnds     - [NDIM][LOW/HIGH] domain bounds
 *   domainbc       - [NDIM] boundary condition codes
 *   blocks, nBlocks- mesh blocks list
 */
int handle_boundary(int NDIM,
                    Particle *particle,
                    const float domainbnds[][HIGH+1],
                    const int domainbc[],
                    const Block *blocks,
                    int nBlocks);
/*
 * createMesh:
 *   Inputs:
 *     NDIM        - dimensionality (1 <= NDIM <= MDIM)
 *     NX          - number of data points along each dimension (unused for partitioning)
 *     lbnd        - length-NDIM array of lower bounds per dimension
 *     ubnd        - length-NDIM array of upper bounds per dimension
 *     nblk        - total number of blocks to create
 *     boundaries  - length-NDIM array of boundary condition codes
 *
 *   Outputs:
 *     domainbnds  - [NDIM][HIGH+1] array: domainbnds[i][LOW] and domainbnds[i][HIGH]
 *     domainbc    - length-NDIM array (copied from boundaries)
 *     blocks      - preallocated array of Block; should have space for at least nblk
 *     nBlocksOut  - actual number of blocks produced (may differ from requested if factoring uneven)
 *
 *   Return:
 *     0 on success, non-zero on error.
 */
int createMesh(int NDIM, int NX,
               const float lbnd[], const float ubnd[],
               int nblk, const int boundaries[],
               float domainbnds[][HIGH + 1],
               int domainbc[],
               Block *blocks, int *nBlocksOut);


/**
 * move_particles:
 *   Moves all particles by -1.5 along each dimension for a given number of steps.
 *   Updates each particle's block membership or handles boundary conditions.
 *
 *   NDIM           - dimensionality
 *   particles      - array of Particle, size *nParticles
 *   nParticles     - pointer to number of particles (may shrink)
 *   blocks, nBlocks- mesh blocks list
 *   domainbnds     - domain bounds from createMesh
 *   domainbc       - boundary conditions array
 *   steps          - number of move steps (each moves by -1.5 in every dim)
 */
void move_particles(int NDIM,
                    Particle *particles,
                    int *nParticles,
                    const Block *blocks,
                    int nBlocks,
                    const float domainbnds[][HIGH+1],
                    const int domainbc[],
                    int steps);

#endif /* MOVE_PARTICLES_H */
