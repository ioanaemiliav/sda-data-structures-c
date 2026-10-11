/*Vizdoaga Emilia-Ioana - 314CD*/

#ifndef PREFIX_H
#define PREFIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbore.h"

typedef struct filePrefix {
    File *file; 
    struct filePrefix *next; 
    struct filePrefix *prev; 
} FilePrefix;

typedef struct listPrefix {
    int size; 
    FilePrefix *first;
    FilePrefix *last;
} *ListPrefix;

/* Initialize a new node for the prefix results list */
FilePrefix *createFilePrefix(File *fisier)
{
    FilePrefix *file = malloc(sizeof(FilePrefix));
    if (file == NULL) {
        return NULL;
    }
    file->file = fisier;
    file->next = NULL;
    file->prev = NULL;
    return file;
}

/* Initialize an empty prefix results list */
ListPrefix createListPrefix(void) 
{
    ListPrefix list = malloc(sizeof(struct listPrefix));
    if (list == NULL) {
        return NULL;
    }
    list->size = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

/* Insert a file pointer into the prefix list in lexicographical order */
void insertListPrefix(ListPrefix list, File *fisier)
{    
    if (list->size == 0) {
        list->first = createFilePrefix(fisier);
        list->last = list->first;
        list->size++;
        return;
    }
    
    FilePrefix *current = list->first; 
    while (current != NULL && strcmp(fisier->id, current->file->id) > 0) {
        current = current->next;
    }
    
    if (current != NULL && strcmp(fisier->id, current->file->id) == 0) {
        return; 
    }
    
    FilePrefix *new = createFilePrefix(fisier);
    if (current == list->first) {
        new->next = list->first;
        list->first->prev = new;
        list->first = new;
    } else if (current == NULL) {
        new->prev = list->last;
        list->last->next = new;
        list->last = new;
    } else {
        new->next = current;
        new->prev = current->prev;
        current->prev->next = new;
        current->prev = new;
    }
    list->size++;
}

/* Recursively search the sub-tree for completed words matching the prefix */
void findFilewithPrefix(ListPrefix lista, KeyTree tree)
{
    if (tree == NULL) {
        return;
    }
    
    if (tree->end == 1 && tree->referinte->size > 0) {
        FileRef *current = tree->referinte->first;
        while (current != NULL) {
            insertListPrefix(lista, current->file);
            current = current->next;
        }
    }
    findFilewithPrefix(lista, tree->child);
    findFilewithPrefix(lista, tree->sibling);
}

/* Free all memory allocated for the prefix list */
void destroyListPrefix(ListPrefix list)
{
    if (list == NULL) {
        return;
    }
    FilePrefix *current = list->first;
	while(current != NULL) {
		FilePrefix *delete = current;
		current = current->next;
		free(delete);
	}
    free(list);
}

#endif
