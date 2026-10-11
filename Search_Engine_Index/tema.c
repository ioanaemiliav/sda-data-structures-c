/*Vizdoaga Emilia-Ioana - 314CD*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "arbore.h"
#include "tasks.h"
#include "max-heap.h"


void comanda(FILE *fout, char* buff, ListFile *listaFisiere, KeyTree treeCuvinte) 
/* analizez fiecare operatie de ce comanda apartine 
si apelez functia pentru comanda respectiva */
{
	if (strncmp(buff, "ADD ", 4) == 0) {
		add(buff, fout, listaFisiere, treeCuvinte);
	}
	if (strncmp(buff, "DEL ", 4) == 0) {
		del(buff, fout, listaFisiere, treeCuvinte);
	}
	if (strncmp(buff, "ADDKW", 5) == 0) {
		addkw(buff, fout, listaFisiere, treeCuvinte);
	}
	if (strncmp(buff, "DELKW", 5) == 0) {
		delkw(buff, fout, listaFisiere, treeCuvinte);
	}
	if (strncmp(buff, "FIND", 4) == 0) {
		find(buff, fout, treeCuvinte);
	}
	if (strncmp(buff, "TOPK", 4) == 0) {
		topk(buff, fout, treeCuvinte);
	}
    if (strncmp(buff, "PRINT", 5) == 0) {
		print(fout, listaFisiere, treeCuvinte);
	}
	if (strncmp(buff, "PREFIX", 6) == 0) {
		prefix(buff, fout, treeCuvinte);
	}
}

void input(ListFile *listaFisiere, KeyTree treeCuvinte)
//citesc si memorez datele de intrare
{
//deschid fisierul de citire
	FILE *fin = fopen("indexare.in", "rt");
	if (fin == NULL) {
		return; 
	}
//deschid fisierul de scriere
	FILE *fout = fopen("indexare.out", "wt");
	if (fout == NULL) {
		return; 
	}
// se va retine numarul de operatiii
	int nr_operatii;
	fscanf(fin, "%d ", &nr_operatii);
	char *buff = malloc(2000 * sizeof(char));
	if (!buff) {
		fprintf(fout, "Eroare de alocare.\n");
		return;
	}
	buff[0] = '\0';
	for (int i = 0; i < nr_operatii; i++) {
		fgets(buff, 2000, fin);
		buff[strcspn(buff, "\r\n")] = 0;
		comanda(fout, buff, listaFisiere, treeCuvinte);
	}
	free(buff);
	fclose(fin);
	fclose(fout);
}

int main(void)
{
// lista cu fisiere
	ListFile *listaFisiere = createListFile();
    if (listaFisiere == NULL) {
        return 0;
    }
// arborele cu cuvinte-cheie
    KeyTree treeCuvinte = createKeyTree('\0');
    if (treeCuvinte == NULL) {
        return 0;
    }

	input(listaFisiere, treeCuvinte);


// eliberarea memoriei
	destroyListFile(listaFisiere);
    destroytKeyTree(treeCuvinte);
    return 0;
}

