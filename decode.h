#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H
#include "tree.h"
void makecodechar(unsigned char, char *);
void decodechar(FILE *, FILE *, const node *, char *, unsigned char *, int *);
void decode(FILE *, FILE *,node *, int);
#endif //HUFFMAN_DECODE_H
