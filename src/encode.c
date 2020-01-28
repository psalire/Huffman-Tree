#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "minheap.h"
#include "binaryTree.h"
#include "huffman.h"
#define VERBOSE_MASK 0x1
#define CODES_MASK 0x2
#define TREE_MASK 0x4
#define HISTOGRAM_MASK 0x8

static void closeAndFree(FILE *in, FILE *out, Heap *h) {
	if (in) {
		fclose(in);
	}
	if (out) {
		fclose(out);
	}
	freeBTree(h->parent);
	free(h);
}

static void printHistogram(uint32_t freq[256]) {
	for (int i = 0; i < 256; i++) {
		if (freq[i]) {
			if (i == 10) {
				printf("NL : %u\n", freq[i]);
			}
			else if (i == 13) {
				printf("CR : %u\n", freq[i]);
			}
			else if (i == 32) {
				printf("Space : %u\n", freq[i]);
			}
			else if (i > 126) {
				printf("%u : %u\n", i, freq[i]);
			}
			else {
				printf("%c : %u\n", i, freq[i]);
			}
		}
	}
}

int main(int argc, char **argv) {
	/**********************Open Files**********************/
	int c;
	uint8_t masks = 0x0;
	char fileOutName[256];
	memset(fileOutName, 0, 256);
	FILE *in = NULL, *out = NULL;
	while ((c = getopt(argc, argv, "i:o:vthc")) != -1) {
		switch(c) {
			case 'i': {
				in = fopen(optarg, "r");
				if (!in) {
					puts("Error: Cannot open input file");
					exit(-1);
				}
				break;
			}
			case 'o': {
				strcpy(fileOutName, optarg);
				out = fopen(optarg, "w");
				if (!out) {
					puts("Error: Cannot open output file");
					exit(-1);
				}
				break;
			}
			case 'v': {
				masks |= VERBOSE_MASK; // Print file size information
				break;
			}
			case 't': {
				masks |= TREE_MASK; // Print tree
				break;
			}
			case 'h': {
				masks |= HISTOGRAM_MASK; // Print character frequencies
				break;
			}
			case 'c': {
				masks |= CODES_MASK; // Print Huffman codes
				break;
			}
		}
	}
	if (!(in && out)) {
		puts("Usage: encode -i [input_file] -o [output_file]");
		exit(-1);
	}
	/**********************Get Character Frequencies**********************/
	uint8_t buf;
	uint32_t freq[256];
	memset(freq, 0, 1024);
	while (fread(&buf, 1, 1, in)) {
		freq[buf]++;
	}
	if (masks & HISTOGRAM_MASK) {
		printHistogram(freq);
	}
	/**********************Push characters to priority queue**********************/
	Heap *pq = newHeap();
	for (uint32_t i = 0; i < 256; i++) {
		if (freq[i]) {
			push(pq, newElem(freq[i], pq->size, (uint8_t) i));
		}
	}
	/**********************Construct Huffman Tree Using Priority Queue**********************/
	for (uint32_t i = 0; pq->size > 1; i++) {
		push(pq, join(pop(pq), pop(pq)));
	}
	if (masks & TREE_MASK) {
		printTree(pq->parent, 0);
	}
	/**********************Get Huffman Codes**********************/
	Bitvector codes[256], d;
	for (int i = 0; i < 256; i++) {
		bVInit(&codes[i]);
	}
	bVInit(&d);
	uint32_t leaves = 0;
	getHuffmanCodes(pq->parent, d, codes, &leaves);
	if (!pq || (pq && !pq->parent) || !pq->parent->val) {
		puts("Input file is empty. Exiting");
		closeAndFree(in, out, pq);
		exit(0);
	}
	if (masks & CODES_MASK) {
		printCodes(codes);
	}
	/**********************Write Compressed File**********************/
	uint32_t magicNum = MAGIC_NUMBER;
	uint32_t newSize = 0;
	/* Write magic number */
	if(!fwrite(&magicNum, 4, 1, out)) {
		puts("Error: write magicNum start fail");
		exit(-1);
	}
	newSize += 4;
	/* Write Huffman tree */
	printTreeToFile(out, pq->parent, &newSize);
	Bitvector toFile;
	bVInit(&toFile);
	fseek(in, 0, SEEK_SET);
	/* Write filesize */
	if(!fwrite(&pq->parent->val, 8, 1, out)) {
		puts("Error: write filesize fail");
		exit(-1);
	}
	newSize += 8;
	/* Write char codes; get each byte and write corresponing code using bitvector */
	while (fread(&buf, 1, 1, in)) {
		for (uint32_t i = 0; i < (codes[buf].len << 3) + codes[buf].curr; i++) {
			uint8_t bit = getBit(&codes[buf], i);
			pushBit(&toFile, &bit);
			if (toFile.len == 256) {
				if(!fwrite(toFile.bV, 1, 256, out)) {
					puts("fwrite() fail");
					exit(-1);
				}
				newSize += 256;
				bVInit(&toFile);
			}
		}
	}
	/* Write remaining bits */
	if (toFile.len || toFile.curr) {
		if(!fwrite(toFile.bV, 1, toFile.len + 1, out)) {
			puts("write code fail");
			exit(-1);
		}
		newSize += toFile.len + 1;
	}
	if (masks & VERBOSE_MASK) {
		printf("Initial file size : %lu bytes\n", pq->parent->val);
		printf("New file size : %u bytes\n", newSize);
		printf("Leaf count: %u\n", leaves);
	}
	/**********************Close And Free**********************/
	closeAndFree(in, out, pq);
	printf("Encoded file saved as \"%s\"\n", fileOutName);
	return 0;
}
