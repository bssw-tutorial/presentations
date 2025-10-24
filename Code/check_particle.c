#include <stdio.h>
#include "constants.h"
#include "mesh.h"
#include "lattice.h"

/**
 * check_particle:
 *   Verifies that a given particle lies within the bounds of its assigned block.
 *
 *   NDIM      - dimensionality of the mesh (1..MDIM)
 *   particle  - pointer to the Particle to check
 *   blocks    - array of Block objects as created by createMesh
 *   nBlocks   - number of blocks in the array
 *
 *   Returns 1 if the particle’s coords lie within
 *   blocks[particle->blkID].bbnd in all dimensions, 0 otherwise.
 */
int check_particle(int NDIM,
                   const Particle *particle,
                   const Block *blocks,
                   int nBlocks)
{
    int bid = particle->blkID;
    /* first, block ID must be in range */
    if (bid < 0 || bid >= nBlocks) {
        fprintf(stderr,
                "check_particle: invalid blkID %d (should be in [0,%d))\n",
                bid, nBlocks);
        return 0;
    }

    /* retrieve the block’s bounding box */
    const Block *blk = &blocks[bid];

    /* check each coordinate lies within [LOW, HIGH] */
    for (int i = 0; i < NDIM; ++i) {
        float c = particle->coords[i];
        float lo = blk->bbnd[i][LOW];
        float hi = blk->bbnd[i][HIGH];
        if (c < lo || c > hi) {
            fprintf(stderr,
                    "check_particle: particle at coord[%d]=%g outside block %d bounds [%g,%g]\n",
                    i, c, bid, lo, hi);
            return 0;
        }
    }

    return 1;
}
