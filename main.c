#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tree.h"
#include "heap.h"
#include "decode.h"

void putCode(FILE * in, FILE * out, char ** tableH){
    unsigned char outchar = 0, c;
    int outbits = 0;
    while(feof(in) == 0){
        c = getc(in);
        if (feof(in) == 0) putByte(tableH[c], out, &outbits, &outchar);
    }
    if (outbits != 8) outchar <<= (8 - outbits);
    if (outbits != 0) fputc(outchar, out);
}

void readInput(FILE *in, char arg, int * countchar, int * heapSize, int * tailCode, int * sizeTree) {
    if (arg == 'c') {
        fseek(in, 3, SEEK_SET);
        while (feof(in) == 0) {
            unsigned char c = getc(in);
            if (!feof(in)) {
                if (!countchar[c])(*heapSize)++;
                countchar[c]++;
            }
        }
        (*sizeTree) = ceil(((double) (((*heapSize) * 10) - 1)) / 8);
        fseek(in, 3, SEEK_SET);
    } else {
        fseek(in, 3, SEEK_SET);
        unsigned char c = getc(in);
        (*tailCode) = (c & 0x0E) >> 1;
        (*sizeTree) = (((int) (c & 0x01)) * 256) + (int) (getc(in));
    }
}

int main() {
    FILE * in = fopen("in.txt", "rb"), * out = fopen("out.txt", "wb");
    int countchar[256] = {0}, heapSize = 0, tailCode = 0, sizeTree = 0;
    node ** heap = NULL, * root = NULL;
    char ** tableH = NULL, arg = getc(in);
    if (arg == 'c') {
        readInput(in, arg, countchar, &heapSize, &tailCode, &sizeTree);
        heap = makeHeap(countchar, heapSize);
        root = makeTree(heap, heapSize);
        tableH = (char **) calloc(256, sizeof(char *));
        makeTableH(root, tableH, &tailCode);
        if (sizeTree < 256) fprintf(out, "%c%c", (tailCode << 1) | (unsigned char)0, (unsigned char) sizeTree);
        else fprintf(out, "%c%c", (tailCode << 1) | (unsigned char)1, (unsigned char) (sizeTree - 256));
        putTree(root, out);
        putCode(in, out, tableH);
    } else {
        readInput(in, arg, countchar, &heapSize, &tailCode, &sizeTree);
        root = recoverTree(in, sizeTree);
        if (sizeTree != 0) decode(in, out, root, tailCode);
    }
    free(tableH);
    freeTree(root);
    freeHeap(heap, heapSize);
    fclose(in);
    fclose(out);
    return 0;
}
