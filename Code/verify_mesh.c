/* verify_mesh.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"
#include "mesh.h"

int main(void)
{
    char filename[256];
    printf("Enter filename to verify: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error: failed to read filename\n");
        return 1;
    }

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
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "NDIM =")) {
            sscanf(line, "NDIM = %d", &NDIM);
        }
        else if (strstr(line, "NX =")) {
            sscanf(line, "NX = %d", &NX);
        }
        else if (strstr(line, "lbnd =")) {
            char *p = strchr(line, '=');
            if (p) {
                p++;
                for (int i = 0; i < NDIM; ++i) {
                    while (*p && isspace((unsigned char)*p)) p++;
                    lbnd[i] = strtof(p, &p);
                }
            }
        }
        else if (strstr(line, "ubnd =")) {
            char *p = strchr(line, '=');
            if (p) {
                p++;
                for (int i = 0; i < NDIM; ++i) {
                    while (*p && isspace((unsigned char)*p)) p++;
                    ubnd[i] = strtof(p, &p);
                }
            }
        }
        else if (strstr(line, "nblk =")) {
            sscanf(line, "nblk = %d", &nblk);
        }
        else if (strstr(line, "boundaries =")) {
            char *p = strchr(line, '=');
            if (p) {
                p++;
                for (int i = 0; i < NDIM; ++i) {
                    while (*p && isspace((unsigned char)*p)) p++;
                    boundaries[i] = strtol(p, &p, 10);
                }
            }
        }
        else if (strstr(line, "Npart =")) {
            sscanf(line, "Npart = %d", &npart);
        }
    }
    fclose(fp);

    /* Basic sanity check */
    if (NDIM < 1 || NDIM > MDIM) {
        fprintf(stderr, "Error: parsed NDIM=%d out of range\n", NDIM);
        return 1;
    }

    /* Recreate the mesh */
    float domainbnds[MDIM][HIGH+1];
    int domainbc[MDIM];
    Block *blocks = malloc(sizeof(Block) * nblk);
    int actual_blocks = 0;
    if (!blocks ||
        createMesh(NDIM, NX, lbnd, ubnd, nblk, boundaries,
                   domainbnds, domainbc, blocks, &actual_blocks) != 0)
    {
        fprintf(stderr, "Error: createMesh failed\n");
        free(blocks);
        return 1;
    }

    /* Verify domain bounds and BCs */
    int ok = 1;
    for (int i = 0; i < NDIM; ++i) {
        if (domainbnds[i][LOW] != lbnd[i]) {
            fprintf(stderr, "Mismatch lbnd[%d]: expected %g, got %g\n",
                    i, lbnd[i], domainbnds[i][LOW]);
            ok = 0;
        }
        if (domainbnds[i][HIGH] != ubnd[i]) {
            fprintf(stderr, "Mismatch ubnd[%d]: expected %g, got %g\n",
                    i, ubnd[i], domainbnds[i][HIGH]);
            ok = 0;
        }
        if (domainbc[i] != boundaries[i]) {
            fprintf(stderr, "Mismatch boundary[%d]: expected %d, got %d\n",
                    i, boundaries[i], domainbc[i]);
            ok = 0;
        }
    }

    if (ok) {
        printf("Domain verification passed.\n");
    } else {
        printf("Domain verification failed.\n");
    }

    free(blocks);
    return ok ? 0 : 1;
}
