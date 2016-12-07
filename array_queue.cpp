
#include <stdlib.h>
#include <assert.h>

#include "disk_queue.h"

void group_array_queue_init(GroupArrayQueue* gq,unsigned max_size){
  gq->size = 0;
  gq->max_size = max_size;
  gq->queue = (GroupProof*) malloc(max_size*sizeof(GroupProof));
  gq->head = 0;
  gq->tail = 0;
}

boolean group_array_queue_pop(GroupArrayQueue* gq,GroupProof* g){
  if(gq->tail == gq->head)
    return False;

  *g = gq->queue[gq->tail];

  gq->tail = (gq->tail+1) % gq->max_size;

  gq->size -= 1;

  return True;
}

boolean group_array_queue_push(GroupArrayQueue* gq, GroupProof* g){
  if((gq->head+1)%gq->max_size == gq->tail)
    return False;

  gq->queue[gq->head] = *g;

  gq->head = (gq->head+1)%gq->max_size;

  gq->size += 1;

  return True;
}

int group_array_queue_size(GroupArrayQueue* gq){
  return gq->size;
}
