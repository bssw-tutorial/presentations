#ifndef LATTICE_H
#define LATTICE_H

#include "constants.h"
#include "mesh.h"

/* Particle representation */
typedef struct {
    int blkID;                  /* block containing the particle (-1 if outside) */
    float coords[MDIM];         /* position; only first NDIM entries are meaningful */
} Particle;

/* Given position pos[0..NDIM-1], returns block ID using explicit block list. */
int getBlkID(int NDIM, const float pos[], const Block *blocks, int nBlocks);

/* Create regular lattice of npart particles spread over domain, assign each to a block. */
int createLattice(int NDIM,
                  int npart,
                  const Block *blocks,
                  int nBlocks,
                  const float domainbnds[][HIGH + 1],
                  Particle *particles,
                  int *actual_npart_out);

#endif /* LATTICE_H */
