#include <stdio.h>

#include "decode.h"
#include "tree.h"

void makecodechar(unsigned char c, char * code){
    for (int i = 0; i < 8; ++i){
        if (c & 0x80) code[i] = '1';
        else code[i] = '0';
        c <<= 1;
    }
}

void decodechar(FILE * in, FILE * out, const node * root, char * input, unsigned char * inchar, int * inbits){
    if (!(root->left) && !(root->right)){
        fprintf(out, "%c", root->c);
        return;
    }
    if ((*inbits) == 8){
        (*inbits) = 0;
        makecodechar((*inchar), input);
        (*inchar) = getc(in);
    }
    if (input[(*inbits)] == '1'){
        (*inbits)++;
        decodechar(in, out, root->right, input, inchar, inbits);
    } else {
        (*inbits)++;
        decodechar(in, out, root->left, input, inchar, inbits);
    }
}

void decode(FILE * in, FILE * out, node * root, int tailCode){
    unsigned char inchar;
    char input[9] = {0};
    int inbits = 0;
    makecodechar(getc(in), input);
    inchar = getc(in);
    decodechar(in, out, root, input, &inchar, &inbits);
    while (1) {
        if (feof(in) && ((inbits % 8) == tailCode)) break;
        decodechar(in, out, root, input, &inchar, &inbits);
    }
}
