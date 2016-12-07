#include "disk_queue.h"


void group_queue_init(GroupQueue* gq,
		      unsigned int buffer_size,
		      char* prefix){
  gq->buffer_size = buffer_size;
  group_array_queue_init(&gq->head,buffer_size+1);
  group_disk_list_init(&gq->disk,prefix);
  group_array_queue_init(&gq->tail,buffer_size+1);
}

boolean group_queue_pop(GroupQueue* gq,GroupProof* g){

  if(group_array_queue_pop(&gq->head,g))
    return True;

  if(group_disk_list_pop(&gq->disk,&gq->head)
    && group_array_queue_pop(&gq->head,g))
     return True;

  if(group_array_queue_pop(&gq->tail,g))
    return True;

  return False;
}

void group_queue_push(GroupQueue* gq, GroupProof* g){
  if(!group_array_queue_push(&gq->tail,g)){
    group_disk_list_push(&gq->disk,&gq->tail);
    group_array_queue_push(&gq->tail,g);
  }
}


unsigned int group_queue_size(GroupQueue* gq){
  return group_array_queue_size(&gq->head)
    + group_disk_list_size(&gq->disk)*(gq->buffer_size)
    + group_array_queue_size(&gq->tail);
}
