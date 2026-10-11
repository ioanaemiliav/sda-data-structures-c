#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct File {
    char *id;
    int scor;
    struct File* next;
    struct File* prev;
} File;

typedef struct ListFile {
    struct File* first;
    struct File* last;
} ListFile;

/* Initialize an empty doubly linked list for files */
ListFile* createListFile(void)
{
    ListFile *list = malloc(sizeof(ListFile));
    list->first = NULL;
    list->last = NULL;
    return list;
}

/* Allocate and initialize a new file node */
File *createFile(char *id, int scor)
{
	File *new = malloc(sizeof(File));
	if (new == NULL) {
		return NULL;
	}
	new->scor = scor;
	new->id = malloc(strlen(id) + 1);
	if (new->id == NULL) {
        free(new);
        return NULL;
    }
    strcpy(new->id, id);
	new->next = NULL;
	new->prev = NULL;
	return new;
}

/* Insert a new file at the end of the list */
void insertFile(ListFile *list, char *id, int scor)
{
	File *new_file = createFile(id, scor);
	if (list->first == NULL) {
		list->first = new_file;
		list->last = new_file;
		return;
	}
	list->last->next = new_file;
	new_file->prev = list->last;
	list->last = new_file;
}

/* Check if a file with the given ID exists in the list */
int containsFile(ListFile *list, char *id)
{
	if (list == NULL || list->first == NULL) {
		return 0;
	}
	if (strcmp(list->last->id, id) == 0) {
		return 1;
	}
	File *aux = list->first;
	while (aux != list->last) {
		if (strcmp(aux->id, id) == 0) {
		    return 1;
	    }
		aux = aux->next;
	}
	return 0;
} 

/* Find and return the pointer to the file with the given ID */
File *findFile(ListFile *list, char *id)
{
	if (list == NULL || list->first == NULL) {
		return 0;
	}
	if (strcmp(list->last->id, id) == 0) {
		return list->last;
	}
	File *aux = list->first;
	while (aux != list->last) {
		if (strcmp(aux->id, id) == 0) {
			return aux;
	    }
		aux = aux->next;
	}
	return 0;
}

/* Remove a specific file from the list and free its memory */
void deleteFile(ListFile *list, File *delete)
{
	if (list == NULL) {
		return;
	}
    
    // Single node case
	if (list->first == list->last && list->first == delete) {
		list->first = NULL;
		list->last = NULL;
		free(delete->id);
		free(delete);
		return;
	}
    
    // First node case
	if (list->first == delete) {
		list->first = delete->next;
		delete->next->prev = NULL;
		free(delete->id);
		free(delete);
		return;
	}
    
    // Last node case
	if (list->last == delete) {
		list->last = delete->prev;
		delete->prev->next = NULL;
		free(delete->id);
		free(delete);
		return;
	}
    
    // Middle node case
	delete->prev->next = delete->next;
	delete->next->prev = delete->prev;
	free(delete->id);
	free(delete);
	return;
}

/* Free all memory allocated for the file list */
void destroyListFile(ListFile* list)
{
	if(list == NULL) {
		return;
	}
	File *current = list->first;
	while(current != NULL) {
		File *delete = current;
		current = current->next;
        free(delete->id);
		free(delete);
	}
	free(list);
}

#endif
