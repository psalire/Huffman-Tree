
#include "binaryTree.h"
#include <stdio.h>

int main() {
	BTree *b = newBTree();
	b->root  = newNode(0, '$');
	b->root->left = newNode(1, '$');
	b->root->right = newNode(2, '$');
	Node *tL = b->root->left;
	Node *tR = b->root->right;
	tL->left = newNode(3, '$');
	tL->right = newNode(4, '$');
	tR->left = newNode(5, '$');
	tR->right = newNode(6, '$');
	printTree(b->root, 0);
	puts("------------");
	puts("swap 3 & 1");
	puts("------------");
	BTswapParent(b, tL->left);
	printTree(b->root, 0);
	puts("swap back");
	puts("------------");
	BTswapParent(b, b->root->left->left);
	printTree(b->root, 0);
	puts("swap 0 & 1");
	puts("------------");
	BTswapParent(b, b->root->left);
	printTree(b->root, 0);
	puts("swap 3 & 0");
	puts("------------");
	BTswapParent(b, b->root->left->left);
	printTree(b->root, 0);
	puts("swap 3 & 4");
	puts("------------");
	BTswapParent(b, b->root->left->right);
	printTree(b->root, 0);
	puts("swap 2 & 6");
	puts("------------");
	BTswapParent(b, b->root->right->right);
	printTree(b->root, 0);
	puts("swap 6 & 5");
	puts("------------");
	BTswapParent(b, b->root->right->left);
	printTree(b->root, 0);
	printf("root: (%u, %c)\n", b->root->val, b->root->symbol);
	freeBTree(b->root);
	free(b);
	return 0;
}
