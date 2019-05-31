#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H
#include <stdio.h>
typedef struct node{
    int count;
    int strlcode;
    unsigned char c;
    char code[256];
    struct node *left;
    struct node *right;
}node;
node * newNode(int);
node * makeTree(node **, int);
void putTree(const node *, FILE *);
node * recoverTree(FILE *, int);
void freeTree(node *);
void makeTableH(node *, char **, int *);
void putByte(char *, FILE *, int *, unsigned char *);
#endif //HUFFMAN_TREE_H
