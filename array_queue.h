#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H

#include "group.h"

typedef struct GroupArrayQueue {
  GroupProof* queue;
  unsigned int head;
  unsigned int tail;
  unsigned int size;
  unsigned int max_size;
}GroupArrayQueue;


void group_array_queue_init(GroupArrayQueue* g,unsigned max_size);
int group_array_queue_size(GroupArrayQueue* gq);
boolean group_array_queue_pop(GroupArrayQueue* gq,GroupProof* g);
boolean group_array_queue_push(GroupArrayQueue* gq, GroupProof* g);

#endif
