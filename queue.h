#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#include "group.h"

typedef unsigned int group_queue_size_t;

typedef struct Node {
  GroupProof group_proof;
  struct Node *next;
} Node;

typedef struct GroupQueue {
  Node *head;
  Node *end;

  group_queue_size_t size;
} GroupQueue;

void group_queue_init(GroupQueue *q);
void group_queue_destroy(GroupQueue *q);
Node *group_node_create(GroupProof *gp, Node *next);
Node *group_queue_append(GroupQueue *q, GroupProof *gp);
boolean group_queue_pop(GroupQueue *q, GroupProof *gp);
group_queue_size_t group_queue_size(GroupQueue *q);

#endif
