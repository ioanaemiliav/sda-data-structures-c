/*Vizdoaga Emilia-Ioana - 314CD*/

#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "arbore.h"
#include "max-heap.h"
#include "bonus.h"

void add(char *buff, FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    char *add = strtok(buff, " "); // se retine "ADD"
    if (add == NULL) {
        return;
    }
    char *id = strtok(NULL, " "); // retinem id-ul
    if (containsFile(listaFisiere, id) == 1) { // fisierul exista deja
        fprintf(fout, "EXISTS\n");
        return;
    }
    fprintf(fout, "OK\n"); // fisierul nu exista
    char *scor_string = strtok(NULL, " "); // retinem scorul ca string
    int scor = atoi(scor_string); // retinem scorul ca int
    insertFile(listaFisiere, id, scor); // adaugam fisierul in lista
    char *nr_cuvinte_string = strtok(NULL, " "); // retinem nr de cuvinte ca string
    int nr_cuvinte = atoi(nr_cuvinte_string); // retinem nr de cuvinte ca int
    for (int i = 0; i < nr_cuvinte; i++) {
        char *cuvant = strtok(NULL, " "); // retinem cate un cuvant
        insertTree(treeCuvinte, cuvant, listaFisiere->last);
    }
}

void del(char *buff, FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    char id[50]; // retinem id-ul
    sscanf(buff, "DEL %s", id);
    if (containsFile(listaFisiere, id) == 0) { // fisierul nu exista
        fprintf(fout, "NOT FOUND\n");
        return;
    }
    fprintf(fout, "OK\n"); // fisierul exista
// retinem adresa fisierului cu id-ul cautat
    File *delete = findFile(listaFisiere, id);
// stergem fisieul din referinta si literele in plus
    deleteRefFile(treeCuvinte->child, delete);
// stergem fisierul din lista
    deleteFile(listaFisiere, delete);
}

void addkw(char *buff, FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    char *addkw = strtok(buff, " "); // se retine "ADDKW"
    if (addkw == NULL) {
        return;
    }
    char *id = strtok(NULL, " "); // retinem id-ul
    if (containsFile(listaFisiere, id) == 0) { // fisierul nu exista
        fprintf(fout, "NOT FOUND\n");
        return;
    }
// fisierul exista
    char *cuvant = strtok(NULL, " "); // retinem cuvantul
    fprintf(fout, "OK\n");
// retinem adresa fisierului cu id-ul cautat
    File *fisier = findFile(listaFisiere, id);
// verificam daca fisierul exista deja in lista de referinta a cuvantului
    if (containsFileRef(findKeyWord(treeCuvinte, cuvant), fisier) == 1) {
        return;
    }
// inseram cuvantul in arbore/ fisierul in lista de referinta a cuvantului
    insertTree(treeCuvinte, cuvant, fisier);
}

void delkw(char *buff, FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    char *delkw = strtok(buff, " "); // se retine "DELKW"
    if (delkw == NULL) {
        return;
    }
    char *id = strtok(NULL, " "); // retinem id-ul
    if (containsFile(listaFisiere, id) == 0) { // fisierul nu exista
        fprintf(fout, "NOT FOUND\n");
        return;
    }
// fisierul exista
    char *cuvant = strtok(NULL, " "); // retinem cuvantul
    fprintf(fout, "OK\n");
// retinem adresa fisierului cu id-ul cautat
    File *delete = findFile(listaFisiere, id);
// verificam daca fisierul exista in lista de referinta a cuvantului
    if (containsFileRef(findKeyWord(treeCuvinte, cuvant), delete) == 0) {
        return;
    }
// stergem fisieul din referinta si literele in plus
    deleteKeyWord(findKeyWord(treeCuvinte, cuvant), delete);
// verificam daca fisierul mai are cuvinte
    if (KeyWordsFile(treeCuvinte, delete) == 0) {
        deleteFile(listaFisiere, delete); // stergem fisierul din lista
    }
}

void find(char *buff, FILE *fout, KeyTree treeCuvinte)
{
    char cuvant[100]; // retinem cuvantulul
    sscanf(buff, "FIND %s", cuvant); 
// retinem pointerul catre finalul cuvantului cautat
    KeyTree last_letter = findKeyWord(treeCuvinte, cuvant);
// verificam daca cuvantul exista in arbore
    if (last_letter == NULL || last_letter->end == 0) {
        fprintf(fout, "EMPTY\n"); // nu exista cuvantul
        return;
    }
// cuvantul exista in arbore
    fprintf(fout, "%d", last_letter->referinte->size);
// afisam id-urile fisierelor din lista de referinta a cuvantului
    FileRef *current = last_letter->referinte->first;
    for (int i = 0; i < last_letter->referinte->size; i++) {
        fprintf(fout, " %s", current->file->id);
        current = current->next;
    }
    fprintf(fout, "\n");
}

void topk(char *buff, FILE *fout, KeyTree treeCuvinte)
{
    char cuvant[100]; // retinem cuvantulul
    int k; // nr de id-uri afisate
    sscanf(buff, "TOPK %s %d", cuvant, &k);
// retinem pointerul catre finalul cuvantului cautat
    KeyTree last_letter = findKeyWord(treeCuvinte, cuvant);
    if (last_letter == NULL || last_letter->end == 0) {
        fprintf(fout, "EMPTY\n"); // cuvantul nu exista
        return;
    }
    ListRef list = last_letter->referinte;
    FileRef *current = list->first;
    HeapID heapID = createHeapID(list->size); // crearea heap-ului
// verificam daca exista toate cele k fisiere de afisat
    if (list->size < k) {
        k = list->size;
    }
    fprintf(fout, "%d", k); // afisam k-ul
    for (int i = 0; i < list->size; i++) {
// adaugam in heap fiecare scor, respectiv fisier
        insertHeapID(heapID, current->file->scor, current->file);
        current = current->next;
    }
// afisarea primelor k fisiere din heap
    for (int i = 0; i < k; i++) {
        fprintf(fout, " %s", heapID->elem[0].fisier->id); // afisam maximul
        removeMaxHeapID(heapID); // stergem maxim pe care l am afisat
    }
    fprintf(fout, "\n");
    destroyHeapID(heapID);
}

void print(FILE *fout, ListFile *listaFisiere, KeyTree treeCuvinte)
{
    int capacitate = 100; // cata memorie am alocat cuvantului
    char *cuvant = malloc(100); // va fi retinut cuvantul_cheie
    if (cuvant == NULL) {
        return;
    }
    int nr_print = 0; // contor pentru nr de cuvinte-cheie afisate
// afisam cuvintele-cheie
    printKeyWords(fout, listaFisiere, treeCuvinte->child, 0, cuvant,
                  &capacitate, &nr_print);
    if (nr_print == 0) { 
        fprintf(fout, "EMPTY\n"); // nu s-a afisat nici un cuvant-cheie
    }
    free(cuvant);
}

void prefix(char *buff, FILE *fout, KeyTree treeCuvinte)
{
    char prefix[100];
    sscanf(buff, "PREFIX %s", prefix); // retinem prefixul
    // retinem pointerul catre finalul prefixului
    KeyTree last_letter = findKeyWord(treeCuvinte, prefix);
    if (last_letter == NULL) {
        fprintf(fout, "EMPTY\n"); // prefixul nu exista in arbore
        return;
    }
// crearea listei in care vor fi retinute fisierele
    ListPrefix listaPrefix = createListPrefix();
// verificam daca prefixul este un cuvant, are lista de referinta
    if (last_letter->end == 1 && last_letter->referinte->size > 0) {
// adaugam fisierele care sunt referinta pentru cuvintele din radacina
        FileRef *current = last_letter->referinte->first;
        while (current != NULL) {
            insertListPrefix(listaPrefix, current->file);
            current = current->next;
        }
    }
// adaugam fisierele care sunt referinta
// pentru cuvintele din subarborele fara radacina
    findFilewithPrefix(listaPrefix, last_letter->child);
    fprintf(fout, "%d", listaPrefix->size);
    FilePrefix *current = listaPrefix->first;
    while (current != NULL) {
        fprintf(fout, " %s",current->file->id);
        current = current->next;
    }
    fprintf(fout, "\n");
// eliberarea memoriei pentru lista in care au fost retinute fisierele
    destroyListPrefix(listaPrefix);
}

#endif