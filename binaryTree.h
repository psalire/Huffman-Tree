
#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H
#include <stdlib.h>
#include <stdint.h>

typedef struct node Node;
struct node { // node for both BT and LL
	uint64_t val;
	uint32_t index;
	uint8_t symbol;
	Node *left, *right, *next;
};
typedef struct BTree {
	Node *root;
} BTree;

BTree *newBTree();
Node *newNode(uint64_t, uint8_t);
Node *join(Node *, Node *);
Node *BTgetParent(Node *, Node *);
void BTswapEnd(BTree *);
void BTswapParent(BTree *, Node *);
void freeBTree(Node *);
void printNode(Node *);
void printTree(Node *, uint32_t);

#endif
