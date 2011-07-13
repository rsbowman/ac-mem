#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Judy.h"
#include "group.h"

typedef struct Hashtable {
  Pvoid_t judy_array;
  Word_t adds;
  Word_t discards;
} Hashtable;

void hashtable_init(Hashtable *h);
boolean hashtable_contains(Hashtable *h, Group *g);
boolean hashtable_add(Hashtable *h, Group *g);
Word_t hashtable_size(Hashtable *h);

#endif
