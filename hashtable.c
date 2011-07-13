#include <assert.h>

#include "hashtable.h"
#include "group.h"
#include "ord_tup.h"

void hashtable_init(Hashtable *h) {
  h->judy_array = NULL;
  h->adds = 0;
  h->discards = 0;
}

boolean hashtable_contains(Hashtable *h, Group *g) {
  int ret;
  Group canonical;

  group_copy(&canonical, g);
  group_canonicalize(&canonical);

  // test if g is in array
  J1T(ret, h->judy_array, group_hash(&canonical));
  return ret;
}

boolean hashtable_add(Hashtable *h, Group *g) {
  int ret;
  Group canonical;

  group_copy(&canonical, g);
  group_canonicalize(&canonical);

  // add g; return 1 if g wasn't already present
  J1S(ret, h->judy_array, group_hash(&canonical));

  if (ret)
    h->adds++;
  else
    h->discards++;

  return ret;
}

Word_t hashtable_size(Hashtable *h) {
  Word_t ret;
  J1C(ret, h->judy_array, 0, -1);

  if (ret != h->adds)
    printf("judy says %d, hash says %d\n", (int)ret, (int)h->adds);
  assert(ret == h->adds);

  return h->adds;
}

