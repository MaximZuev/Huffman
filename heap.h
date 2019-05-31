#ifndef HUFFMAN_HEAP_H
#define HUFFMAN_HEAP_H
#include "tree.h"
node * unionNode(node **, int *);
void add(node **, int *, node *);
node ** makeHeap(int *, int);
void freeHeap(node **, int);
#endif //HUFFMAN_HEAP_H
