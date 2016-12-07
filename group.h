#ifndef GROUP_H
#define GROUP_H

#include <stdint.h>

typedef int boolean;
#define True 1
#define False 0

#define MIN(x, y) (((x)<(y))?(x):(y))
/*
 * Groups specialized to two generators, two relators
 */
typedef uint64_t Relator;

#define MAX_RELATOR_SIZE 32 

#define gen_a (Relator)0
/* gen_ainv is ((~gen_a) & 3) */
#define gen_ainv (Relator)3
#define gen_b (Relator)1
/* sim. for gen_binv */
#define gen_binv (Relator)2

/* 2**n - 1, for masks and such */
#define ONES(n) ((1LL << (n)) - 1)
#define LETTER_AT(g,index) (((g)>>(2*(index)))&ONES(2))

#define GEN_INV(g) (Relator)((~g)&3)
#define IS_INVERSE(g1,g2) (((g1)^(g2))==3)

typedef struct Group {
  Relator relator[2];    /* relators */
  unsigned char len[2];  /* lengths of the relators */
} Group;

// 3*78 makes sizeof(GroupProof) 256
// 3*35+1 makes sizeof(GroupProof) 128
#define MAX_PROOF_LENGTH (3*35)

typedef unsigned char Proof[MAX_PROOF_LENGTH];

// opcodes for proofs
#define GP_COM 1
#define GP_INV 2
#define GP_CONJ 3
#define GP_CP 4

#define PROOF_END 66

/* a group together with a sequence of moves it took to get here */
typedef struct GroupProof {
  Group group;
  Proof proof;
  char filler[1];
} GroupProof;

uint64_t bitwise_reverse(uint64_t);

void group_init_from_string(Group *g, const char *r1, const char *r2);
Relator relator_from_string(const char *r);
void group_init(Group *g);
void group_proof_init(GroupProof *gp);
void group_print(Group *g);
void relator_print(Group *g, int rel);
void group_copy(Group *dest, Group *src);
void group_proof_copy(GroupProof *desg, GroupProof *src);
boolean group_equals(Group *g1, Group *g2);

int relator_length(Group *g, int relator);
void relator_reduce(Group *g, int relator);
void relator_cyclic_reduce(Group *g, int relator);
void relator_cyclic_reduce_only(Group *g, int relator);

boolean group_conjugate(Group *g, int relator, int generator);
boolean group_compose(Group *g, int r1, int r2);
boolean group_invert(Group *g, int rel);
void group_cyclic_permute(Group *g, int relator, int amount);

boolean relator_lt(Relator r1, int len1, Relator r2, int len2);
int relator_compare_inplace(Relator r, int length, int ofs1, int ofs2);
void group_canonicalize(Group *g);
void relator_canonicalize(Group *g, int relator);

void group_proof_append(GroupProof *gp, unsigned char op,
			unsigned char op1, unsigned char op2);
void group_proof_print(GroupProof *gp, Group *start);
int group_proof_length(GroupProof *gp);
void group_proof_op(GroupProof *gp, int index, unsigned char *op,
		    unsigned char *op1, unsigned char *op2);
void group_proof_last_op(GroupProof *gp, unsigned char *op,
			 unsigned char *op1, unsigned char *op2);
#endif
