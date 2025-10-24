#include "lattice.h"
#include <stdio.h>
#include <math.h>

/* Balanced factorization helper */
static void factorBalanced(int n, int NDIM, int out_counts[])
{
    for (int i = 0; i < NDIM; ++i) out_counts[i] = 1;
    int remaining = n;
    while (remaining > 1) {
        int f;
        for (f = 2; f <= remaining; ++f) {
            if (remaining % f == 0) break;
        }
        int d = 0;
        for (int j = 1; j < NDIM; ++j)
            if (out_counts[j] < out_counts[d]) d = j;
        out_counts[d] *= f;
        remaining /= f;
    }
}

int getBlkID(int NDIM, const float pos[], const Block *blocks, int nBlocks)
{
    if (NDIM < 1 || NDIM > MDIM || blocks == NULL || nBlocks <= 0 || pos == NULL) {
        return -1;
    }

    for (int b = 0; b < nBlocks; ++b) {
        int inside = 1;
        for (int i = 0; i < NDIM; ++i) {
            float lower = blocks[b].bbnd[i][LOW];
            float upper = blocks[b].bbnd[i][HIGH];
            if (pos[i] < lower || pos[i] > upper) {
                inside = 0;
                break;
            }
        }
        if (inside) return blocks[b].blkID;
    }
    return -1;
}

int createLattice(int NDIM,
                  int npart,
                  const Block *blocks,
                  int nBlocks,
                  const float domainbnds[][HIGH + 1],
                  Particle *particles,
                  int *actual_npart_out)
{
    if (NDIM < 1 || NDIM > MDIM) return -1;
    if (npart < 1) return -2;
    if (blocks == NULL || nBlocks <= 0 || particles == NULL || actual_npart_out == NULL) return -3;

    int npd[MDIM];
    factorBalanced(npart, NDIM, npd);
    int total = 1;
    for (int i = 0; i < NDIM; ++i) total *= npd[i];
    if (total != npart) {
        fprintf(stderr, "createLattice: mismatch expected %d particles, got %d\n", npart, total);
    }

    for (int p = 0; p < total; ++p) {
        int idx[MDIM];
        int rem = p;
        for (int i = NDIM - 1; i >= 0; --i) {
            idx[i] = rem % npd[i];
            rem /= npd[i];
        }
        float coord[MDIM];
        for (int i = 0; i < NDIM; ++i) {
            float low = domainbnds[i][LOW];
            float high = domainbnds[i][HIGH];
            float span = high - low;
            if (span <= 0.0f) {
                coord[i] = low;
            } else {
                float delta = span / npd[i];
                coord[i] = low + (idx[i] + 0.5f) * delta;
            }
        }
        for (int i = 0; i < NDIM; ++i) particles[p].coords[i] = coord[i];
        for (int i = NDIM; i < MDIM; ++i) particles[p].coords[i] = 0.0f;
        particles[p].blkID = getBlkID(NDIM, coord, blocks, nBlocks);
        if (particles[p].blkID == -1) {
            fprintf(stderr, "createLattice: particle %d outside domain\n", p);
        }
    }

    *actual_npart_out = total;
    return 0;
}
