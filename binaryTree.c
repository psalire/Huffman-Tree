
#include "binaryTree.h"
#include <stdio.h>

BTree *newBTree() {
	BTree *b = (BTree *) malloc(sizeof(BTree));
	b->root = NULL;
	return b;
}
Node *newNode(uint64_t val, uint8_t sym) {
	Node *new = (Node *) malloc(sizeof(Node));
	if (new) {
		new->val = val;
		new->symbol = sym;
		new->index = 0; // for PQ
		new->next = NULL; // for PQ
		new->left = NULL;
		new->right = NULL;
	}
	else {
		puts("newNode() malloc fail");
		exit(-1);
	}
	return new;
}
Node *join(Node *l, Node *r) {
	Node *j;
	if (l && r) {
		j = newNode(l->val + r->val, '$');
	}
	else if (l) {
		j = newNode(l->val, '$');
	}
	else if (r) {
		j = newNode(r->val, '$');
	}
	else {
		j = newNode(0, '$');
	}
	j->left = l;
	j->right = r;
	return j;
}
Node *BTgetParent(Node *r, Node *n) {
	if (r && n) {
		if (r->left == n || r->right == n) {
			return r;
		}
		Node *tL = BTgetParent(r->left, n);
		if (tL) {
			return tL;
		}
		return BTgetParent(r->right, n);
	}
	else {
		return NULL;
	}
}
void BTswapParent(BTree *b, Node *n) {
	if (b && n) {
		Node *r = b->root;
		if (r) {
			Node *tP = BTgetParent(r, n);
			if (tP) {
				Node *tG = BTgetParent(r, tP);
				uint8_t dir = 0;
				if (tG) {
					if (tG->left == tP) {
						dir = 'l';
					}
					else if (tG->right == tP) {
						dir = 'r';
					}
				}
				Node *tL = n->left;
				Node *tR = n->right;
				if (tP->left == n) {
					Node *tPR = tP->right;
					tP->left = tL;
					tP->right = tR;
					n->left = tP;
					n->right = tPR;
					if (dir == 'l') {
						tG->left = n;
					}
					else if (dir == 'r') {
						tG->right = n;
					}
				}
				else if (tP->right == n) {
					puts("r");
					Node *tPL = tP->left;
					tP->left = tL;
					tP->right = tR;
					n->right = tP;
					n->left = tPL;
					if (dir == 'l') {
						tG->left = n;
					}
					else if (dir == 'r') {
						tG->right = n;
					}
				}
				if (!BTgetParent(r, n)) {
					b->root = n;
				}
			}
		}
	}
}
void BTswapEnd(BTree *h) {
	if (h) {
		Node *p = h->root;
		for (; p->left != NULL; p = p->left); // lowest level always starts left
		Node *pP = BTgetParent(h->root, p);
		if (pP) {
			pP->left = h->root;
			p->left = h->root->left;
			p->right = h->root->right;
			h->root->left = NULL;
			h->root->right = NULL;
			h->root = p;
		}
	}
}
void freeBTree(Node *r) {
	if (r) {
		freeBTree(r->left);
		freeBTree(r->right);
		free(r);
		r = NULL;
	}
}
void printNode(Node *e) {
	if (e->symbol < 32 || e->symbol > 126) {
		printf("(%lu, %u)\n", e->val, e->symbol);
	}
	switch(e->symbol) {
		case ' ': {
			printf("(%lu, SPACE)\n", e->val);
			break;
		}
		case '\n': {
			printf("(%lu, NL)\n", e->val);
			break;
		}
		case '\r': {
			printf("(%lu, CR)\n", e->val);
			break;
		}
		case '\t': {
			printf("(%lu, TAB)\n", e->val);
			break;
		}
		default:{
			printf("(%lu, %c)\n", e->val, e->symbol);
			break;
		}
	}
}
void printTree(Node *r, uint32_t level) {
	if (r) {
		printTree(r->right, level + 1);
		for (uint32_t i = 0; i < level; i++) {
			printf("-");
		}
		printNode(r);
		printTree(r->left, level + 1);
	}
}
