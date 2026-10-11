#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "arbore.h"
#include "max-heap.h"
#include "bonus.h"

/* Parse input and add a new file with its keywords to the index */
void add(char *buff, FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    char *add = strtok(buff, " ");
    if (add == NULL) {
        return;
    }
    char *id = strtok(NULL, " ");
    if (containsFile(listaFisiere, id) == 1) {
        fprintf(fout, "EXISTS\n");
        return;
    }
    fprintf(fout, "OK\n");
    
    char *scor_string = strtok(NULL, " ");
    int scor = atoi(scor_string);
    insertFile(listaFisiere, id, scor);
    
    char *nr_cuvinte_string = strtok(NULL, " ");
    int nr_cuvinte = atoi(nr_cuvinte_string);
    
    for (int i = 0; i < nr_cuvinte; i++) {
        char *cuvant = strtok(NULL, " ");
        insertTree(treeCuvinte, cuvant, listaFisiere->last);
    }
}

/* Delete a file from the index and clean up any dangling Trie nodes */
void del(char *buff, FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    char id[50];
    sscanf(buff, "DEL %s", id);
    if (containsFile(listaFisiere, id) == 0) {
        fprintf(fout, "NOT FOUND\n");
        return;
    }
    fprintf(fout, "OK\n");
    
    File *delete = findFile(listaFisiere, id);
    deleteRefFile(treeCuvinte->child, delete);
    deleteFile(listaFisiere, delete);
}

/* Add a new keyword to an existing file in the index */
void addkw(char *buff, FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    char *addkw = strtok(buff, " ");
    if (addkw == NULL) {
        return;
    }
    char *id = strtok(NULL, " ");
    if (containsFile(listaFisiere, id) == 0) {
        fprintf(fout, "NOT FOUND\n");
        return;
    }
    
    char *cuvant = strtok(NULL, " ");
    fprintf(fout, "OK\n");
    
    File *fisier = findFile(listaFisiere, id);
    
    if (containsFileRef(findKeyWord(treeCuvinte, cuvant), fisier) == 1) {
        return;
    }
    insertTree(treeCuvinte, cuvant, fisier);
}

/* Remove a specific keyword from a file and update the Trie structure */
void delkw(char *buff, FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    char *delkw = strtok(buff, " ");
    if (delkw == NULL) {
        return;
    }
    char *id = strtok(NULL, " ");
    if (containsFile(listaFisiere, id) == 0) {
        fprintf(fout, "NOT FOUND\n");
        return;
    }
    
    char *cuvant = strtok(NULL, " ");
    fprintf(fout, "OK\n");
    
    File *delete = findFile(listaFisiere, id);
    
    if (containsFileRef(findKeyWord(treeCuvinte, cuvant), delete) == 0) {
        return;
    }
    
    deleteKeyWord(findKeyWord(treeCuvinte, cuvant), delete);
    
    if (KeyWordsFile(treeCuvinte, delete) == 0) {
        deleteFile(listaFisiere, delete);
    }
}

/* Search the Trie and print all files associated with a specific keyword */
void find(char *buff, FILE *fout, KeyTree treeCuvinte)
{
    char cuvant[100];
    sscanf(buff, "FIND %s", cuvant); 
    
    KeyTree last_letter = findKeyWord(treeCuvinte, cuvant);
    
    if (last_letter == NULL || last_letter->end == 0) {
        fprintf(fout, "EMPTY\n");
        return;
    }
    
    fprintf(fout, "%d", last_letter->referinte->size);
    FileRef *current = last_letter->referinte->first;
    
    for (int i = 0; i < last_letter->referinte->size; i++) {
        fprintf(fout, " %s", current->file->id);
        current = current->next;
    }
    fprintf(fout, "\n");
}

/* Retrieve the top K most relevant files for a keyword using a Max-Heap */
void topk(char *buff, FILE *fout, KeyTree treeCuvinte)
{
    char cuvant[100];
    int k;
    sscanf(buff, "TOPK %s %d", cuvant, &k);
    
    KeyTree last_letter = findKeyWord(treeCuvinte, cuvant);
    if (last_letter == NULL || last_letter->end == 0) {
        fprintf(fout, "EMPTY\n");
        return;
    }
    
    ListRef list = last_letter->referinte;
    FileRef *current = list->first;
    HeapID heapID = createHeapID(list->size);
    
    if (list->size < k) {
        k = list->size;
    }
    
    fprintf(fout, "%d", k);
    for (int i = 0; i < list->size; i++) {
        insertHeapID(heapID, current->file->scor, current->file);
        current = current->next;
    }
    
    for (int i = 0; i < k; i++) {
        fprintf(fout, " %s", heapID->elem[0].fisier->id);
        removeMaxHeapID(heapID);
    }
    fprintf(fout, "\n");
    destroyHeapID(heapID);
}

/* Print all indexed keywords and their associated files in lexicographical order */
void print(FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    int capacitate = 100;
    char *cuvant = malloc(100);
    if (cuvant == NULL) {
        return;
    }
    
    int nr_print = 0;
    printKeyWords(fout, listaFisiere, treeCuvinte->child, 0, cuvant,
                  &capacitate, &nr_print);
                  
    if (nr_print == 0) { 
        fprintf(fout, "EMPTY\n");
    }
    free(cuvant);
}

/* Find and print all files containing keywords that match a given prefix */
void prefix(char *buff, FILE *fout, KeyTree treeCuvinte)
{
    char prefix[100];
    sscanf(buff, "PREFIX %s", prefix);
    
    KeyTree last_letter = findKeyWord(treeCuvinte, prefix);
    if (last_letter == NULL) {
        fprintf(fout, "EMPTY\n");
        return;
    }
    
    ListPrefix listaPrefix = createListPrefix();
    
    if (last_letter->end == 1 && last_letter->referinte->size > 0) {
        FileRef *current = last_letter->referinte->first;
        while (current != NULL) {
            insertListPrefix(listaPrefix, current->file);
            current = current->next;
        }
    }
    
    findFilewithPrefix(listaPrefix, last_letter->child);
    fprintf(fout, "%d", listaPrefix->size);
    
    FilePrefix *current = listaPrefix->first;
    while (current != NULL) {
        fprintf(fout, " %s",current->file->id);
        current = current->next;
    }
    fprintf(fout, "\n");
    
    destroyListPrefix(listaPrefix);
}

#endif
