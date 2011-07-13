#ifndef ORD_TUP_H
#define ORD_TUP_H

#include "group.h"

typedef unsigned int relator_index_t;

relator_index_t cutoff(int, int);
relator_index_t relator_index(Relator r, int length);
relator_index_t compute_ordered_tuples(int, int);
relator_index_t cumulative_tuples(relator_index_t n, relator_index_t m);
relator_index_t group_hash(Group *g);

void init_ordered_tuples();
void init_cutoff_table();
void init_ordered_tuples_four();
relator_index_t ordered_tuples_four(int n);

#endif
