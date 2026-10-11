/*Vizdoaga Emilia-Ioana - 314CD*/

#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct File{
    char *id; // retinem id-ul
    int scor; // retinem scorul
    struct File* next; // next file
    struct File* prev; //  prev file
} File;

typedef struct ListFile{
    struct File* first; // primul fisier din lista
    struct File* last; // ultimul fisier din lista
} ListFile;


// crearea unei liste
ListFile* createListFile(void)
{
    ListFile *list = malloc(sizeof(ListFile));
    list->first = NULL;
    list->last = NULL;
    return list;
}

// crearea unui fisier
File *createFile(char *id, int scor)
{
	File *new= malloc(sizeof(File));
	if (new == NULL) {
		return NULL;
	}
	new->scor = scor; // retinem scorul
	new->id = malloc(strlen(id) + 1);
	if (new->id == NULL) {
        free(new);
        return NULL;
    }
    strcpy(new->id, id); // retinem id-ul
	new->next = NULL;
	new->prev = NULL;
	return new;
}

// adaugarea fisierului la finalul listei
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

// verificarea daca fisierul exista
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

// gasirea adresei fisierului cu id ul cautat
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

// eliminarea fisierului cu id ul cautat
void deleteFile(ListFile *list, File *delete)
{
// lista este goala
	if (list == NULL) {
		return;
	}
// lista are un singur fisier
	if (list->first == list->last && list->first == delete) {
		list->first = NULL;
		list->last = NULL;
		free(delete->id);
		free(delete);
		return;
	}
// fisierul este primul
	if (list->first == delete) {
		list->first = delete->next;
		delete->next->prev = NULL;
		free(delete->id);
		free(delete);
		return;
	}
// fisierul este ultimul
	if (list->last == delete) {
		list->last = delete->prev;
		delete->prev->next = NULL;
		free(delete->id);
		free(delete);
		return;
	}
// fisierul este la mijloc
	delete->prev->next = delete->next;
	delete->next->prev = delete->prev;
	free(delete->id);
	free(delete);
	return;
}

// eliberarea memoriei listei cu fisiere
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
