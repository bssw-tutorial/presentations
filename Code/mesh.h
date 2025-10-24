#ifndef MESH_H
#define MESH_H

#include "constants.h"

/* A block in the Cartesian decomposition */
typedef struct {
    int blkID;                          /* lexicographic block ID (starts at 0) */
    float bbnd[MDIM][HIGH + 1];         /* [dim][LOW/HIGH] bounds; uses LOW=1, HIGH=2 */
} Block;

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

#endif /* MESH_H */
