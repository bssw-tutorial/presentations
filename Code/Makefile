CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2

# Targets
TARGETS = mesh_lattice verify_mesh verify_particles test_move move_particles check_particle

# Object files for each target
MESH_LATTICE_OBJS      = main.o mesh.o lattice.o
VERIFY_MESH_OBJS       = verify_mesh.o mesh.o
VERIFY_PARTICLES_OBJS  = verify_particles.o mesh.o lattice.o
MOVE_PARTICLES_OBJS      = move_particles.o
TEST_MOVE_OBJS         = test_move.o mesh.o lattice.o move_particles.o
CHECK_PARTICLE_OBJS   = verify_particles.o mesh.o lattice.o

.PHONY: all clean move_particles

all: $(TARGETS)

mesh_lattice: $(MESH_LATTICE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

verify_mesh: $(VERIFY_MESH_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

verify_particles: $(VERIFY_PARTICLES_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test_move: $(TEST_MOVE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

move_particles: $(MOVE_PARTICLES_OBJS)
	@echo "Compiled move_particles (move_particles.o)"

check_particle: $(CHECK_PARTICLE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation rules
main.o: main.c mesh.h lattice.h constants.h
	$(CC) $(CFLAGS) -c main.c

mesh.o: mesh.c mesh.h constants.h
	$(CC) $(CFLAGS) -c mesh.c

lattice.o: lattice.c lattice.h mesh.h constants.h
	$(CC) $(CFLAGS) -c lattice.c

verify_mesh.o: verify_mesh.c mesh.h constants.h
	$(CC) $(CFLAGS) -c verify_mesh.c

verify_particles.o: verify_particles.c mesh.h lattice.h constants.h
	$(CC) $(CFLAGS) -c verify_particles.c

move_particles.o: move_particles.c mesh.h lattice.h constants.h
	$(CC) $(CFLAGS) -c move_particles.c

test_move.o: test_move.c mesh.h lattice.h move_particles.h constants.h
	$(CC) $(CFLAGS) -c test_move.c

clean:
	rm -f $(TARGETS) *.o *~
