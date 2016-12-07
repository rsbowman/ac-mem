#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <unordered_set>

#include "group.h"

typedef struct Hashtable {
    std::unordered_set<std::uint64_t> set;
    std::uint64_t adds;
    std::uint64_t discards;
} Hashtable;

void hashtable_init(Hashtable *h);
boolean hashtable_contains(Hashtable *h, Group *g);
boolean hashtable_add(Hashtable *h, Group *g);
std::uint64_t hashtable_size(Hashtable *h);

#endif
