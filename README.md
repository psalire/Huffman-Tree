# Huffman Tree Encoding and Decoding

## File compression and Decompression

### Files

* src/
    * encode.c - Encoder/compressor
    * decode.c - Decoder/decompressor
    * binaryTree.c - Binary tree implementation
    * minheap.c - Min-heap implementation
    * stack.c - Stack implementation
* include/
    * .h files
* bin/
    * .o files

### How to use

On a Linux machine:
1. Run ```make```
2. Compress a file with ```./encode -i [input file] -o [output name]```
3. Decompress a file with ```./decode -i [input file] -o [output name]```

### How it works

Huffman trees are used as a type of lossless data compression. It works by encoding the most frequent occurring bytes in a file to smaller sizes e.g. to a few bits. For example, if the most frequent occurring byte in a file is 'e', which is 101 in ASCII or `01100101` in binary, this may be encoded to fewer bits e.g. `001` - here, letter 'e' only takes 3 bits per occurrence rather than 8 bits, saving 5 bits per letter 'e'.

#### encode.c

Building the Huffman tree:
1. Read file and build table of each character occurrence.
2. Push all characters mapped to occurence to a max priority queue.
3. Pop 2 values from the priority queue and join them as children of a binary tree node. Push this node back to the queue with the sum of the children character counts as its value.
4. Repeat 3. until priority is length 1.
5. Huffman tree is constructed i.e. the last node in the priority queue. The highest occurring characters are near the top of the tree.
6. Do a tree traversal of the Huffman tree to get the encoded values of each occurring character. I.e. if traversing to the left child, record '0', if traversing to the right child, record '1'. For example, if the path to the letter 'E' is left, left, right, then the encoded value for it is '001'- 3 bits long. Save this in a table.

Writing the encoded file:
7. Write ```0xd0d0d00d``` to the output file. This is used to identify that it was written by this program when decoding.
8. Write the Huffman tree to the output file represented as a string and the filesize of the input file. These are used when decoding.
9. Read a byte in the input file and use the table from 6. to write the encoded value to the output file. Use a bitvector to write individual bits.
10. Repeat 9. until end of the input file.

#### decode.c

1. Read the input file. Check if ```0xd0d0d00d``` occurs at the beginning to determine if this file was written by ```encode.c```
2. Reconstruct the Huffman tree from the string representation in the file.
3. Read and save the filesize that is written in the file.
4. Read each bit in the input file and use the bit value to traverse the Huffman tree. I.e. '1' means traverse right, '0' means traverse left. Once a leaf is reached, write the character to the output file.
