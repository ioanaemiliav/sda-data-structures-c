/*Vizdoaga Emilia-Ioana - 314CD*/

#ifndef ARBORE_H
#define ARBORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

typedef struct fileRef {
    File *file; // retinem un pointer catre fisierul din lista
    struct fileRef *next; // urmatorul pointer la fisier
    struct fileRef *prev; // anteriorul pointer la fisier
} FileRef;

typedef struct listRef {
    int size; // numarul de fisiere din lista de referinta
    FileRef *first;
    FileRef *last;
} *ListRef;

typedef struct keyTree{
    char litera; // retinem litera in arbore
    int end; // retinem daca litera termina un cuvant
    ListRef referinte ; // daca bool == 1
    struct keyTree *parent; // parintele nodului
    struct keyTree *child; // primul copil al nodului (cel mai din stanga)
    struct keyTree *sibling; // primul frate al nodului de pe acelasi nivel
} *KeyTree;

// creearea unui nod cu fisierul de referinta
FileRef *createFileRef(File *fisier)
{
    FileRef *file = malloc(sizeof(struct fileRef));
    if (file == NULL) {
        return NULL;
    }
    file->file = fisier;
    file->next = NULL;
    file->prev = NULL;
    return file;
}

// creearea listei cu pointeri
ListRef createListRef(void) {
    ListRef list = malloc(sizeof(struct listRef));
    if (list == NULL) {
        return list;
    }
    list->size = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

// creearea arborelui cu cuvinte-cheie
KeyTree createKeyTree(char letter)
{
    KeyTree tree = (KeyTree)malloc(sizeof(struct keyTree));
    if (tree == NULL) {
        return NULL;
    }
    tree->litera = letter; // retinem litera
    tree->end = 0;
    tree->referinte = createListRef();
    tree->parent = NULL;
    tree->child = NULL;
    tree->sibling = NULL;
    return tree;
}

// adaugarea unui pointer in lista
void insertListRef(ListRef list, File *fisier)
{   
    if (list->size == 0) { // lista este goala
        list->first = createFileRef(fisier);
        list->last = list-> first ;
        list->size++;
        return;
    }
    FileRef *current = list->first; 
    while (current != NULL && strcmp(fisier->id, current->file->id) > 0) {
        current = current->next;
    }
// current-> pointer la fisierul >= fisierul cautat, dpdv lexicografic 
    if (current != NULL && strcmp(fisier->id, current->file->id) == 0) {
        return; // fisierul exista deja in lista de referinta
    }
// crearea structurii cu pointerul catre fisierul cautat
    FileRef *new = createFileRef(fisier);
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

// inserarea in arbore
void insertTree(KeyTree tree, char *cuvant, File *file)
{
    KeyTree current = tree;
    int len = strlen(cuvant);
    for (int i = 0; i < len; i++) {
        char letter = cuvant[i];
// arborele curent nu are nici un copil
        if (current->child == NULL) {
            current->child = createKeyTree(letter);
            current->child->parent = current;
            current = current->child;
            continue;
        }
// litera e la inceput
        if (letter < current->child->litera) {
            KeyTree new = createKeyTree(letter);
            new->sibling = current->child;
            new->parent = current;
            current->child = new;
            current->child->parent = current;
            current = new;
            continue;
        }
// cautam litera in fratii sai
        KeyTree child = current->child;
        int ok = 0;
        while (child != NULL) {
            if (child->litera == letter) {
                ok = 1;
                current = child;
                break;
            }
            if (child->sibling == NULL || letter < child->sibling->litera) {
                break; 
            }
            child = child->sibling;
        }
        if (ok == 1) { // am gasit litera
            continue;
        }
        // inseram litera
        KeyTree new = createKeyTree(letter);
        new->sibling = child->sibling;
        new->parent = current;
        child->sibling = new;
        current->child->parent = current;
        current = new;
    }
    current->end = 1;
    insertListRef(current->referinte, file);
}

// eliminarea literelor din arbore care formeaza un cuvant fara lista de ref
void deleteLetter(KeyTree tree)
{
    if (tree == NULL || tree->parent == NULL || tree->referinte->size != 0
        || tree->child != NULL) {
        return;
    }
    KeyTree delete = tree;
    tree = tree->parent;
// daca nu termina un cuvant si nu are copii, se sterge
    while (tree != NULL && delete->end == 0 && delete->child == NULL) {
        if (tree->child == delete) { // actualizam primul copil al parintelui
            tree->child = delete->sibling;
        } else {
            KeyTree child = tree->child; // il stergem din lista de copii
            while (child != NULL && child->sibling != delete) {
                child = child->sibling;
            }
            if (child->sibling != NULL) {
                child->sibling = delete->sibling;
            }
        }
// eliberarea memoriei pentru lista de ref a nodului cu litera care se sterge
        if (delete->referinte != NULL) {
            free(delete->referinte);
        }
// eliberarea momoriei a modului care se sterge
        free(delete);
        delete = tree;
        tree = tree->parent;
    }
} 

// eliminarea fisierului de referinta
void deleteRefFile(KeyTree tree, File *fisier) {
    if (tree == NULL) {
        return;
    }
    KeyTree current = tree;
    while (current != NULL) {
        if (current->end == 1) { // formeaza un cuvant
// cautarea fisierului in lista de referinta a cuvantului gasit
            ListRef list = current->referinte; 
            FileRef *currentRef = list->first;
            FileRef *delete = NULL;
            while (currentRef != NULL) {
                if (currentRef->file == fisier) {
                    delete = currentRef;
                    break;
                }
                currentRef = currentRef->next;
            }
            if (delete != NULL) { // am gasit fisierul
                if (list->first == list->last && list->first == delete) {
// este singurul din lista
                    list->first = NULL;
                    list->last = NULL;
                } else if (list->first == delete) {
// este primul in lista
                    list->first = delete->next;
                    delete->next->prev = NULL;
                } else if (list->last== delete) {
// este ultimul in lista
                    list->last = delete->prev;
                    delete->prev->next = NULL;
                } else {
// este la mijloc
                    delete->prev->next = delete->next;
                    delete->next->prev = delete->prev;
                }
// stergem fisierul din lista de referinta
                free(delete);
                list->size--;
                if (list->size == 0) { // nu mai exista fisiere cu acest cuvant
                    current->end = 0;
// verificam daca este prefix pentru alt cuvant
                    if(current->child == NULL) {
                        KeyTree frate = current->sibling;
// stergem literele in plus
                        deleteLetter(current);
                        current = frate;
                    }
                    continue; 
                }
            }
        }
        if (current->child != NULL) { // repetam pentru cuvintele din copil
            deleteRefFile(current->child, fisier);
        }
        current = current->sibling; // repetam pentru cuvintele din frate
    }
}

// gasirea unui cuvant-cheie in arbore si 
// returnarea pointerului catre litera terminala
KeyTree findKeyWord(KeyTree tree, char *cuvant)
{
    if (tree == NULL || tree->child == NULL) {
        return NULL;
    }
    int len = strlen(cuvant);
    KeyTree current = tree->child;
    for (int i = 0; i < len; i++) {
        char litera = cuvant[i];
        while (current->litera != litera) {
            if (current->sibling == NULL) {
                return NULL; // nu exista litera
            }
            current = current->sibling;
        }
        if (current->litera == litera && i != len - 1) {
            if (current->child == NULL) {
                return NULL; // nu exista urmatoarea litera din cuvant
            }
            current = current->child; // trecem la cautarea urmatoarei litere
            continue;
        } else {
            break;
        }
    }
    return current;
}

// verificarea daca fisierul apartine listei de referinta a cuvantului
int containsFileRef(KeyTree tree, File *fisier)
{
    if (tree == NULL) {
        return 0;
    }
    ListRef list = tree->referinte;
    FileRef *current = list->first;
    while (current != NULL) {
        if (current->file == fisier) {
            return 1; // am gasit fisierul
        }
        current = current->next;
    }
    return 0;
}

// eliminarea doar unui cuvant-cheie
void deleteKeyWord(KeyTree tree, File *fisier)
{
// cautarea fisierului in lista de referinta a cuvantului gasit
    ListRef list = tree->referinte;
    FileRef *currentRef = list->first;
    FileRef *delete = NULL;
    while (currentRef != NULL) {
        if (currentRef->file == fisier) {
            delete = currentRef;
            break;
        }
        currentRef = currentRef->next;
    }
    if (delete != NULL) { // am gasit fisierul
        if (list->first == list->last && list->first == delete) {
// este singurul din lista
            list->first = NULL;
            list->last = NULL;
        } else if (list->first == delete) {
// este primul in lista
            list->first = delete->next;
            delete->next->prev = NULL;
        } else if (list->last== delete) {
// este ultimul in lista
            list->last = delete->prev;
            delete->prev->next = NULL;
        } else {
// este la mijloc
                delete->prev->next = delete->next;
                delete->next->prev = delete->prev;
        }
// stergem fisierul din lista de referinta
        free(delete);
        list->size--;
        if (list->size == 0) { // nu mai exista fisiere cu acest cuvant
            tree->end = 0;
// verificam daca este prefix pentru alt cuvant
            if(tree->child == NULL) {
                deleteLetter(tree); // stergem literele in plus
            } 
        }
    }
}

// verificarea daca mai exista cuvinte pentru un anumit fisier
int KeyWordsFile(KeyTree tree, File *fisier)
{
    if (tree == NULL) {
        return 0;
    }
// cautare prin lista de referinta a literelor ce formeaza un cuvant
    if (tree->end == 1) {
        FileRef *current = tree->referinte->first;
        while (current != NULL) {
            if (current->file == fisier) {
                return 1; // am gasit fisierul cautat
            }
            current = current->next;
        }
    }
    return KeyWordsFile(tree->child, fisier) + KeyWordsFile(tree->sibling, fisier);
}

// afisarea cuvintelor din arbore
void printKeyWords(FILE* fout,  ListFile *listaFisiere, KeyTree tree,
                   int idx_cuvant, char *cuvant, int *capacitate, int *nr_print)
{
    if (tree == NULL) {
        return;
    }
    cuvant[idx_cuvant] = tree->litera; // retinem litera
    cuvant[idx_cuvant + 1] = '\0';
    if (tree->end == 1) { // am gasit un cuvant
        (*nr_print)++;
        fprintf(fout, "%s %d", cuvant, tree->referinte->size);
        if (tree->referinte->size != 0) {
// afisam fisierele de referinta ale cuvantului
            FileRef *current = tree->referinte->first;
            for (int i = 0; i < tree->referinte->size; i++) {
                fprintf(fout, " %s", current->file->id);
                current = current->next;
            }
            fprintf(fout, "\n");
        }
    }
    idx_cuvant++; // trecem la urmatoarea litera din cuvant
    printKeyWords(fout, listaFisiere, tree->child, idx_cuvant, cuvant, capacitate, nr_print);
    printKeyWords(fout, listaFisiere, tree->sibling, idx_cuvant - 1, cuvant, capacitate, nr_print);
}

// eliberarea memoriei arborelui
void destroytKeyTree(KeyTree tree)
{
	if(tree == NULL) {
		return;
	}
    if (tree->referinte != NULL) {
        FileRef *current = tree->referinte->first;
	    while(current != NULL) {
		    FileRef *delete = current;
		    current = current->next;
		    free(delete);
	    }
        free(tree->referinte);
    }
	destroytKeyTree(tree->child);
    destroytKeyTree(tree->sibling);
	free(tree);
}

#endif