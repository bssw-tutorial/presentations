#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "constants.h"
#include "mesh.h"
#include "lattice.h"

#define EPS 1e-5

/**
 * verify_particles:
 *   Reads a sample run output file, parses the mesh inputs and the particle list,
 *   rebuilds the mesh and lattice, and compares each parsed particle against
 *   the generated one. Reports mismatches.
 *
 * Returns 0 if all particles match, 1 otherwise.
 */
int verify_particles(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    /* Parsed inputs */
    int NDIM = 0, NX = 0, nblk = 0, npart = 0;
    float lbnd[MDIM] = {0}, ubnd[MDIM] = {0};
    int boundaries[MDIM] = {0};

    char line[512];
    int parsedCount = 0;
    int *parsed_blkID = NULL;
    float (*parsed_coords)[MDIM] = NULL;
    int inParticles = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (!inParticles) {
            if (strstr(line, "NDIM =")) {
                sscanf(line, "NDIM = %d", &NDIM);
            }
            else if (strstr(line, "NX =")) {
                sscanf(line, "NX = %d", &NX);
            }
            else if (strstr(line, "lbnd =")) {
                char *p = strchr(line, '=');
                for (int i = 0; i < NDIM && p; ++i) {
                    lbnd[i] = strtof(p+1, &p);
                }
            }
            else if (strstr(line, "ubnd =")) {
                char *p = strchr(line, '=');
                for (int i = 0; i < NDIM && p; ++i) {
                    ubnd[i] = strtof(p+1, &p);
                }
            }
            else if (strstr(line, "nblk =")) {
                sscanf(line, "nblk = %d", &nblk);
            }
            else if (strstr(line, "boundaries =")) {
                char *p = strchr(line, '=');
                for (int i = 0; i < NDIM && p; ++i) {
                    boundaries[i] = strtol(p+1, &p, 10);
                }
            }
            else if (strstr(line, "Npart =")) {
                sscanf(line, "Npart = %d", &npart);
                /* allocate arrays once we know npart */
                parsed_blkID   = malloc(sizeof(int) * npart);
                parsed_coords  = malloc(sizeof(*parsed_coords) * npart);
                if (!parsed_blkID || !parsed_coords) {
                    fprintf(stderr, "Allocation failed for parsed arrays\n");
                    fclose(fp);
                    return 1;
                }
            }
            else if (strstr(line, "Particles")) {
                inParticles = 1;
            }
        }
        else if (parsedCount < npart) {
            /* parse lines like:
             *   particle 0: blkID=0 coords=4.000000,4.000000
             */
            char *pBlk = strstr(line, "blkID=");
            char *pC   = strstr(line, "coords=");
            if (pBlk && pC) {
                int blk = strtol(pBlk + 6, NULL, 10);
                float coords[MDIM];
                char *p = pC + 7;
                for (int i = 0; i < NDIM; ++i) {
                    coords[i] = strtof(p, &p);
                    if (*p == ',') ++p;
                }
                parsed_blkID[parsedCount] = blk;
                for (int i = 0; i < NDIM; ++i) {
                    parsed_coords[parsedCount][i] = coords[i];
                }
                ++parsedCount;
            }
        }
    }
    fclose(fp);

    if (parsedCount != npart) {
        fprintf(stderr, "Error: expected %d particles, parsed %d\n", npart, parsedCount);
        free(parsed_blkID);
        free(parsed_coords);
        return 1;
    }

    /* Recreate mesh */
    float domainbnds[MDIM][HIGH+1];
    int domainbc[MDIM];
    Block *blocks = malloc(sizeof(Block) * nblk);
    int actual_blocks = 0;
    if (!blocks ||
        createMesh(NDIM, NX, lbnd, ubnd, nblk, boundaries,
                   domainbnds, domainbc, blocks, &actual_blocks) != 0) {
        fprintf(stderr, "Error: createMesh failed\n");
        free(parsed_blkID);
        free(parsed_coords);
        free(blocks);
        return 1;
    }

    /* Recreate lattice */
    Particle *gen = malloc(sizeof(Particle) * npart);
    int actual_n = 0;
    if (!gen ||
        createLattice(NDIM, npart, blocks, actual_blocks,
                      domainbnds, gen, &actual_n) != 0) {
        fprintf(stderr, "Error: createLattice failed\n");
        free(parsed_blkID);
        free(parsed_coords);
        free(blocks);
        free(gen);
        return 1;
    }

    /* Compare parsed vs generated */
    int ok = 1;
    for (int p = 0; p < npart; ++p) {
        if (parsed_blkID[p] != gen[p].blkID) {
            fprintf(stderr,
                    "Mismatch blkID for particle %d: parsed %d, gen %d\n",
                    p, parsed_blkID[p], gen[p].blkID);
            ok = 0;
        }
        for (int i = 0; i < NDIM; ++i) {
            float e = gen[p].coords[i];
            float a = parsed_coords[p][i];
            if (fabs(e - a) > EPS) {
                fprintf(stderr,
                        "Mismatch coord[%d] for particle %d: parsed %g, gen %g\n",
                        i, p, a, e);
                ok = 0;
            }
        }
    }

    if (ok) {
        printf("Particle verification passed.\n");
    } else {
        printf("Particle verification failed.\n");
    }

    free(parsed_blkID);
    free(parsed_coords);
    free(blocks);
    free(gen);
    return ok ? 0 : 1;
}

int main(void) {
    char filename[256];
    printf("Enter filename to verify particles: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Failed to read filename\n");
        return 1;
    }
    return verify_particles(filename);
}
