#ifndef BFS_H
#define BFS_H

#include "group.h"
#include "disk_queue.h"
#include "hashtable.h"

boolean is_interesting_presentation(GroupProof *gp, GroupProof *start);
boolean search(GroupProof *start, int max_relator_length,
	       int queue_buffer_size, char* queue_prefix);
int apply_transforms(GroupQueue *q, GroupProof *gp, GroupProof *start,
		      Hashtable *ht, int max_length);

#endif
