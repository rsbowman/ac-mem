#include "disk_list.h"
#include "array_queue.h"

#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void group_disk_list_node_save(GroupDiskListNode* node,
			       GroupArrayQueue* gaq){
  int fd;
  GroupProof g;

  fd = open(node->filename,O_WRONLY|O_TRUNC|O_CREAT,0644);
  if (fd <= 0){
    printf("%s\n",strerror(errno));
  }
  assert(fd >= 0);

  while(group_array_queue_pop(gaq,&g)) //this could be just two writes
    assert(sizeof(GroupProof) == write(fd,&g,sizeof(GroupProof)));

  close(fd);

  gaq->size = 0;
  gaq->head = 0;
  gaq->tail = 0;

}

void group_disk_list_node_load(GroupDiskListNode* node,
			       GroupArrayQueue* gaq){
  int fd,size;
  GroupProof g;

  gaq->size = 0;
  gaq->head = 0;
  gaq->tail = 0;

  fd = open(node->filename, O_RDONLY);
  assert(fd >= 0);

  while(1){
    size = read(fd, &g, sizeof(GroupProof));

    if(size == 0)
      break;

    assert(size == sizeof(GroupProof));

    group_array_queue_push(gaq, &g);
  }

  close(fd);

  unlink(node->filename);
}

void group_disk_list_init(GroupDiskList* gdl,char* prefix){
  gdl->head = 0;
  gdl->tail = 0;
  gdl->size = 0;
  gdl->file = 0;
  gdl->prefix = prefix;
}

boolean group_disk_list_pop(GroupDiskList* gdl,GroupArrayQueue* gaq){
  GroupDiskListNode* node = gdl->tail;

  if(!gdl->tail)
    return False;


  if(node->next)
    node->next->prev = NULL;
  else
    gdl->head = NULL; //this is the last node

  gdl->tail = node->next;

  group_disk_list_node_load(node, gaq);

  free(node);

  gdl->size -= 1;

  return True;
}

void group_disk_list_push(GroupDiskList* gdl,GroupArrayQueue* gaq){
  GroupDiskListNode* node
    = (GroupDiskListNode*) malloc(sizeof(GroupDiskListNode));

  assert(node);

  sprintf(node->filename,"%s%08x",gdl->prefix,gdl->file++);

  group_disk_list_node_save(node,gaq);

  node->next = NULL;
  node->prev = gdl->head;

  if(gdl->head)
    gdl->head->next = node;
  else
    gdl->tail = node;

  gdl->head = node;

  gdl->size += 1;
}

unsigned int group_disk_list_size(GroupDiskList* gdl){
  return gdl->size;
}
