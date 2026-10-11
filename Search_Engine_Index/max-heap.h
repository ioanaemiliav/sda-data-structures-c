/*Vizdoaga Emilia-Ioana - 314CD*/

#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

typedef struct heapNode{
	int scor; // retinem scorul, in functie de el se face prioritatea
	File *fisier; // retinem pointer catre fisier
} HeapNode;

typedef struct heapID {
    HeapNode *elem; // elementele din heap
    int size; 
    int capacity;
} *HeapID;

// crearea Heap-ului cu ID-uri
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

// returneaza indexul din elem al copilului stang
int getLeftChild(int idx)
{
	return idx * 2 + 1;
}

// returneaza indexul din elem al copilului drept
int getRightChild(int idx)
{
	return idx * 2 + 2;;
}

// returneaza indexul din elem al parintelui
int getParent(int idx)
{
	if (idx == 0) {
		return 0;
	}
	return (idx - 1) / 2;
}

// cernere in sus
void siftUp(HeapID heap, int idx)
{
	if (idx == 0) {
		return;
	}
	int parent = getParent(idx); // indexul parintelui
// urcam nodul respectand prioritatea conform scorului
// daca scorul e egal urcam lexicografic
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

// inserarea unui nod in heap
void insertHeapID(HeapID heap, int scor, File *fisier)
{
	if (heap->capacity == heap->size) {
		heap->capacity *= 2;
		HeapNode *aux = (HeapNode *)realloc(heap->elem, heap->capacity
                                            * sizeof(HeapNode));
		if (aux == NULL) {
			return;
		}
		heap->elem = aux;
	}
	heap->elem[heap->size].scor = scor;
    heap->elem[heap->size]. fisier = fisier;
    heap->size++;
	siftUp(heap, heap->size - 1);
}

// cernerea in jos
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

// stergerea maximului din Heap
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

// eliberarea memoriei alocata pentru heap
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