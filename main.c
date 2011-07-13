#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "group.h"
#include "bfs.h"
#include "ord_tup.h"

int main(int argc, char **argv) {
  GroupProof gp;
  int queue_buffer_size = 1000000;
  int max_relator_length = 18;

  init_ordered_tuples(); // precompute some values

  if (argc < 4 || argc > 5) {
    printf("Usage: main r1 r2 path-to-queue-dir [max-depth]\n");
    printf("r1 and r2 are relators in {a, b, A, B}\n");
    exit(1);
  }
  
  if (argc == 5) {
    max_relator_length = atoi(argv[4]);
  }

  assert(1 < max_relator_length && max_relator_length < 50);

  group_proof_init(&gp);
  group_init_from_string(&gp.group, argv[1], argv[2]);

  if (search(&gp, max_relator_length, queue_buffer_size, argv[3]))
    printf("oh yeah baby!!! whoooo!!!!\n");
  else
    printf("(pffft)\n");

  return 0; // whatever
}
