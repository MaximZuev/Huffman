#include <stdlib.h>

#include "heap.h"
#include "tree.h"

static void swap(node * a, node * b){
    node t = *a;
    *a = *b;
    *b = t;
}

static void sift(node ** A, int i){
    while ((A[i]->count < A[(i - 1) / 2]->count) && (i > 0)){
        swap(A[i], A[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

static void siftDown(node ** A, int i, int heapSize){
    int j, left, right;
    while ((2 * i + 1) < heapSize) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        j = left;
        if ((right < heapSize) && (A[right]->count < A[left]->count)) j = right;
        if (A[i]->count <= A[j]->count) break;
        swap(A[i], A[j]);
        i = j;
    }
}

static node * extractMin(node ** A, int * heapSize){
    node * min = (node *) malloc(sizeof(node));
    (*min) = (*A)[0];
    swap(A[0], A[(*heapSize) - 1]);
    (*heapSize) = (*heapSize) - 1;
    siftDown(A, 0, *heapSize);
    return min;
}

node * unionNode(node ** A, int * heapSize){
    node * root, *left, *right;
    left = extractMin(A, heapSize);
    right = extractMin(A, heapSize);
    root = newNode(left->count + right->count);
    root->left = left;
    root->right = right;
    return root;
}

void add(node ** A, int * heapSize, node * root){
    *(A[(*heapSize)]) = (*root);
    sift(A, (*heapSize));
    (*heapSize)++;
}

node ** makeHeap(int * countchar, int heapSize){
    node **heap = (node **) calloc(heapSize, sizeof(node *));
    for (int i = 0, k = 0; i < 256; ++i) {
        if (countchar[i] != 0) {
            heap[k] = newNode(countchar[i]);
            heap[k]->c = (unsigned char) i;
            sift(heap, k);
            k++;
        }
    }
    return heap;
}

void freeHeap(node ** heap, int heapSize){
    if (!heap) return;
    for (int i = 1; i < heapSize; ++i){
        free(heap[i]);
    }
    free(heap);
}
