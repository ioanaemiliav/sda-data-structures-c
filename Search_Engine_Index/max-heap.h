#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

typedef struct heapNode{
	int scor;     // Score used for priority
	File *fisier; // Pointer to the associated file
} HeapNode;

typedef struct heapID {
    HeapNode *elem; 
    int size; 
    int capacity;
} *HeapID;

/* Initialize an empty Max-Heap with a given capacity */
HeapID createHeapID(int capacity)
{
    HeapID heap = (HeapID)malloc(sizeof(struct heapID));
    if (heap == NULL) {
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->elem = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    return heap;
}

/* Get the index of the left child */
int getLeftChild(int idx)
{
	return idx * 2 + 1;
}

/* Get the index of the right child */
int getRightChild(int idx)
{
	return idx * 2 + 2;
}

/* Get the index of the parent node */
int getParent(int idx)
{
	if (idx == 0) {
		return 0;
	}
	return (idx - 1) / 2;
}

/* Sift up to maintain the max-heap property */
void siftUp(HeapID heap, int idx)
{
	if (idx == 0) {
		return;
	}
	int parent = getParent(idx); 
    
    // Bubble up based on score (descending) and lexicographical order (ascending) on tie
	while (idx > 0 && (heap->elem[parent].scor < heap->elem[idx].scor ||
           (heap->elem[parent].scor == heap->elem[idx].scor && strcmp(
           heap->elem[parent].fisier->id, heap->elem[idx].fisier->id) > 0))) {
		
        HeapNode aux = heap->elem[parent];
		heap->elem[parent] = heap->elem[idx];
		heap->elem[idx] = aux;
		idx = parent;
		parent = getParent(idx);
	}
}

/* Insert a new node into the heap */
void insertHeapID(HeapID heap, int scor, File *fisier)
{
	if (heap->capacity == heap->size) {
		heap->capacity *= 2;
		HeapNode *aux = (HeapNode *)realloc(heap->elem, heap->capacity * sizeof(HeapNode));
		if (aux == NULL) {
			return;
		}
		heap->elem = aux;
	}
	heap->elem[heap->size].scor = scor;
    heap->elem[heap->size].fisier = fisier;
    heap->size++;
	siftUp(heap, heap->size - 1);
}

/* Sift down to maintain the max-heap property after extraction */
void siftDown(HeapID heap, int idx)
{
	int left = getLeftChild(idx);
	int right = getRightChild(idx);
	int big = idx;
    
	if (left < heap->size && (heap->elem[left].scor > heap->elem[big].scor ||
        (heap->elem[left].scor == heap->elem[big].scor && strcmp(
        heap->elem[left].fisier->id, heap->elem[big].fisier->id) < 0))) {
		big = left;
	}
	if (right < heap->size && (heap->elem[right].scor > heap->elem[big].scor ||
        (heap->elem[right].scor == heap->elem[big].scor && strcmp(
        heap->elem[right].fisier->id, heap->elem[big].fisier->id) < 0))) {
		big = right;
	}
	if (big != idx) {
		HeapNode aux = heap->elem[idx];
		heap->elem[idx] = heap->elem[big];
		heap->elem[big] = aux;
		idx = big;
		siftDown(heap, idx);
	}
}

/* Remove the maximum element (root) from the heap */
void removeMaxHeapID(HeapID heap)
{
	if (heap == NULL || heap->size == 0) {
		return;
	}
	heap->elem[0] = heap->elem[heap->size -1];
	heap->size--;
	if (heap->size > 0) {
		siftDown(heap, 0);
	}
}

/* Free all memory allocated for the heap */
void destroyHeapID(HeapID heap)
{
    if (heap == NULL) {
        return;
    }
	if (heap->elem != NULL) {
        free(heap->elem);
    }
	free(heap);
}
#endif
