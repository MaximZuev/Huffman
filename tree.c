#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "heap.h"
#include "decode.h"

node * newNode(int count){
    node * newNode = (node *) malloc(sizeof(node));
    newNode->strlcode = 0;
    newNode->c = '\0';
    newNode->code[0] = '\0';
    newNode->count = count;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

node * makeTree(node ** A, int heapSize){
    int size = heapSize;
    if (size > 0) {
        if (size == 1) {
            node * root = newNode(0);
            root->left = A[0];
            return root;
        }
        while (size > 1) {
            node * root = unionNode(A, &size);
            add(A, &size, root);
            free(root);
        }
        return A[0];
    } else return NULL;
}

static void putNode(const node *root, FILE * out, int * outbits, unsigned char * outchar){
    char codech[9];
    codech[8] = '\0';
    if ((root->left) && (root->right)) {
        putByte("1", out, outbits, outchar);
        putNode(root->left, out, outbits, outchar);
        putNode(root->right, out, outbits, outchar);
    } else {
        if (root->left) {
            putByte("1", out, outbits, outchar);
            putNode(root->left, out, outbits, outchar);
        } else {
            putByte("0", out, outbits, outchar);
            makecodechar(root->c, codech);
            putByte(codech, out, outbits, outchar);
        }
    }
}

void putTree(const node * root, FILE * out){
    if (!root) return;
    unsigned char outchar = 0;
    int outbits = 0;
    putNode(root, out, &outbits, &outchar);
    if (outbits != 8) outchar <<= (8 - outbits);
    if (outbits != 0) fputc(outchar, out);
}

static void recoverNode(node * root, char * input, int * inbits, unsigned char * tree, int * iteration, int sizeTree){
    if((*iteration) == sizeTree){
        return;
    }
    if((*inbits) == 8){
        (*inbits) = 0;
        (*iteration)++;
        makecodechar(tree[*iteration], input);
    }
    if (input[*inbits] == '1'){
        root->left = newNode(0);
        root->right = newNode(0);
        (*inbits)++;
        recoverNode(root->left, input, inbits, tree, iteration, sizeTree);
        recoverNode(root->right, input, inbits, tree, iteration, sizeTree);
    } else {
        (*inbits)++;
        root->c = (((tree[*iteration]) << (*inbits)) | ((tree[(*iteration) + 1]) >> (8 - (*inbits))));
        (*iteration)++;
        makecodechar(tree[*iteration], input);
    }
}

node * recoverTree(FILE * in, int sizeTree){
    unsigned char input[9] = {0}, * tree = (unsigned char *)calloc(sizeTree, sizeof(unsigned char));
    int inbits = 0, iteration = 0;
    node * root = newNode(0);
    fread(tree, 1, sizeTree, in);
    makecodechar(tree[0], input);
    recoverNode(root, input, &inbits, tree, &iteration, sizeTree);
    free(tree);
    return root;
}

void freeTree(node * root){
    if (!root) return;
    if (root->left) freeTree(root->left);
    if (root->right) freeTree(root->right);
    free(root);
}

void makeTableH(node * root, char ** tableH, int * tailCode){
    if (!root) return;
    if (!(root->left) && !(root->right)) {
        tableH[root->c] = root->code;
        (*tailCode) = ((*tailCode) + (root->strlcode * root->count)) % 8;
        return;
    }
    if (root->left) {
        strcat(root->left->code, root->code);
        strcat(root->left->code, "0");
        root->left->strlcode += root->strlcode + 1;
        makeTableH(root->left, tableH, tailCode);
    }
    if (root->right) {
        strcat(root->right->code, root->code);
        strcat(root->right->code, "1");
        root->right->strlcode += root->strlcode + 1;
        makeTableH(root->right, tableH, tailCode);
    }
}

void putByte(char * outcode, FILE * out, int * outbits, unsigned char * outchar){
    for (int i = 0; outcode[i] != '\0'; ++i){
        (*outchar) <<= 1;
        (*outchar) |= (outcode[i] & 0x01);
        (*outbits)++;
        if ((*outbits) == 8){
            fputc((*outchar), out);
            (*outbits) = 0;
            (*outchar) = 0;
        }
    }
}
