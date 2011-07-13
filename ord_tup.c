#include <assert.h>

#include "ord_tup.h"

/* XXXXXXXXXXXXXXXXXXXXXXXXXX: 
   cache the crap out of all this stuff
 */


/* look at all n-tuples, give the number of elements occuring before
   there's a k in the first place; eg. for two elements
   00 <- cutoff(2, 0) = 0
   10 <- cutoff(2, 1) = 1
   11
   20 <- cutoff(2, 2) = 3
   21
   22
   30 <- cutoff(2, 3) = 6
   31
   32
   33 ...
 */
relator_index_t cutoff(int n, int k) {
  int i;
  relator_index_t sum = 0;

  if (n <= 1)
    return k;
  else {
    for (i = 1; i <= k; ++i)
      sum += cutoff(n - 1, i);
  }
  return sum;
}

void init_ordered_tuples() {
  init_cutoff_table();
  init_ordered_tuples_four();
}

#define MAX_TUPLE_SIZE 64
relator_index_t cutoff_table[MAX_TUPLE_SIZE][4] = {{0}};

void init_cutoff_table() {
  int i, j;
  for (i = 0; i < MAX_TUPLE_SIZE; ++i) {
    for (j = 0; j < 4; ++j)
      cutoff_table[i][j] = cutoff(i, j);
  }
}

/* find the number of ordered n-tuples from an alphabet of m letters
 */
relator_index_t compute_ordered_tuples(int n, int m) {
  int i;
  relator_index_t sum = 0;

  if (n <= 1)
    return m;
  else {
    for (i = 1; i <= m; ++i)
      sum += compute_ordered_tuples(n - 1, i);
  }
  return sum;
}

relator_index_t ordered_tuples_table[MAX_TUPLE_SIZE] = {0};

void init_ordered_tuples_four() {
  int i;
  for (i = 0; i < MAX_TUPLE_SIZE; ++i) {
    ordered_tuples_table[i] = compute_ordered_tuples(i, 4);
  }
}

/* the above specialized to m = 4*/
relator_index_t ordered_tuples_four(int n) {
  assert(n > 0 && n < MAX_TUPLE_SIZE && ordered_tuples_table[0]);
  return ordered_tuples_table[n];
}

relator_index_t cumulative_tuples(relator_index_t n, relator_index_t m) {
  int i;
  relator_index_t sum = 0;

  for (i = 1; i <= n; ++i)
    sum += compute_ordered_tuples(i, m);

  return sum;
}

/* return the index of a relator (considered as an ordered tuple) in
   the list of ordered tuples sorted lexicographically.

   XXXXXXXXXXXXXXXX: the relator must by in canonical form.  this
   function is specialized for the case when the alphabet consists of
   four letters.

   XXXX: length must be <= 32 in order for the square of the result to
   fit in a 32 bit integer (this matters because we hash on TWO relators).
   reason: cumulative_tuples(33, 4)**2 > 2**32
 */
relator_index_t relator_index(Relator r, int length) {
  int fst;
  Relator snd;

  assert(length > 0 && length < MAX_TUPLE_SIZE);

  if (length == 1)
    return (relator_index_t)r;
  else {
    fst = r & ONES(2);
    snd = r >> 2;

    return cutoff_table[length][fst] +
      relator_index(snd, length - 1) +
      ordered_tuples_table[length - 1];
  }
}

/* MAX_TUPS = cumulative_tuples(32, 4).  this is the maximum number
   of tuples we can handle with 32 bit integers.
 */
#define MAX_TUPS 58904

/* as mentioned above, on 32 bit machines this only works when the
   relator lengths are <= 32.
 */
relator_index_t group_hash(Group *g) {
  relator_index_t ri1, ri2;
  ri1 = relator_index(g->relator[0], g->len[0]);
  ri2 = relator_index(g->relator[1], g->len[1]);

  return ri1*MAX_TUPS + ri2;
}
