#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "bfs.h"
#include "group.h"
#include "disk_queue.h"
#include "hashtable.h"
#include "ord_tup.h"

/*
 * append all cyclic permutations of both relators
 */
int append_cyclic_permutations(GroupQueue *q, GroupProof *gp) {
  GroupProof new, permuted;
  int i, j;
  int len1 = relator_length(&gp->group, 0),
    len2 = relator_length(&gp->group, 1);

  group_proof_copy(&permuted, gp);
  for (i = 0; i < len1; ++i) {
    for (j = 0; j < len2; ++j) {
      if (i == 0 && j == 0)
	continue;
      group_proof_copy(&new, &permuted);
      group_cyclic_permute(&new.group, 1, j);
      group_proof_append(&new, GP_CP, i, j);
      group_queue_push(q, &new);
    }
    group_cyclic_permute(&permuted.group, 0, 1);
  }
  return len1*len2;
}

int apply_transforms(GroupQueue *q, GroupProof *gp, GroupProof *start,
		     Hashtable *ht, int max_length) {
  GroupProof new;
  int discarded = 0;
  unsigned char op, op1, op2;

  group_proof_copy(&new, gp);
  if (group_compose(&new.group, 0, 1)) {
    if (relator_length(&new.group, 0) < max_length &&
	hashtable_add(ht, &new.group)) {
      group_proof_append(&new, GP_COM, 0, 1);
      group_queue_push(q, &new);
    } else {
      discarded++;
    }
  }
  group_proof_copy(&new, gp);
  if (group_compose(&new.group, 1, 0)) {
    if (relator_length(&new.group, 1) < max_length &&
	hashtable_add(ht, &new.group)) {
      group_proof_append(&new, GP_COM, 1, 0);
      group_queue_push(q, &new);
    } else {
      discarded++;
    }
  }
  group_proof_last_op(gp, &op, &op1, &op2);

  if (op != GP_CP) {
    group_proof_copy(&new, gp);
    if (group_invert(&new.group, 0)) {
      if (hashtable_add(ht, &new.group)) {
	group_proof_append(&new, GP_INV, 0, 0);
	group_queue_push(q, &new);
      } else {
	discarded++;
      }
    }
    group_proof_copy(&new, gp);
    if (group_invert(&new.group, 1)) {
      if (hashtable_add(ht, &new.group)) {
	group_proof_append(&new, GP_INV, 1, 0);
	group_queue_push(q, &new);
      } else {
	discarded++;
      }
    }

    append_cyclic_permutations(q, gp);
  }

  return discarded;
}


boolean is_interesting_presentation(GroupProof *gp,
				    GroupProof *start) {
  return (gp->group.len[0] == 1 ||
	  gp->group.len[1] == 1);
}

time_t t_start, t_end;

void print_stats(uint64_t popped, uint64_t discarded, int depth,
		 GroupQueue *q, Hashtable *h, GroupProof *smallest) {
  t_end = time(0);
  printf("time %5d, popped rate %5d/s\n", (int)(t_end-t_start),
	 (int)((float)popped/(float)(t_end-t_start)));
  printf("depth %d, popped %lld, discarded %lld, queue size %d\n",
	 depth, (long long int)popped, (long long int)discarded,
         group_queue_size(q));
  printf("hashtable size %ld, adds %ld, discards %ld\n",
	 hashtable_size(h), h->adds, h->discards);
  printf("smallest seen: ");
  group_print(&smallest->group); printf("\n");
  fflush(stdout);
}

boolean search(GroupProof *start, int max_relator_length,
	       int queue_buffer_size, char* queue_file_prefix) {
  uint64_t popped = 0, discarded = 0;
  GroupQueue q;
  GroupProof gp;
  GroupProof smallest;
  Hashtable ht;
  int depth = 0, rlen,
    min_relator_length = relator_length(&start->group, 0) +
    relator_length(&start->group, 1);

  hashtable_init(&ht);
  hashtable_add(&ht, &start->group);

  group_proof_copy(&smallest, start);

  group_queue_init(&q, queue_buffer_size, queue_file_prefix);
  group_queue_push(&q, start);

  t_start = time(0);

  while (group_queue_size(&q) > 0) {
    group_queue_pop(&q, &gp);

    rlen = relator_length(&gp.group, 0) +
      relator_length(&gp.group, 1);
    if (rlen < min_relator_length) {
      group_proof_copy(&smallest, &gp);
      min_relator_length = rlen;
      //printf("small group found: \n");
      //group_proof_print(&smallest, &start->group);
    }
    popped++;
    depth = group_proof_length(&gp);

    if (is_interesting_presentation(&gp, start)) {
      printf("something interesting:\n");
      group_proof_print(&gp, &start->group);
      return True;
    }
    discarded += apply_transforms(&q, &gp, start, &ht, max_relator_length);

    if (!(popped % 1000000))
      print_stats(popped, discarded, depth, &q, &ht, &smallest);
    //if (popped == 10000000)
    //return False;
  }
  return False;
}
