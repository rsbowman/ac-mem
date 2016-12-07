#include <assert.h>

#include "hashtable.h"
#include "group.h"
#include "ord_tup.h"

void hashtable_init(Hashtable *h) {
  h->adds = 0;
  h->discards = 0;
}

boolean hashtable_contains(Hashtable *h, Group *g) {
  Group canonical;

  group_copy(&canonical, g);
  group_canonicalize(&canonical);

  return h->set.count(group_hash(&canonical));
}

boolean hashtable_add(Hashtable *h, Group *g) {
  Group canonical;

  group_copy(&canonical, g);
  group_canonicalize(&canonical);

  auto pair = h->set.insert(group_hash(&canonical));
  auto is_new = pair.second;

  if (is_new)
    h->adds++;
  else
    h->discards++;

  return is_new;
}

std::uint64_t hashtable_size(Hashtable *h) {
    return h->set.size();
}
