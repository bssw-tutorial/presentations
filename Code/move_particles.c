#include <stdio.h>
#include <string.h>
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
                    int nBlocks)
{
    /* Compute new positions per dimension without modifying original until after checks */
    float newpos[MDIM];
    for (int d = 0; d < NDIM; ++d) {
        float lo = domainbnds[d][LOW];
        float hi = domainbnds[d][HIGH];
        float c  = particle->coords[d];
        int bc   = domainbc[d];
        float L  = hi - lo;
        /* Inside boundary: retain coordinate */
        if (c >= lo && c <= hi) {
            newpos[d] = c;
        } else {
            /* Outside boundary */
            if (bc == OUTFLOW) {
                return 0;  /* remove particle */
            }
            if (bc == REFLECTIVE) {
                /* reflect at the appropriate edge */
                int edge = (c < lo) ? LOW : HIGH;
                float edgeval = (edge == LOW) ? lo : hi;
                newpos[d] = 2.0f * edgeval - c;
            } else if (bc == PERIODIC) {
                /* wrap around periodic */
                if (c < lo)      newpos[d] = c + L;
                else /* c > hi */ newpos[d] = c - L;
            } else {
                /* unknown BC: keep as is */
                newpos[d] = c;
            }
        }
    }
    /* Apply new positions */
    for (int d = 0; d < NDIM; ++d) {
        particle->coords[d] = newpos[d];
    }
    /* Update block ID based on new position */
    particle->blkID = getBlkID(NDIM, particle->coords, blocks, nBlocks);
    return 1;
}
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
                    int steps)
{
    for (int step = 0; step < steps; ++step) {
        for (int i = 0; i < *nParticles; ++i) {
            Particle *p = &particles[i];
            /* move by -1.5 in each dimension */
            for (int d = 0; d < NDIM; ++d) {
                p->coords[d] -= 1.5f;
            }
            /* find new block */
            int newBlk = getBlkID(NDIM, p->coords, blocks, nBlocks);
            if (newBlk >= 0) {
                p->blkID = newBlk;
            } else {
                /* handle boundary; remove if outflow */
                if (!handle_boundary(NDIM,
                                     p,
                                     domainbnds,
                                     domainbc,
                                     blocks,
                                     nBlocks)) {
                    /* remove particle by shifting subsequent elements */
                    memmove(&particles[i],
                            &particles[i+1],
                            sizeof(Particle) * (*nParticles - i - 1));
                    (*nParticles)--;
                    i--;  /* adjust index for removed element */
                }
            }
        };
	    /* Print particles */
	printf("\nParticles (one per line):\n");
	for (int p = 0; p < *nParticles; ++p) {
	  printf("particle %d: blkID=%d coords=", p, particles[p].blkID);
	  for (int i = 0; i < NDIM; ++i) {
            printf("%f", particles[p].coords[i]);
            if (i < NDIM - 1) printf(",");
	  }
        printf("\n");
    }

    }
}

