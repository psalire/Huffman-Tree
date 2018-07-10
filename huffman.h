
#ifndef _HUFFMAN_H
#define _HUFFMAN_H
#include <stdint.h>
#include <stdio.h>
#include "binaryTree.h"
#define MAGIC_NUMBER 0xd0d0d00d

typedef struct bitvector {
	uint8_t bV[256], curr;
	uint32_t len;
} Bitvector;

void bVInit(Bitvector *);
void pushBit(void *, void *);
void popBit(Bitvector *);
uint8_t getBit(Bitvector *, uint32_t);
void print(void *, void *);
void forEachBit(Bitvector *, Bitvector *, Node *, void (*)(void *, void *));
void printBV(Bitvector *);
void getHuffmanCodes(Node *, Bitvector, Bitvector *, uint32_t *);
void printCodes(Bitvector *);
void printTreeToFile(FILE *, Node *, uint32_t *);

#endif
