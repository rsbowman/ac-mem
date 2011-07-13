#ifndef DISK_LIST_H

#define DISK_LIST_H

#include "array_queue.h"

typedef struct GroupDiskListNode{
  struct GroupDiskListNode* next;
  struct GroupDiskListNode* prev;
  char filename[100];
}GroupDiskListNode;


typedef struct GroupDiskList{
  GroupDiskListNode* head;
  GroupDiskListNode* tail;
  unsigned int size;
  char* prefix;
  unsigned int file;
}GroupDiskList;


void group_disk_list_node_save(GroupDiskListNode* node,
			       GroupArrayQueue* gaq);
void group_disk_list_node_load(GroupDiskListNode* node,
			       GroupArrayQueue* gaq);

void group_disk_list_init(GroupDiskList* gdl,char* prefix);
boolean group_disk_list_pop(GroupDiskList* gdl,GroupArrayQueue* gaq);
void group_disk_list_push(GroupDiskList* gdl,GroupArrayQueue* gaq);

unsigned int group_disk_list_size(GroupDiskList* gdl);


#endif
