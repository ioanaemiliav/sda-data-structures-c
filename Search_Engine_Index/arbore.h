#ifndef ARBORE_H
#define ARBORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

typedef struct fileRef {
    File *file; 
    struct fileRef *next; 
    struct fileRef *prev; 
} FileRef;

typedef struct listRef {
    int size; 
    FileRef *first;
    FileRef *last;
} *ListRef;

typedef struct keyTree {
    char litera; 
    int end; 
    ListRef referinte; 
    struct keyTree *parent; 
    struct keyTree *child; 
    struct keyTree *sibling; 
} *KeyTree;

/* Initialize a new reference node for file tracking */
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

/* Initialize an empty reference list */
ListRef createListRef(void) 
{
    ListRef list = malloc(sizeof(struct listRef));
    if (list == NULL) {
        return list;
    }
    list->size = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

/* Initialize a new Trie node with a given character */
KeyTree createKeyTree(char letter)
{
    KeyTree tree = (KeyTree)malloc(sizeof(struct keyTree));
    if (tree == NULL) {
        return NULL;
    }
    tree->litera = letter;
    tree->end = 0;
    tree->referinte = createListRef();
    tree->parent = NULL;
    tree->child = NULL;
    tree->sibling = NULL;
    return tree;
}

/* Insert a file reference into the reference list in lexicographical order */
void insertListRef(ListRef list, File *fisier)
{    
    if (list->size == 0) {
        list->first = createFileRef(fisier);
        list->last = list->first;
        list->size++;
        return;
    }
    FileRef *current = list->first; 
    while (current != NULL && strcmp(fisier->id, current->file->id) > 0) {
        current = current->next;
    }
    
    if (current != NULL && strcmp(fisier->id, current->file->id) == 0) {
        return; 
    }
    
    FileRef *new = createFileRef(fisier);
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

/* Insert a keyword and its associated file into the Trie */
void insertTree(KeyTree tree, char *cuvant, File *file)
{
    KeyTree current = tree;
    int len = strlen(cuvant);
    for (int i = 0; i < len; i++) {
        char letter = cuvant[i];
        
        if (current->child == NULL) {
            current->child = createKeyTree(letter);
            current->child->parent = current;
            current = current->child;
            continue;
        }
        
        if (letter < current->child->litera) {
            KeyTree new = createKeyTree(letter);
            new->sibling = current->child;
            new->parent = current;
            current->child = new;
            current->child->parent = current;
            current = new;
            continue;
        }
        
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
        if (ok == 1) {
            continue;
        }
        
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

/* Remove redundant Trie nodes that no longer form valid words or have children */
void deleteLetter(KeyTree tree)
{
    if (tree == NULL || tree->parent == NULL || tree->referinte->size != 0
        || tree->child != NULL) {
        return;
    }
    KeyTree delete = tree;
    tree = tree->parent;
    
    while (tree != NULL && delete->end == 0 && delete->child == NULL) {
        if (tree->child == delete) {
            tree->child = delete->sibling;
        } else {
            KeyTree child = tree->child; 
            while (child != NULL && child->sibling != delete) {
                child = child->sibling;
            }
            if (child->sibling != NULL) {
                child->sibling = delete->sibling;
            }
        }
        
        if (delete->referinte != NULL) {
            free(delete->referinte);
        }
        free(delete);
        delete = tree;
        tree = tree->parent;
    }
} 

/* Remove a file reference from all matching keywords in the Trie */
void deleteRefFile(KeyTree tree, File *fisier) 
{
    if (tree == NULL) {
        return;
    }
    KeyTree current = tree;
    while (current != NULL) {
        if (current->end == 1) {
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
            if (delete != NULL) {
                if (list->first == list->last && list->first == delete) {
                    list->first = NULL;
                    list->last = NULL;
                } else if (list->first == delete) {
                    list->first = delete->next;
                    delete->next->prev = NULL;
                } else if (list->last == delete) {
                    list->last = delete->prev;
                    delete->prev->next = NULL;
                } else {
                    delete->prev->next = delete->next;
                    delete->next->prev = delete->prev;
                }
                
                free(delete);
                list->size--;
                if (list->size == 0) {
                    current->end = 0;
                    if(current->child == NULL) {
                        KeyTree frate = current->sibling;
                        deleteLetter(current);
                        current = frate;
                    }
                    continue; 
                }
            }
        }
        if (current->child != NULL) {
            deleteRefFile(current->child, fisier);
        }
        current = current->sibling; 
    }
}

/* Search for a keyword in the Trie and return the pointer to its terminal node */
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
                return NULL; 
            }
            current = current->sibling;
        }
        if (current->litera == litera && i != len - 1) {
            if (current->child == NULL) {
                return NULL; 
            }
            current = current->child; 
            continue;
        } else {
            break;
        }
    }
    return current;
}

/* Check if a file belongs to the reference list of a keyword */
int containsFileRef(KeyTree tree, File *fisier)
{
    if (tree == NULL) {
        return 0;
    }
    ListRef list = tree->referinte;
    FileRef *current = list->first;
    while (current != NULL) {
        if (current->file == fisier) {
            return 1; 
        }
        current = current->next;
    }
    return 0;
}

/* Remove a specific keyword association for a file */
void deleteKeyWord(KeyTree tree, File *fisier)
{
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
    if (delete != NULL) {
        if (list->first == list->last && list->first == delete) {
            list->first = NULL;
            list->last = NULL;
        } else if (list->first == delete) {
            list->first = delete->next;
            delete->next->prev = NULL;
        } else if (list->last == delete) {
            list->last = delete->prev;
            delete->prev->next = NULL;
        } else {
                delete->prev->next = delete->next;
                delete->next->prev = delete->prev;
        }
        
        free(delete);
        list->size--;
        if (list->size == 0) {
            tree->end = 0;
            if(tree->child == NULL) {
                deleteLetter(tree); 
            } 
        }
    }
}

/* Check if a file has any remaining keywords associated in the Trie */
int KeyWordsFile(KeyTree tree, File *fisier)
{
    if (tree == NULL) {
        return 0;
    }
    if (tree->end == 1) {
        FileRef *current = tree->referinte->first;
        while (current != NULL) {
            if (current->file == fisier) {
                return 1; 
            }
            current = current->next;
        }
    }
    return KeyWordsFile(tree->child, fisier) + KeyWordsFile(tree->sibling, fisier);
}

/* Recursively print all indexed keywords in lexicographical order */
void printKeyWords(FILE* fout, ListFile *listaFisiere, KeyTree tree,
                   int idx_cuvant, char *cuvant, int *capacitate, int *nr_print)
{
    if (tree == NULL) {
        return;
    }
    cuvant[idx_cuvant] = tree->litera; 
    cuvant[idx_cuvant + 1] = '\0';
    if (tree->end == 1) {
        (*nr_print)++;
        fprintf(fout, "%s %d", cuvant, tree->referinte->size);
        if (tree->referinte->size != 0) {
            FileRef *current = tree->referinte->first;
            for (int i = 0; i < tree->referinte->size; i++) {
                fprintf(fout, " %s", current->file->id);
                current = current->next;
            }
            fprintf(fout, "\n");
        }
    }
    idx_cuvant++; 
    printKeyWords(fout, listaFisiere, tree->child, idx_cuvant, cuvant, capacitate, nr_print);
    printKeyWords(fout, listaFisiere, tree->sibling, idx_cuvant - 1, cuvant, capacitate, nr_print);
}

/* Free all memory allocated for the Trie structure */
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
