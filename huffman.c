
#include "binaryTree.h"
#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void bVInit(Bitvector *b) {
	b->len = 0;
	b->curr = 0;
	memset(b->bV, 0, 256);
}
void pushBit(void *x, void *y) {
	Bitvector *b = (Bitvector *) x;
	uint8_t bit = *((uint8_t *) y);
	if (b && bit <= 1) {
		b->bV[b->len] |= (bit << b->curr++);
		if (b->curr >> 3) {
			b->curr = 0;
			if (++b->len > 256) {
				puts("buffer overflow");
				exit(-1);
			}
		}
	}
	else {
		puts("Error: pushBit()");
		exit(-1);
	}
}
void popBit(Bitvector *b) {
	if (b && (b->curr || b->len)) {
		if (b->curr--) {
			b->bV[b->len] &= ~(0x1 << (b->curr));
		}
		else {
			b->curr = 7;
			b->bV[b->len--] &= ~(0x1 << (b->curr));
		}
	}
}
uint8_t getBit(Bitvector *b, uint32_t bit) {
	if (b && (b->curr || b->len) && bit / 8 <= b->len) {
		uint8_t bitSet = bit / 8;
		uint8_t bitShift = bit - (bitSet << 3);
		return (b->bV[bitSet] & (0x1 << bitShift)) >> bitShift;
	}
	return 2;
}
void forEachBit(Bitvector *b, Bitvector *c, Node *n, void (*ftn)(void *a, void *b)) {
	if (b) {
		uint8_t mask = 0x1;
		void *which = NULL;
		if (c) {
			which = &c[n->symbol];
		}
		if (!b->len) {
			for (uint8_t i = 0; i < b->curr; i++) {
				uint32_t val = (b->bV[0] & mask) >> i;
				ftn(which, &val);
				mask <<= 0x1;
			}
			if (!c) {
				printf("\n");
			}
		}
		else {
			for (uint32_t i = 0; i <= b->len; i++) {
				if (i == b->len) {
					for (uint8_t j = 0; j < b->curr; j++) {
						uint32_t val = (b->bV[i] & mask) >> j;
						ftn(which, &val);
						mask <<= 0x1;
					}
				}
				else {
					for (uint8_t j = 0; j < 8; j++) {
						uint32_t val = (b->bV[i] & mask) >> j;
						ftn(which, &val);
						mask <<= 0x1;
					}
				}
				if (!c) {
					printf(" ");
				}
				mask = 0x1;
			}
			if (!c) {
				printf("\n");
			}
		}
	}
}
void print(void *a, void *b) {
	(void)(a);
	printf("%u", *((uint32_t *) b));
}
void printBV(Bitvector *b) {
	forEachBit(b, NULL, NULL, print);
}
void printCodes(Bitvector *codes) {
	for (uint8_t i = 0; i < 255; i++) {
		if (codes[i].len || codes[i].curr) {
			if (i == ' ') {
				printf("SPACE: ");
			}
			else if (i == '\n') {
				printf("NL: ");
			}
			else if (i == 'r') {
				printf("CR: ");
			}
			else if (i == '\t') {
				printf("TAB: ");
			}
			else if (i > 126) {
				printf("%u: ", i);
			}
			else {
				printf("%c: ", i);
			}
			printBV(&codes[i]);
		}
	}
}
void getHuffmanCodes(Node *n, Bitvector c, Bitvector *b, uint32_t *leaves) {
	if (n && b) {
		uint8_t bit;
		if (n->left) {
			bit = 0;
			pushBit(&c, &bit);
			getHuffmanCodes(n->left, c, b, leaves);
			if (n->right) {
				popBit(&c);
				bit = 1;
				pushBit((void *) &c, (void *) &bit);
				getHuffmanCodes(n->right, c, b, leaves);
			}
		}
		else if (n->right) {
			bit = 1;
			pushBit(&c, &bit);
			getHuffmanCodes(n->left, c, b, leaves);
			if (n->left) {
				popBit(&c);
				bit = 0;
				pushBit(&c, &bit);
				getHuffmanCodes(n->right, c, b, leaves);
			}
		}
		// Is a leaf, save code
		else {
			*leaves += 1;
			/* Save current bit path to codes array */
			forEachBit(&c, b, n, pushBit);
		}
	}
}
void printTreeToFile(FILE *fd, Node *n, uint32_t *fileSize) {
	if (fd && n) {
		printTreeToFile(fd, n->left, fileSize);
		printTreeToFile(fd, n->right, fileSize);
		uint8_t leaf = 'L', internal = 'I';
		if (!(n->left || n->right)) {
			if (!fwrite(&leaf, 1, 1, fd)) {
				puts("printTreeToFile fwrite() fail");
				exit(-1);
			}
			if (!fwrite(&n->symbol, 1, 1, fd)) {
				puts("printTreeToFile fwrite() fail");
				exit(-1);
			}
			*fileSize += 2;
		}
		else {
			if (!fwrite(&internal, 1, 1, fd)) {
				puts("printTreeToFile fwrite() fail");
				exit(-1);
			}
			*fileSize += 1;
		}
	}
}
