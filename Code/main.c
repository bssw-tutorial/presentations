
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "mesh.h"
#include "lattice.h"

int main(void)
{
    int NDIM;
    printf("Enter NDIM (1..%d): ", MDIM);
    if (scanf("%d", &NDIM) != 1) {
        fprintf(stderr, "Failed to read NDIM\n");
        return 1;
    }
    if (NDIM < 1 || NDIM > MDIM) {
        fprintf(stderr, "Invalid NDIM: %d\n", NDIM);
        return 1;
    }

    int NX;
    printf("Enter NX: ");
    if (scanf("%d", &NX) != 1) {
        fprintf(stderr, "Failed to read NX\n");
        return 1;
    }

    float lbnd[MDIM];
    float ubnd[MDIM];
    printf("Enter lbnd (space-separated, %d values): ", NDIM);
    for (int i = 0; i < NDIM; ++i) {
        if (scanf("%f", &lbnd[i]) != 1) {
            fprintf(stderr, "Failed to read lbnd[%d]\n", i);
            return 1;
        }
    }
    printf("Enter ubnd (space-separated, %d values): ", NDIM);
    for (int i = 0; i < NDIM; ++i) {
        if (scanf("%f", &ubnd[i]) != 1) {
            fprintf(stderr, "Failed to read ubnd[%d]\n", i);
            return 1;
        }
    }

    int nblk;
    printf("Enter nblk (number of blocks): ");
    if (scanf("%d", &nblk) != 1) {
        fprintf(stderr, "Failed to read nblk\n");
        return 1;
    }

    int boundaries[MDIM];
    printf("Enter boundaries (space-separated %d values): ", NDIM);
    for (int i = 0; i < NDIM; ++i) {
        if (scanf("%d", &boundaries[i]) != 1) {
            fprintf(stderr, "Failed to read boundary[%d]\n", i);
            return 1;
        }
    }

    int npart;
    printf("Enter Npart (number of particles): ");
    if (scanf("%d", &npart) != 1) {
        fprintf(stderr, "Failed to read Npart\n");
        return 1;
    }

    /* Echo inputs */
    printf("\nInput summary:\n");
    printf("NDIM = %d\n", NDIM);
    printf("NX = %d\n", NX);
    printf("lbnd =");
    for (int i = 0; i < NDIM; ++i) printf(" %g", lbnd[i]);
    printf("\n");
    printf("ubnd =");
    for (int i = 0; i < NDIM; ++i) printf(" %g", ubnd[i]);
    printf("\n");
    printf("nblk = %d\n", nblk);
    printf("boundaries =");
    for (int i = 0; i < NDIM; ++i) printf(" %d", boundaries[i]);
    printf("\n");
    printf("Npart = %d\n", npart);

    /* Create mesh */
    float domainbnds[MDIM][HIGH + 1];
    int domainbc[MDIM];
    Block *blocks = malloc(sizeof(Block) * nblk);
    if (!blocks) {
        fprintf(stderr, "Allocation failed for blocks\n");
        return 1;
    }
    int actual_blocks = 0;
    if (createMesh(NDIM, NX, lbnd, ubnd, nblk, boundaries,
                   domainbnds, domainbc, blocks, &actual_blocks) != 0) {
        fprintf(stderr, "createMesh failed\n");
        free(blocks);
        return 1;
    }

    /* Print blocks */
    printf("\nBlocks (one per line):\n");
    for (int b = 0; b < actual_blocks; ++b) {
        printf("blkID=%d", blocks[b].blkID);
        for (int i = 0; i < NDIM; ++i) {
            printf(" [dim%d: %f,%f]", i, blocks[b].bbnd[i][LOW], blocks[b].bbnd[i][HIGH]);
        }
        printf("\n");
    }

    /* Create lattice */
    Particle *particles = malloc(sizeof(Particle) * npart);
    if (!particles) {
        fprintf(stderr, "Allocation failed for particles\n");
        free(blocks);
        return 1;
    }
    int actual_npart = 0;
    if (createLattice(NDIM, npart, blocks, actual_blocks,
                      domainbnds, particles, &actual_npart) != 0) {
        fprintf(stderr, "createLattice failed\n");
        free(blocks);
        free(particles);
        return 1;
    }

    /* Print particles */
    printf("\nParticles (one per line):\n");
    for (int p = 0; p < actual_npart; ++p) {
        printf("particle %d: blkID=%d coords=", p, particles[p].blkID);
        for (int i = 0; i < NDIM; ++i) {
            printf("%f", particles[p].coords[i]);
            if (i < NDIM - 1) printf(",");
        }
        printf("\n");
    }

    free(blocks);
    free(particles);
    return 0;
}
