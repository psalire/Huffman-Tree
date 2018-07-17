### A standard Huffman Tree Encoder and Decoder

encode.c - builds Huffman tree based on character frequency, writes filesize, tree, and encoded bytes to file

decode.c - builds Huffman tree from an encoded file and decodes bytes by traversing tree

Huffman trees are used as a type of lossless data compression. It works by encoding the most frequent occurring bytes in a file to shorter lengths of a few bits.

For example if encoding the string "Hello," it may be encoded as:

l: 1

e: 01

o: 000

H: 001

'l' saves 7 bits each time it is written.
