#include <assert.h>
#include <stdio.h>

#include "group.h"
#include "ord_tup.h"
//#include "queue.h"
#include "hashtable.h"

void test_bit_reverse() {
  assert(bitwise_reverse(1<<1) == (1LL<<63));
  assert(bitwise_reverse(1<<4) == (1LL<<58));
}

void test_group() {
  Group g;
  Relator r;

  assert(IS_INVERSE(gen_a, gen_ainv) &&
	 IS_INVERSE(gen_b, gen_binv) &&
	 IS_INVERSE(gen_ainv, gen_a) &&
	 IS_INVERSE(gen_binv, gen_b));

  r = relator_from_string("abAABa");
  assert(LETTER_AT(r, 0) == gen_a &&
	 LETTER_AT(r, 1) == gen_b &&
	 LETTER_AT(r, 2) == gen_ainv &&
	 LETTER_AT(r, 3) == gen_ainv &&
	 LETTER_AT(r, 4) == gen_binv &&
	 LETTER_AT(r, 5) == gen_a);

  group_init_from_string(&g, "aaaAA", "Bbb");
  group_print(&g);
  assert(g.len[0] == 5 && g.len[1] == 3);
  relator_reduce(&g, 0);
  group_print(&g);
  assert(g.len[0] == 1 && g.len[1] == 3);

  group_conjugate(&g, 0, gen_b);
  group_conjugate(&g, 1, gen_ainv);
  assert(g.len[0] == 3 && g.len[1] == 5);
  printf("should get <a,b: baB, ABbba> next...\n");
  group_print(&g);

  group_compose(&g, 0, 1);
  group_print(&g);
  assert(g.len[0] == 8 && g.len[1] == 5);
  printf("should get <a,b: baBABbba, ABbba>...\n");
  group_print(&g);

  group_invert(&g, 0);
  assert(g.len[0] == 8 && g.len[1] == 5);
  printf("should get <a,b: ABBbabAB, ABbba>...\n");
  group_print(&g);

  group_init_from_string(&g, "BaBBa", "abAAb");
  group_compose(&g, 1, 0);
  group_print(&g);
  assert(g.len[0] == 5 && g.len[1] == 6 &&
	 g.relator[1] == relator_from_string("abABBa"));

  group_init_from_string(&g, "abbbA", "BBabbb");
  relator_reduce(&g, 0);
  assert(g.len[0] == 5 && g.len[1] == 6);
  relator_cyclic_reduce(&g, 0);
  assert(g.len[0] == 3 && g.len[1] == 6);
  relator_cyclic_reduce(&g, 1);
  assert(g.len[0] == 3 && g.len[1] == 2);

  group_init_from_string(&g, "AabA", "b");
  relator_reduce(&g, 0);
  assert(g.len[0] == 2);
  group_init_from_string(&g, "AbaA", "b");
  relator_reduce(&g, 0);
  assert(g.len[0] == 2);
  group_init_from_string(&g, "aBBBbabABbabABbabABAA", "b");
  relator_cyclic_reduce(&g, 0);
  assert(g.len[0] == 9);
}

void test_canonical() {
  Group g, g2;
  group_init_from_string(&g, "abAb", "ba");
  group_init_from_string(&g2, "a", "b");

  assert(relator_compare_inplace(g.relator[0], g.len[0], 0, 1) == -1);
  assert(relator_compare_inplace(g.relator[0], g.len[0], 1, 2) == -1);
  assert(relator_compare_inplace(g.relator[0], g.len[0], 1, 0) == 1);
  assert(relator_compare_inplace(g.relator[0], g.len[0], 2, 1) == 1);
  assert(relator_compare_inplace(g.relator[0], g.len[0], 1, 1) == 0);

  group_init_from_string(&g, "baaaaaa", "aba");
  assert(relator_compare_inplace(g.relator[0], g.len[0], 1, 2) == -1);

  assert(g.relator[1] != gen_b);
  relator_canonicalize(&g, 1);
  assert(g.relator[1] == gen_b);

  group_init_from_string(&g, "bAbaBa", "AA");
  relator_canonicalize(&g, 0);
  assert((g.relator[0] & 3) == gen_ainv &&
	 (g.relator[0] & (3 << 2)) == (gen_b << 2) &&
	 (g.relator[0] & (3 << 4)) == (gen_a << 4) &&
	 (g.relator[0] & (3 << 6)) == (gen_binv << 6) &&
	 (g.relator[0] & (3 << 8)) == (gen_a << 8) &&
	 (g.relator[0] & (3 << 10)) == (gen_b << 10));

  group_canonicalize(&g);
  assert(g.len[0] == 2 && (g.relator[0] & (3 << 2)) == (gen_ainv << 2) &&
	 g.len[1] == 6);

  group_init_from_string(&g, "Aba", "b");
  group_canonicalize(&g);
  //group_print(&g);
  assert(g.len[0] == 1 && g.relator[0] == gen_b);
}

void test_relator_index() {
  Relator r;

  init_ordered_tuples();

  assert(compute_ordered_tuples(1, 4) == 4);
  assert(compute_ordered_tuples(2, 4) == 10);
  assert(compute_ordered_tuples(3, 4) == 20);
  assert(compute_ordered_tuples(6, 4) == 84);
  assert(compute_ordered_tuples(12, 4) == 455);

  assert(cutoff(2, 0) == 0);
  assert(cutoff(2, 1) == 1);
  assert(cutoff(2, 2) == 3);
  assert(cutoff(2, 3) == 6);
  assert(cutoff(3, 0) == 0);
  assert(cutoff(3, 2) == 4);
  assert(cutoff(5, 2) == 6);
  assert(cutoff(11, 3) == 78);

  r = 0; assert(relator_index(r, 1) == 0);
  r = 1; assert(relator_index(r, 1) == 1);
  r = 2; assert(relator_index(r, 1) == 2);
  r = 3; assert(relator_index(r, 1) == 3);
  r = ONES(10); assert(relator_index(r, 5) == 124);
  r = ONES(12); assert(relator_index(r, 6) == 208);
}

/*
void test_queue() {
  GroupProof gp, gp2;
  GroupQueue q;

  group_init_from_string(&gp.group, "a", "b");
  gp.proof = 0;

  group_queue_init(&q);
  assert(q.size == 0);
  group_queue_append(&q, &gp);
  assert(q.size == 1);
  assert(group_queue_pop(&q, &gp2));
  assert(gp2.group.len[0] == 1 && gp2.group.len[1] == 1);
  assert(q.size == 0);
  group_queue_destroy(&q);
  } */

void test_hashtable() {
  Hashtable ht;
  Group g1, g2, g3;

  hashtable_init(&ht);
  group_init_from_string(&g1, "aaBBB", "abaBAB");
  group_init_from_string(&g2, "aBB", "abaBAB");
  group_init_from_string(&g3, "ab", "a");

  assert(hashtable_size(&ht) == 0);
  assert(hashtable_add(&ht, &g1));
  assert(hashtable_contains(&ht, &g1));
  assert(!hashtable_contains(&ht, &g2));
  assert(hashtable_add(&ht, &g2));
  assert(!hashtable_add(&ht, &g1));
  assert(!hashtable_add(&ht, &g2));
  assert(!hashtable_contains(&ht, &g3));
}

void test_gpproof() {
  GroupProof gp;
  unsigned char op, op1, op2;

  group_proof_init(&gp);
  group_init_from_string(&gp.group, "aba", "b");
  group_proof_append(&gp, GP_COM, 0, 1);
  group_proof_append(&gp, GP_CONJ, 0, gen_a);
  assert(group_proof_length(&gp) == 2);
  group_proof_last_op(&gp, &op, &op1, &op2);
  assert(op == GP_CONJ && op1 == 0 && op2 == gen_a);

  printf("sizeof(GroupProof) = %zu\n", sizeof(GroupProof));
}

int main(void) {
  test_bit_reverse();
  test_group();
  test_canonical();
  test_relator_index();
  //test_queue();
  test_hashtable();
  test_gpproof();
  return 0;
}
