/*Vizdoaga Emilia-Ioana - 314CD*/

#ifndef BONUS_H
#define BONUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbore.h"

typedef struct filePrefix {
    File *file; // retinem un pointer catre fisierul din lista
    struct filePrefix *next; // urmatorul pointer la fisier
    struct filePrefix *prev; // anteriorul pointer la fisier
} FilePrefix;

typedef struct listPrefix {
    int size; // numarul de fisiere din lista de referinta
    FilePrefix *first;
    FilePrefix *last;
} *ListPrefix;

// crearea unei nod din lista
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

// creearea listei cu pointeri
ListPrefix createListPrefix(void) {
    ListPrefix list = malloc(sizeof(struct listRef));
    if (list == NULL) {
        return list;
    }
    list->size = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

// adaugarea unui pointer in lista
void insertListPrefix(ListPrefix list, File *fisier)
{   
    if (list->size == 0) { // lista este goala
        list->first = createFilePrefix(fisier);
        list->last = list-> first ;
        list->size++;
        return;
    }
    FilePrefix *current = list->first; 
    while (current != NULL && strcmp(fisier->id, current->file->id) > 0) {
        current = current->next;
    }
// current-> pointer la fisierul >= fisierul cautat, dpdv lexicografic 
    if (current != NULL && strcmp(fisier->id, current->file->id) == 0) {
        return; // fisierul exista deja in lista de referinta
    }
// crearea structurii cu pointerul catre fisierul cautat
    FilePrefix *new = createFilePrefix(fisier);
    if (current == list->first) { // este primul in lista
        new->next = list->first;
        list->first->prev = new;
        list->first = new;
    } else if (current == NULL) { // este ultimul in lista
        new->prev = list->last;
        list->last->next = new;
        list->last = new;
    } else { // este la mijloc
        new->next = current;
        new->prev = current->prev;
        current->prev->next = new;
        current->prev = new;
    }
    list->size++;
}

// cautarea cuvintelor terminate in subarbore
void findFilewithPrefix(ListPrefix lista, KeyTree tree)
{
    if (tree == NULL) {
        return;
    }
// verificam daca este un cuvant, are lista de referinta
    if (tree->end == 1 && tree->referinte->size > 0) {
        FileRef *current = tree->referinte->first;
        while (current != NULL) {
            insertListPrefix(lista, current->file); // adaugam fisierul
            current = current->next;
        }
    }
    findFilewithPrefix(lista, tree->child);
    findFilewithPrefix(lista, tree->sibling);

}

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
