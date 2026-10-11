#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "arbore.h"
#include "tasks.h"
#include "max-heap.h"

/* Parse each operation and dispatch to the corresponding command handler */
void comanda(FILE *fout, char* buff, ListFile *listaFisiere, KeyTree treeCuvinte) 
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

/* Parse input data and process all commands */
void input(ListFile *listaFisiere, KeyTree treeCuvinte)
{
	FILE *fin = fopen("indexare.in", "rt");
	if (fin == NULL) {
		return; 
	}

	FILE *fout = fopen("indexare.out", "wt");
	if (fout == NULL) {
		return; 
	}

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
	ListFile *listaFisiere = createListFile();
    if (listaFisiere == NULL) {
        return 0;
    }

    KeyTree treeCuvinte = createKeyTree('\0');
    if (treeCuvinte == NULL) {
        return 0;
    }

    // Start processing input commands
	input(listaFisiere, treeCuvinte);

    // Free allocated memory to prevent memory leaks
	destroyListFile(listaFisiere);
    destroytKeyTree(treeCuvinte);
    
    return 0;
}
