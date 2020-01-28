
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"
#include "binaryTree.h"
#include "stack.h"

int main(int argc, char **argv) {
	/**********************Open Files**********************/
	int c;
	FILE *in = NULL, *out = NULL;
	uint8_t tree = 0;
	char fileOutName[256];
	memset(fileOutName, 0, sizeof(char) << 8);
	while ((c = getopt(argc, argv, "ti:o:")) != -1) {
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
					puts("Error: Cannot open input file");
					exit(-1);
				}
				break;
			}
			case 't' : {
				tree = 1;
				break;
			}
		}
	}
	if (!(in && out)) {
		puts("Usage: decode -i [input_file] -o [output_file]");
		exit(-1);
	}
	/**********************Read File**********************/
	/* Check magic number */
	uint32_t magicNum;
	if (!fread(&magicNum, 4, 1, in)) {
		puts("read magic num fail");
		exit(-1);
	}
	if (magicNum != MAGIC_NUMBER) {
		puts("Incorrect magic number. This file was not encoded by the encoder");
		if (in) {
			fclose(in);
		}
		if (out) {
			fclose(out);
		}
		exit(0);
	}
	/* Build Huffman Tree */
	Stack *s = stackInit();
	uint8_t buf;
	for(;;) {
		/* Get Node type, leaf or internal */
		if(!fread(&buf, 1, 1, in)) {
			puts("Error: fread() fail Huffman Tree");
			exit(-1);
		}
		if (buf == 'I') {
			stackPush(s, join(stackPop(s), stackPop(s)));
		}
		else if (buf == 'L') {
			/* Read leaf symbol */
			if(!fread(&buf, 1, 1, in)) {
				puts("Error: fread() fail Huffman Tree symbol");
				exit(-1);
			}
			stackPush(s, newNode(0, buf));
		}
		/* End of tree */
		else {
			break;
		}
	}
	if (tree) {
		printTree(s->top, 0);
	}
	fseek(in, -1, SEEK_CUR);
	/* Read File Size */
	uint64_t fileSize;
	if (!fread(&fileSize, 8, 1, in)) {
		puts("read file size fail");
		exit(-1);
	}
	/**********************Write File using Huffman Tree**********************/
	Node *n = s->top;
	uint8_t decoded;
	for (uint64_t i = 0; i < fileSize && fread(&buf, 1, 1, in);) {
		/* Traverse Huffman Tree reading byte */
		for (int j = 0; j < 8; j++) {
			/* 1, go right; 0; go left */
			if ((buf & (0x1 << j)) >> j) {
				n = n->right;
			}
			else {
				n = n->left;
			}
			/* Leaf, write decoded code */
			if (n && !(n->left || n->right)) {
				decoded = n->symbol;
				if (!fwrite(&decoded, 1, 1, out)) {
					puts("write fail Huffman decoded");
					exit(-1);
				}
				n = s->top;
				if (++i >= fileSize) {
					break;
				}
			}
		}
	}
	/**********************Close And Free**********************/
	if (in) {
		fclose(in);
	}
	if (out) {
		fclose(out);
	}
	printf("Decoded file saved as \"%s\"\n", fileOutName);
	freeBTree(s->top);
	free(s);
	return 0;
}
