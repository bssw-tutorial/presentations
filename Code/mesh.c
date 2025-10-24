#include "mesh.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Balanced factorization of nblk into NDIM factors stored in blk_per_dim.
 * Tries perfect root; otherwise greedily factors to keep dimensions balanced.
 */
static void decompose_blocks(int nblk, int NDIM, int blk_per_dim[])
{
    for (int i = 0; i < NDIM; ++i) blk_per_dim[i] = 1;

    int root = (int)round(pow((double)nblk, 1.0 / NDIM));
    int prod = 1;
    for (int i = 0; i < NDIM; ++i) prod *= root;

    if (prod == nblk) {
        for (int i = 0; i < NDIM; ++i) blk_per_dim[i] = root;
    } else {
        int remaining = nblk;
        while (remaining > 1) {
            int f;
            for (f = 2; f <= remaining; ++f) {
                if (remaining % f == 0) break;
            }
            int d = 0;
            for (int j = 1; j < NDIM; ++j)
                if (blk_per_dim[j] < blk_per_dim[d]) d = j;
            blk_per_dim[d] *= f;
            remaining /= f;
        }
    }
}

int createMesh(int NDIM, int NX,
               const float lbnd[], const float ubnd[],
               int nblk, const int boundaries[],
               float domainbnds[][HIGH + 1],
               int domainbc[],
               Block *blocks, int *nBlocksOut)
{
    if (NDIM < 1 || NDIM > MDIM) {
        fprintf(stderr, "createMesh: NDIM (%d) out of allowed range [1,%d]\n", NDIM, MDIM);
        return -1;
    }
    if (nblk < 1) {
        fprintf(stderr, "createMesh: nblk must be >=1\n");
        return -2;
    }

    for (int i = 0; i < NDIM; ++i) {
        domainbnds[i][LOW] = lbnd[i];
        domainbnds[i][HIGH] = ubnd[i];
        domainbc[i] = boundaries[i];
    }

    int blk_per_dim[MDIM];
    decompose_blocks(nblk, NDIM, blk_per_dim);

    int total_blocks = 1;
    for (int i = 0; i < NDIM; ++i)
        total_blocks *= blk_per_dim[i];

    if (total_blocks != nblk) {
        fprintf(stderr,
                "createMesh: warning requested nblk=%d, produced %d blocks (using decomposition), proceeding with %d.\n",
                nblk, total_blocks, total_blocks);
    }

    for (int b = 0; b < total_blocks; ++b) {
        int idx[MDIM];
        int rem = b;
        for (int i = NDIM - 1; i >= 0; --i) {
            idx[i] = rem % blk_per_dim[i];
            rem /= blk_per_dim[i];
        }

        blocks[b].blkID = b;
        for (int i = 0; i < NDIM; ++i) {
            float delta = (ubnd[i] - lbnd[i]) / (float)blk_per_dim[i];
            float lower = lbnd[i] + idx[i] * delta;
            float upper = lower + delta;
            blocks[b].bbnd[i][LOW] = lower;
            blocks[b].bbnd[i][HIGH] = upper;
        }
        for (int i = NDIM; i < MDIM; ++i) {
            blocks[b].bbnd[i][LOW] = 0.0f;
            blocks[b].bbnd[i][HIGH] = 0.0f;
        }
    }

    *nBlocksOut = total_blocks;
    return 0;
}
