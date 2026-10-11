#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuri.h"

/* Parse input and add a new incident to the appropriate priority queue */
void add_incident(char * buff, ListIncidente *ListIncidente,
			 	  QueueAsteptare *queue_high, QueueAsteptare 
				  *queue_medium,QueueAsteptare *queue_low)
{
	int id;
	char priority[7];
	sscanf(buff, "ADD_INCIDENT %d %s", &id, priority);
	char *description = malloc(100);
	if (!description) {
		return;
	}
	description[0] = '\0';

	char *inceput_des = strchr(buff, '"');
	char *sfarsit_des = strrchr(buff, '"');
	int distanta = sfarsit_des - inceput_des - 1;

	if (distanta > 0) {
		strncpy(description, inceput_des+ 1, distanta);
		description[distanta] = '\0';
	} else {
		description[0] = '\0';
	}

	insertIncident(ListIncidente, id, priority, description);

	if (strcmp(priority, "low") == 0) {
		enqueueAsteptare(queue_low, ListIncidente->santinela->prev);
	}
	if (strcmp(priority, "medium") == 0) {
		enqueueAsteptare(queue_medium, ListIncidente->santinela->prev);
	}
	if (strcmp(priority, "high") == 0) {
		enqueueAsteptare(queue_high, ListIncidente->santinela->prev);
	}
	free(description);
}

/* Print the number of currently available emergency units */
void check_units_availability(FILE *fout, QueueDisponibilitate *queue_disp)
{
	int num_available_units= queue_disp->size;
	fprintf(fout, "Number of available units: %d\n", num_available_units);
}

/* Assign the highest priority incident to the first available unit */
void dispatch(FILE *fout, QueueDisponibilitate *queue_disp, QueueAsteptare
			  *queue_high, QueueAsteptare *queue_medium, QueueAsteptare 
			  *queue_low, ListInterventii *listaInterventii,
			  StackDispatch *stivaDispatch)
{
	if (queue_disp->front == NULL || (queue_high->front == NULL &&
		queue_medium->front == NULL && queue_low->front == NULL)) {
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	Echipaj *echipaj = queue_disp->front->echipaj;
	dequeueDisponibilitate(queue_disp);
	echipaj->availability = 0;

	Incident *incident;
	int ok = 0;
    
	if (queue_high->front) {
		ok = 1;
		incident = queue_high->front->incident;
		dequeueAsteptare(queue_high);
	}
	if (ok == 0 && queue_medium->front) {
		ok = 1;
		incident = queue_medium->front->incident;
		dequeueAsteptare(queue_medium);
	}
	if (ok == 0 && queue_low->front) {
		ok = 1;
		incident = queue_low->front->incident;
		dequeueAsteptare(queue_low);
	}
    
	strcpy(incident->status, "intervened");
	insertInterventie(listaInterventii, incident, echipaj);
	pushDispatch(stivaDispatch, listaInterventii->santinela->prev);
}

/* Revert the last active dispatch, freeing the unit and re-queuing the incident */
void undo_last_dispatch(FILE *fout, ListInterventii *listaInterventii,
						QueueDisponibilitate *queue_disp, QueueAsteptare
						*queue_high, QueueAsteptare *queue_medium, 
						QueueAsteptare *queue_low, StackDispatch 
						*stivaDispatch)
{
	while (stivaDispatch != NULL && stivaDispatch->size > 0 && 
		   strcmp(stivaDispatch->head->interventie->incident->status,
		   "solved") == 0) {
		popDispatch(stivaDispatch);
	}

	if (stivaDispatch == NULL || stivaDispatch->size == 0) {
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
		return;
	}
    
	Incident *incident = stivaDispatch->head->interventie->incident;
	Echipaj *echipaj = stivaDispatch->head->interventie->echipaj;

	strcpy(incident->status,"queued");
	if (strcmp(incident->priority, "high") == 0) {
		enqueueAsteptareInceput(queue_high, incident);
	} else if (strcmp(incident->priority, "medium") == 0) {
		enqueueAsteptareInceput(queue_medium, incident);
	} else if (strcmp(incident->priority, "low") == 0) {
		enqueueAsteptareInceput(queue_low, incident);
	}

	echipaj->availability = 1;
	enqueueDisponibilitate(queue_disp, echipaj);

	deleteInterventie(listaInterventii, stivaDispatch->head->interventie);
	popDispatch(stivaDispatch);
}

/* Mark a specific incident as solved and free the assigned unit */
void solved_incident(FILE *fout, char *buff, StackDispatch 
					 *stivaDispatch, QueueDisponibilitate *queue_disp)
{
	int id;
	sscanf(buff, "SOLVED_INCIDENT %d", &id);

	int ok = 0;
	Dispatch *aux = stivaDispatch->head;
	Incident *incident_current;
	Echipaj *echipaj_current;
    
	while (aux != NULL && ok == 0) {
		incident_current = aux->interventie->incident;
		echipaj_current = aux->interventie->echipaj;
		if (incident_current->id == id) {
			ok = 1;
		}
		aux = aux->next;
	}

	if (ok == 0) {
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	if (strcmp(incident_current->status, "intervened") != 0) {
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
		return;
	}
    
	strcpy(incident_current->status, "solved");
	echipaj_current->availability = 1;
	enqueueDisponibilitate(queue_disp, echipaj_current);
}

/* Display details for a specific emergency unit */
void show_unit(FILE *fout, char *buff, ListEchipaje *listaEchipaje)
{
	int id;
	sscanf(buff, "SHOW_ UNIT %d", &id);

	int ok = 0;
	Echipaj *aux = listaEchipaje->first;
	Echipaj *echipaj_current;
    
	while (aux != NULL && ok == 0) {
		echipaj_current = aux;
		if (echipaj_current->id == id) {
			ok = 1;
		}
		aux = aux->next;  
	}

	if (ok == 0) {
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
		return;
	}
    
	fprintf(fout, "Unit %d is type %c and is ", id, echipaj_current->type);
	if (echipaj_current->availability == 1) {
		fprintf(fout, "available\n");
	} else {
		fprintf(fout, "unavailable\n");
	}
}

/* Display details for a specific incident */
void show_incident(FILE *fout, char *buff, ListIncidente *listaIncidente)
{
	int id;
	sscanf(buff, "SHOW _INCIDENT %d", &id);

	int ok = 0;
	Incident *aux = listaIncidente->santinela->next;
	Incident *incident_current;
    
	while (aux != listaIncidente->santinela && ok == 0) {
	   incident_current = aux;
		if (incident_current->id == id) {
			ok = 1;
		}
		aux = aux->next;  
	}

	if (ok == 0) {
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
		return;
	}
    
	fprintf(fout, "Incident %d has %s ", id, incident_current->priority);
	fprintf(fout, "priority, the following description: \"");
	fprintf(fout, "%s\" and is ", incident_current->description);
	fprintf(fout, "%s\n", incident_current->status);
}

/* Display all registered interventions and their current status */
void show_interventions(FILE *fout, ListInterventii *listaInterventii)
{
	if (listaInterventii->santinela->next == listaInterventii->santinela) {
		fprintf(fout, "No intervention has been initiated\n");
	}

	Interventie *interventie = listaInterventii->santinela->next;
	while (interventie != listaInterventii->santinela) {
		Incident *incident = interventie->incident;
		Echipaj *echipaj = interventie->echipaj;
		fprintf(fout, "Incident %d was assigned ", incident->id);
		fprintf(fout, "to unit %d, and has the ", echipaj->id);
		fprintf(fout, "following status: \"%s\"\n", incident->status);
		interventie = interventie->next;
	}
}

#endif
