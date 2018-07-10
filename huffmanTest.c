
#include "huffman.h"
#include <stdio.h>

int main() {
	Bitvector b;
	bVInit(&b);
	srand(1);
	uint8_t bit;
	for (int i = 0; i < 50; i++) {
		bit = rand() % 2;
		pushBit(&b, &bit);
		printBV(&b);
	}
	for (int i = 0; i < 52; i++) {
		popBit(&b);
		printBV(&b);
	}
	for (int i = 0; i < 50; i++) {
		bit = rand() % 2;
		pushBit(&b, &bit);
		printBV(&b);
	}
	puts("getBits()");
	for (uint32_t i = 0; i < (b.len << 3) + b.curr; i++) {
		printf("%u", getBit(&b, i));
		if (!((i + 1) % 8)) {
			printf(" ");
		}
	}
	return 0;
}
