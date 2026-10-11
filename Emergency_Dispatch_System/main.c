/*Vizdoaga Emilia-Ioana - 314CD*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuri.h"
#include "tasks.h"

void comanda(FILE *fout, char* buff, ListEchipaje *listaEchipaje,
			 ListIncidente *listaIncidente,QueueAsteptare *queue_high,
			 QueueAsteptare *queue_medium, QueueAsteptare *queue_low,
			 QueueDisponibilitate *queue_disp, ListInterventii 
			 *listaInterventii, StackDispatch *stivaDispatch) 
/* analizez fiecare operatie de ce comanda apartine 
si apelez functia pentru comanda respectiva */
{
	if (strncmp(buff, "ADD_INCIDENT", 12) == 0) {
		add_incident(buff, listaIncidente, queue_high, 
					 queue_medium, queue_low);
	}
	if (strcmp(buff, "CHECK_UNITS_AVAILABILITY") == 0) {
		check_units_availability(fout, queue_disp);
	}
	if (strcmp(buff, "DISPATCH") == 0) {
		dispatch(fout, queue_disp, queue_high, queue_medium,
				 queue_low, listaInterventii, stivaDispatch);
	}
	if (strcmp(buff, "UNDO_LAST_DISPATCH") == 0) {
		undo_last_dispatch(fout, listaInterventii, queue_disp,
						   queue_high, queue_medium, queue_low,
						   stivaDispatch);
	}
	if (strncmp(buff, "SOLVED_INCIDENT", 15) == 0) {
		solved_incident(fout, buff, stivaDispatch, queue_disp);
	}
	if (strncmp(buff, "SHOW_UNIT", 9) == 0) {
		show_unit(fout, buff, listaEchipaje);
	}
	if (strncmp(buff, "SHOW_INCIDENT", 13) == 0) {
		show_incident(fout, buff, listaIncidente);
	}
	 if (strcmp(buff, "SHOW_INTERVENTIONS") == 0) {
		show_interventions(fout,listaInterventii);
	}
}

void input(ListEchipaje *listaEchipaje, ListIncidente *listaIncidente,
		   QueueAsteptare *queue_high, QueueAsteptare *queue_medium,
		   QueueAsteptare *queue_low, QueueDisponibilitate *queue_disp,
		   ListInterventii *listaInterventii, StackDispatch *stivaDispatch)
//citesc si memorez datele de intrare
{
	//deschid fisierul de citire
	FILE *fin = fopen("tema1.in", "rt");
	if (fin == NULL) {
		return; 
	}
	//deschid fisierul de scriere
	FILE *fout = fopen("tema1.out", "wt");
	if (fout == NULL) {
		return; 
	}
// se va retine numarul de echipaje, apoi cel de operatii
	int nr_echipaje, nr_operatii;
	fscanf(fin, "%d", &nr_echipaje);
// memorez fiecare echipaj citit
	for (int i = 0; i < nr_echipaje; i++) {
		int id;
		char type;
		fscanf(fin, "%d %c ", &id, &type);
		insertEchipaj(listaEchipaje, id, type);
		enqueueDisponibilitate(queue_disp, listaEchipaje->last);
	}
	fscanf(fin, "%d ", &nr_operatii);
	char *buff = malloc(100 * sizeof(char));
	if (!buff) {
		fprintf(fout, "Eroare de alocare.\n");
		return;
	}
	buff[0] = '\0';
	for (int i = 0; i < nr_operatii; i++) {
		fgets(buff, 100, fin);
		buff[strcspn(buff, "\r\n")] = 0;
		comanda(fout, buff, listaEchipaje, listaIncidente, queue_high,
				queue_medium, queue_low, queue_disp, listaInterventii,
				stivaDispatch);
		
	}
	free(buff);
	fclose(fin);
	fclose(fout);
}

int main(void)
{
// lista cu echipaje
	ListEchipaje *listaEchipaje = createListEchipaje();
// lista cu incidentele
	ListIncidente *listaIncidente = createListIncidente();
// cozile cu incidentele in asteptare
	QueueAsteptare *queue_high = createQueueAsteptare(); 
	QueueAsteptare *queue_medium = createQueueAsteptare();
	QueueAsteptare *queue_low = createQueueAsteptare();
// coada in care memorez echipajele disponibile
	QueueDisponibilitate *queue_disp = createQueueDisponibilitate();
// lista cu interventiile
	ListInterventii *listaInterventii = createListInterventii();
// stiva in care memorez interventiile
	StackDispatch *stivaDispatch = createStack();
// prelucrarea datele
	input(listaEchipaje, listaIncidente, queue_high, queue_medium,
		  queue_low, queue_disp, listaInterventii, stivaDispatch);
// eliberarea memoriei
	destroyListEchipaje(listaEchipaje);
	destroyListIncident(listaIncidente);
	destroyQueueAsteptare(queue_high);
	destroyQueueAsteptare(queue_medium);
	destroyQueueAsteptare(queue_low);
	destroyQueueDisponibilitate(queue_disp);
	destroyListInterventii(listaInterventii);
	destroyStackDispatch(stivaDispatch);
}
