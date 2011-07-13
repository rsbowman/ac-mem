#ifndef QUEUE_H
#define QUEUE_H

#include "array_queue.h"
#include "disk_list.h"
#include "group.h"

typedef
struct GroupQueue {
  GroupArrayQueue head;
  GroupDiskList   disk;
  GroupArrayQueue tail;
  unsigned int buffer_size;
} GroupQueue;


void group_queue_init(GroupQueue* gq, 
		      unsigned int buffer_size,
		      char* prefix);
boolean group_queue_pop(GroupQueue* gq, GroupProof* g);
void group_queue_push(GroupQueue* gq,  GroupProof* g);

unsigned int group_queue_size(GroupQueue* gq);

#endif
