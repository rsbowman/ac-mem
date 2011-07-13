#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "queue.h"

void group_queue_init(GroupQueue *q) {
  q->head = q->end = NULL;
  q->size = 0;
}

void group_queue_destroy(GroupQueue *q) {
  Node *current, *tofree;
  
  if (q == NULL)
    return;
  
  current = q->head;
  while (current != NULL) {
    tofree = current;
    current = current->next;
    free(tofree);
  }

  q->head = q->end = NULL;
  q->size = 0;
}

Node *group_node_create(GroupProof *gp, Node *next) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  assert(new_node != NULL);
  group_proof_copy(&new_node->group_proof, gp);
  new_node->next = next;
  return new_node;
}

Node *group_queue_append(GroupQueue *q, GroupProof *gp) {
  Node *new_node = group_node_create(gp, 0);

  if (!q->head) {
    q->head = q->end = new_node;
  } else {
    q->end->next = new_node;
    q->end = new_node;
  }
  q->size++;

  return q->end;
}

boolean group_queue_pop(GroupQueue *q, GroupProof *gp) {
  Node *n;
  if (q->size > 0) {
    n = q->head;
    q->head = q->head->next;
    group_proof_copy(gp, &n->group_proof);
    free(n);
    q->size--;
    return True;
  }
  return False;
}

group_queue_size_t group_queue_size(GroupQueue *q) {
  return q->size;
}
