#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "group.h"

/*
 * reverse bits by twos: 1100 becomes 0011, 0100 becomes 0001, etc
 */
uint64_t bitwise_reverse(uint64_t bits) {
  bits = ((bits&0x00000000ffffffffLL) <<  32) |
    ((bits&0xffffffff00000000LL) >> 32);
  bits = ((bits&0x0000ffff0000ffffLL) <<  16) |
    ((bits&0xffff0000ffff0000LL) >> 16);
  bits = ((bits&0x00ff00ff00ff00ffLL) <<   8) |
    ((bits&0xff00ff00ff00ff00LL) >>  8);
  bits = ((bits&0x0f0f0f0f0f0f0f0fLL) <<   4) |
    ((bits&0xf0f0f0f0f0f0f0f0LL) >>  4);
  bits = ((bits&0x3333333333333333LL) <<   2) |
    ((bits&0xccccccccccccccccLL) >>  2);
  /*bits = ((bits&0x5555555555555555LL) <<   1) |
    ((bits&0xaaaaaaaaaaaaaaaaLL) >>  1); */
  return bits;
}


Relator relator_from_string(const char *r) {
  int i = 0;
  Relator rel1 = 0;

  for (i = 0; i < 32 && r[i]; ++i) {
    switch (r[i]) {
    case 'a': rel1 |= (gen_a << (2*i)); break;
    case 'b': rel1 |= (gen_b << (2*i)); break;
    case 'A': rel1 |= (gen_ainv << (2*i)); break;
    case 'B': rel1 |= (gen_binv << (2*i)); break;
    }
  }
  return rel1;
}

/* r1 & r2 should be strings in {a, b, A, B}
 */
void group_init_from_string(Group *g, const char *r1, const char *r2) {
  g->relator[0] = relator_from_string(r1);
  g->len[0] = strlen(r1);

  g->relator[1] = relator_from_string(r2);
  g->len[1] = strlen(r2);
}

void group_init(Group *g) {
  g->relator[0] = 0; g->relator[1] = 0;
  g->len[0] = 0; g->len[1] = 0;
}

void group_proof_init(GroupProof *gp) {
  group_init(&gp->group);
  memset(gp->proof, 0, sizeof(Proof));
}

void group_copy(Group *dest, Group *src) {
  dest->relator[0] = src->relator[0];
  dest->relator[1] = src->relator[1];
  dest->len[0] = src->len[0];
  dest->len[1] = src->len[1];
}

void group_proof_copy(GroupProof *dest, GroupProof *src) {
  group_copy(&dest->group, &src->group);
  memccpy(dest->proof, src->proof, PROOF_END, sizeof(Proof));
}

boolean group_equals(Group *g1, Group *g2) {
  return (g1->len[0] == g2->len[0] &&
	  g1->len[1] == g2->len[1] &&
	  g1->relator[0] == g2->relator[0] &&
	  g1->relator[1] == g2->relator[1]);
}

void group_print(Group *g) {
  printf("<a, b: ");
  relator_print(g, 0);
  printf(", ");
  relator_print(g, 1);
  printf(">\n");
}

void relator_print(Group *g, int rel) {
  int i;
  for (i = 0; i < g->len[rel]; ++i) {
    switch ((g->relator[rel] >> 2*i) & 3) {
    case gen_a: printf("a"); break;
    case gen_ainv: printf("A"); break;
    case gen_b: printf("b"); break;
    case gen_binv: printf("B"); break;
    default: printf("X"); break;
    }
  }
}

int relator_length(Group *g, int relator) {
  return g->len[relator];
}

void relator_reduce(Group *g, int relator) {
  Relator r = g->relator[relator];
  int len = g->len[relator];
  unsigned int l1, l2;
  int newlen = len;
  int i = 0;

  while (i < newlen - 1) {
    l1 = LETTER_AT(r, i);
    l2 = LETTER_AT(r, i+1);
    if (l1 == GEN_INV(l2)) {
      /* shift the bits of r above l1 and l2 right to kill l1 and l2 */
      r = ((r >> 4) & (~ONES(2*i))) |
	(r & ONES(2*i));
      newlen -= 2;
      if (i > 0)
	i -= 2;
      else
	i -= 1;
    }
    i++;
  }

  if (newlen == 0) {
    printf("aargh! ");
    group_print(g);
    printf("\n");
  }

  //assert(newlen > 0);

  g->relator[relator] = r;
  g->len[relator] = newlen;
}

void relator_cyclic_reduce(Group *g, int relator) {
  relator_reduce(g, relator);
  relator_cyclic_reduce_only(g, relator);
}

void relator_cyclic_reduce_only(Group *g, int relator) {
  Relator r;
  int len;
  int l1, l2;

  r = g->relator[relator];
  len = g->len[relator];

  while (len > 1) {
    l1 = r & 3;
    l2 = LETTER_AT(r, len-1);
    // XXX::
    if (l1 == (int)GEN_INV(l2)) {
      r >>= 2;  // get rid of 1st letter
      r &= ONES(2*(len - 2)); // get rid of last letter
      len -= 2;
    } else {
      break;
    }
  }
  if (len < 1) {
    printf("different aarg!");
    group_print(g);
  }

  g->relator[relator] = r;
  g->len[relator] = len;
}

/*
 * right rotate: abc becomes cab after one rotation
 */
void group_cyclic_permute(Group *g, int relator, int amount) {
  int len = g->len[relator];
  Relator r = g->relator[relator],
    newr = 0;
  newr |= (r >> (2*amount));
  newr |= (r & ONES(2*amount)) << (2*(len - amount));

  g->relator[relator] = newr;
}

boolean group_conjugate(Group *g, int relator, int generator) {
  if (g->len[relator] + 2 > MAX_RELATOR_SIZE)
    return False;

  g->relator[relator] <<= 2;
  g->relator[relator] |= generator;
  g->relator[relator] |= (GEN_INV(generator) << (2*g->len[relator] + 2));

  g->len[relator] += 2;

  return True;
}

/* r1 = r1*r2 */
boolean group_compose(Group *g, int rel_index1, int rel_index2) {
  Relator r1, r2;
  int len1, len2;
  int i, j;

  if (g->len[0] + g->len[1] > MAX_RELATOR_SIZE)
    return False;

  r1 = g->relator[rel_index1];
  r2 = g->relator[rel_index2];

  len1 = g->len[rel_index1];
  len2 = g->len[rel_index2];

  i = len1 - 1;
  j = 0;
  while (i >= 0 && j < len2 &&
	 IS_INVERSE(LETTER_AT(r1, i), LETTER_AT(r2, j))) {
    i--;
    j++;
  }

  r2 >>= 2*j;
  len1 -= j;
  len2 -= j;

  g->relator[rel_index1] = (r1 & ONES(len1*2)) | (r2 << (2*len1));
  g->len[rel_index1] = len1 + len2;

  relator_cyclic_reduce_only(g, rel_index1);

  return True;
}

boolean group_invert(Group *g, int rel) {
  g->relator[rel] = ~g->relator[rel];
  g->relator[rel] = bitwise_reverse(g->relator[rel]);
  g->relator[rel] >>= 2*(MAX_RELATOR_SIZE - g->len[rel]);
  return True;
}

/*
 * compare the cyclic permutation of r starting at ofs1 to
 * the one starting at ofs2; return 1 if the first is lexicographically
 * greater, -1 if lesser, and zero otherwise
 */
int relator_compare_inplace(Relator r, int length, int ofs1, int ofs2) {
  int i;
  unsigned int w1, w2;

  for (i = 0; i < length; ++i) {
    w1 = LETTER_AT(r, (ofs1 + i) % length);
    w2 = LETTER_AT(r, (ofs2 + i) % length);
    if (w1 > w2)
      return 1;
    if (w1 < w2)
      return -1;
  }
  return 0;
}

void relator_canonicalize(Group *g, int relator) {
  int i, max = 0;
  Relator r;
  int len;

  relator_cyclic_reduce_only(g, relator);

  r = g->relator[relator];
  len = g->len[relator];

  // make things a bit faster when there's an "A" (=11) in the relator
  /*
  for (i = 0; i < len; ++i) {
    if (LETTER_AT(r, i) == gen_ainv) {
      max = i;
      break;
    }
    } */

  for (i = 1; i < len; ++i) {
    if (relator_compare_inplace(r, len, i, max) == 1)
      max = i;
  }

  group_cyclic_permute(g, relator, max);
}

boolean relator_lt(Relator r1, int len1, Relator r2, int len2) {
  int i;
  int v1, v2;
  for (i = 0; i < MIN(len1, len2); ++i) {
    v1 = (r1 >> (2*i)) & ONES(2);
    v2 = (r2 >> (2*i)) & ONES(2);
    if (v1 < v2)
      return True;
    if (v1 > v2)
      return False;
  }
  if (len1 < len2)
    return True;
  else
    return False;
}

void group_canonicalize(Group *g) {
  Relator r1, r2, temp;
  unsigned char temp_len;

  relator_canonicalize(g, 0);
  relator_canonicalize(g, 1);
  r1 = g->relator[0];
  r2 = g->relator[1];

  /* put the bigger one first */
  if (relator_lt(r1, g->len[0], r2, g->len[1])) {
    temp = r1;
    r1 = r2;
    r2 = temp;

    temp_len = g->len[0];
    g->len[0] = g->len[1];
    g->len[1] = temp_len;
  }
  g->relator[0] = r1;
  g->relator[1] = r2;
}

void group_proof_append(GroupProof *gp, unsigned char op,
			unsigned char op1, unsigned char op2) {
  int i;
  unsigned char *p = gp->proof;

  for(i=0; i<MAX_PROOF_LENGTH; i += 3)
    if(p[i] == 0 || p[i] == PROOF_END)
      break;

  // assert(i<MAX_PROOF_LENGTH);
  // if we've filled up the proof, just stop.
  if (i >= MAX_PROOF_LENGTH)
    return;

  p[i] = op;
  p[i+1] = op1;
  p[i+2] = op2;

  if (i + 3 < MAX_PROOF_LENGTH)
    p[i + 3] = PROOF_END;
}

void group_proof_print(GroupProof *gp, Group *start) {
  int i;
  unsigned char op, op1, op2;
  Group g;

  group_copy(&g, start);

  group_print(&g);
  for (i = 0; i < group_proof_length(gp); ++i) {
    group_proof_op(gp, i, &op, &op1, &op2);
    switch (op) {
    case GP_COM:
      printf("compose   %d  %d -> ", op1, op2);
      group_compose(&g, op1, op2);
      break;
    case GP_INV:
      printf("invert    %d    -> ", op1);
      group_invert(&g, op1);
      break;
    case GP_CONJ:
      printf("conjugate %d %d  -> ", op1, op2);
      group_conjugate(&g, op1, op2);
      relator_reduce(&g, op1);
      break;
    case GP_CP:
      printf("cyc perm  %d %2d -> ", op1, op2);
      group_cyclic_permute(&g, 0, op1);
      group_cyclic_permute(&g, 1, op2);
      break;
    default: printf("???\n");
    }
    group_print(&g);
  }
  group_print(&gp->group);
}

int group_proof_length(GroupProof *gp) {
  int i;
  unsigned char* p = gp->proof;
  for (i = 0; i < MAX_PROOF_LENGTH; i += 3) {
    if (p[i] == 0 || p[i] == PROOF_END)
      break;
  }
  return i/3;
}

void group_proof_last_op(GroupProof *gp, unsigned char *op,
			 unsigned char *op1, unsigned char *op2) {
  int len = group_proof_length(gp);
  if (len > 0)
    group_proof_op(gp, len - 1, op, op1, op2);
}

void group_proof_op(GroupProof *gp, int index, unsigned char *op,
		    unsigned char *op1, unsigned char *op2) {
  int len = group_proof_length(gp);
  unsigned char *p = gp->proof;

  assert(0 <= index && index < len);

  *op  = p[index * 3];
  *op1 = p[index * 3 + 1];
  *op2 = p[index * 3 + 2];
}
