#ifndef STRUCTURI_H
#define STRUCTURI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- EMERGENCY UNITS MANAGEMENT ---

typedef struct Echipaj {
	int id;
	char type;
	int  availability;
	struct Echipaj *next;
	struct Echipaj *prev;
} Echipaj;

typedef struct ListEchipaje {
	Echipaj *first;
	Echipaj *last;
} ListEchipaje;

/* Initialize an empty doubly linked list for emergency units */
ListEchipaje *createListEchipaje(void)
{
	ListEchipaje *list = malloc(sizeof(ListEchipaje));
	list->first = NULL;
	list->last = NULL;
	return list;
}

/* Allocate and initialize a new emergency unit */
Echipaj *createEchipaj(int id, char type)
{
	Echipaj *new_echipaj = malloc(sizeof(Echipaj));
	if (new_echipaj == NULL) {
		return NULL;
	}
	new_echipaj->id = id;
	new_echipaj->type = type;
	new_echipaj->availability = 1;
	new_echipaj->next = NULL;
	new_echipaj->prev = NULL;
	return new_echipaj;
}

/* Insert a new unit at the end of the list */
void insertEchipaj(ListEchipaje *list, int id, char type)
{
	Echipaj *new_echipaj = createEchipaj(id, type);
	if (list->first == NULL) {
		list->first = new_echipaj;
		list->last = new_echipaj;
		return;
	}
	list->last->next = new_echipaj;
	new_echipaj->prev = list->last;
	list->last = new_echipaj;
}

/* Free all memory allocated for the units list */
void destroyListEchipaje(ListEchipaje* list)
{
	if(list == NULL) {
		return;
	}
	Echipaj *current = list->first;
	while(current != NULL) {
		Echipaj *delete = current;
		current = current->next;
		free(delete);
	}
	free(list);
}


// --- INCIDENTS MANAGEMENT ---

typedef struct Incident {
	int id;
	char priority[7];     // "high", "medium", or "low"
	char *description;
	char status[11];      // "queued", "intervened", or "solved"
	struct Incident *next;
	struct Incident *prev;
} Incident;

/* Create a dummy incident to serve as a sentinel node */
Incident *fictivIncident(void)
{
	Incident *incident_fictiv = malloc(sizeof(Incident));
	if (incident_fictiv == NULL) {
		return NULL;
	}
	incident_fictiv->id = 0;
	strcpy(incident_fictiv->priority, "low");
	incident_fictiv->description = malloc(15 * sizeof(char));
	if (incident_fictiv->description != NULL) {
		strcpy(incident_fictiv->description, "test incident");
	}
	strcpy(incident_fictiv->status, "solved");
	incident_fictiv->next = NULL;
	incident_fictiv->prev = NULL;
	return incident_fictiv;
}

typedef struct ListIncidente {
	Incident *santinela; 
} ListIncidente;

/* Initialize a circular doubly linked list for incidents with a sentinel */
ListIncidente *createListIncidente(void)
{
	ListIncidente *list = malloc(sizeof(ListIncidente));
	list->santinela = fictivIncident();
	list->santinela->next = list->santinela;
	list->santinela->prev = list->santinela;
	return list;
}

/* Allocate and initialize a new incident */
Incident *createIncident(int id, char *priority, char *descrip)
{
	Incident *new_incident = malloc(sizeof(Incident));
	if (new_incident == NULL) {
		return NULL;
	}
	new_incident->id = id;
	strcpy(new_incident->priority, priority);
	new_incident->description = malloc((strlen(descrip) + 1) * sizeof(char));
	if (new_incident->description != NULL) {
		strcpy(new_incident->description, descrip);
	}
	strcpy(new_incident->status, "queued");
	new_incident->next = NULL;
	new_incident->prev = NULL;
	return new_incident;
}

/* Insert an incident at the end of the circular list */
void insertIncident(ListIncidente *list, int id, char *priority,
					char *description)
{
	Incident *new_incident = createIncident(id, priority, description);
	Incident *last = list->santinela->prev; 
	
	new_incident->next = list->santinela;
	new_incident->prev = last;
	last->next = new_incident;
	list->santinela->prev = new_incident;
}

/* Free all memory allocated for the incidents list */
void destroyListIncident(ListIncidente *list)
{
	if (list == NULL) {
		return;
	}
	Incident *current = list->santinela->next;
	while (current != list->santinela) {
		if (current->description != NULL) {
			free(current->description);
		}
		Incident *delete = current;
		current = current->next;
		free(delete);
	}
	if (list->santinela->description != NULL) {
		free(list->santinela->description);
	}
	free(list->santinela);
	free(list);
}


// --- INTERVENTIONS MANAGEMENT ---

typedef struct Interventie {
	Incident *incident;
	Echipaj *echipaj;
	struct Interventie *next;
	struct Interventie *prev;
} Interventie;

typedef struct ListInterventii {
	Interventie *santinela; 
} ListInterventii;

/* Allocate a new intervention node */
Interventie *createInterventie(Incident *incident, Echipaj *echipaj)
{
	Interventie *new_interventie = malloc(sizeof(Interventie));
	if (new_interventie == NULL) {
		return NULL;
	}
	new_interventie->incident = incident;
	new_interventie->echipaj = echipaj;
	new_interventie->next = NULL;
	new_interventie->prev = NULL;
	return new_interventie;
}

/* Initialize a circular doubly linked list for interventions */
ListInterventii *createListInterventii(void)
{
	ListInterventii *list = malloc(sizeof(ListInterventii));
	list->santinela = createInterventie(NULL, NULL);
	list->santinela->next = list->santinela;
	list->santinela->prev = list->santinela;
	return list;
}

/* Insert an intervention at the end of the circular list */
void insertInterventie(ListInterventii *list, Incident *incident,
						Echipaj *echipaj)
{
	Interventie *new_interventie = createInterventie(incident, echipaj);
	Interventie *last = list->santinela->prev;
	
	new_interventie->next = list->santinela;
	new_interventie->prev = last;
	last->next = new_interventie;
	list->santinela->prev = new_interventie;
}

/* Remove a specific intervention from the list */
void deleteInterventie(ListInterventii *list, Interventie *interventie)
{
	if (list->santinela->next == list->santinela) {
		return;
	}
	interventie->prev->next = interventie->next;
	interventie->next->prev = interventie->prev;
	free(interventie);
}

/* Free all memory allocated for the interventions list */
void destroyListInterventii(ListInterventii *list)
{
	if (list == NULL) {
		return;
	}
	Interventie *current = list->santinela->next;
	while (current != list->santinela) {
		Interventie *delete = current;
		current = current->next;
		free(delete);
	}
	free(list->santinela);
	free(list);
}


// --- INCIDENT WAITING QUEUES ---

typedef struct IncidentAsteptare {
	Incident *incident;
	struct IncidentAsteptare *next;
} IncidentAsteptare;

typedef struct QueueAsteptare {
	IncidentAsteptare *front;
	IncidentAsteptare *rear;
	long size;
} QueueAsteptare;

/* Initialize an empty queue for waiting incidents */
QueueAsteptare* createQueueAsteptare(void)
{
	QueueAsteptare *coada= malloc(sizeof(QueueAsteptare));
	coada->front = NULL;
	coada->rear = NULL;
	coada->size = 0;
	return coada;
}

/* Enqueue an incident at the rear */
void enqueueAsteptare(QueueAsteptare *q, Incident *incident)
{
	IncidentAsteptare *new_incident = malloc(sizeof(IncidentAsteptare));
	new_incident->incident = incident;
	new_incident->next = NULL;
	if (q->front == NULL) {
		q->size++;
		q->front = new_incident;
		q->rear = new_incident;
		return;
	}
	q->size++;
	q->rear->next = new_incident;
	q->rear = new_incident;
}

/* Enqueue an incident at the front (used for UNDO operations) */
void enqueueAsteptareInceput(QueueAsteptare *q, Incident *incident)
{
	IncidentAsteptare *new_incident = malloc(sizeof(IncidentAsteptare));
	new_incident->incident = incident;
	new_incident->next = NULL;
	if (q->front == NULL) {
		q->size++;
		q->front = new_incident;
		q->rear = new_incident;
		return;
	}
	new_incident->next = q->front;
	q->size++;
	q->front = new_incident;
}

/* Dequeue an incident from the front */
void dequeueAsteptare(QueueAsteptare* q)
{
	if (q->front == NULL) {
		return;
	}
	IncidentAsteptare *delete = q->front;
	q->front = delete->next;
	free(delete);
	q->size--;
	if (q->size == 0) {
		q->rear = NULL;
	}
}

/* Free all memory allocated for the waiting queue */
void destroyQueueAsteptare(QueueAsteptare *q)
{
	if (q == NULL) {
		return;
	}
	IncidentAsteptare *current = q->front;
	while (current != NULL) {
		IncidentAsteptare *delete = current;
		current = current->next;
		free(delete);
	}
	free(q);
}


// --- UNITS AVAILABILITY QUEUE ---

typedef struct EchipajDisponibil {
	Echipaj *echipaj;
	struct EchipajDisponibil *next;
} EchipajDisponibil;

typedef struct QueueDisponibilitate {
	EchipajDisponibil *front;
	EchipajDisponibil *rear;
	long size;
} QueueDisponibilitate;

/* Initialize an empty queue for available emergency units */
QueueDisponibilitate* createQueueDisponibilitate(void)
{
	QueueDisponibilitate *coada= malloc(sizeof(QueueDisponibilitate));
	coada->front = NULL;
	coada->rear = NULL;
	coada->size = 0;
	return coada;
}

/* Enqueue an available unit at the rear */
void enqueueDisponibilitate(QueueDisponibilitate *q, Echipaj *echipaj)
{
	EchipajDisponibil *new_echipaj = malloc(sizeof(EchipajDisponibil));
	new_echipaj->echipaj = echipaj;
	new_echipaj->next = NULL;
	if (q->front == NULL) {
		q->size++;
		q->front = new_echipaj;
		q->rear = new_echipaj;
		return;
	}
	q->size++;
	q->rear->next = new_echipaj;
	q->rear = new_echipaj;
}

/* Dequeue a unit from the front (when dispatched) */
void dequeueDisponibilitate(QueueDisponibilitate* q)
{
	if (q->front == NULL) {
		return;
	}
	EchipajDisponibil *delete = q->front;
	q->front = delete->next;
	free(delete);
	q->size--;
	if (q->size == 0) {
		q->rear = NULL;
	}
}

/* Free all memory allocated for the availability queue */
void destroyQueueDisponibilitate(QueueDisponibilitate *q)
{
	if (q == NULL) {
		return;
	}
	EchipajDisponibil *current = q->front;
	while (current != NULL) {
		EchipajDisponibil *delete = current;
		current = current->next;
		free(delete);
	}
	free(q);
}


// --- DISPATCH HISTORY STACK ---

typedef struct Dispatch {
	Interventie *interventie;
	struct Dispatch *next;
} Dispatch;

typedef struct StackDispatch {
	struct Dispatch *head;  
	long size;
} StackDispatch;

/* Initialize an empty stack to track dispatch history */
StackDispatch *createStack(void)
{
	StackDispatch *stiva = malloc(sizeof(StackDispatch));
	stiva->head = NULL;
	stiva->size = 0;
	return stiva;
}

/* Push a new dispatch event onto the stack */
void pushDispatch(StackDispatch *stiva, Interventie *interventie)
{
	Dispatch *add = malloc(sizeof(Dispatch));
	add->interventie = interventie;
	add->next = NULL;
	if (stiva == NULL || stiva->size == 0) {
		stiva->head = add;
		stiva->size++;
		return;
	}
	add->next = stiva->head;
	stiva->head = add;
	stiva->size++;
}

/* Pop the most recent dispatch event from the stack */
void popDispatch(StackDispatch *stiva)
{
	if (stiva == NULL || stiva->size == 0) {
		return;
	}
	Dispatch *delete = stiva->head;
	stiva->head = delete->next;
	free(delete);
	stiva->size--;
}

/* Free all memory allocated for the dispatch stack */
void destroyStackDispatch(StackDispatch *stiva)
{
	if (stiva == NULL) {
		return;
	}
	Dispatch *aux = stiva->head;
	for (int i = 1; i < stiva->size; i++) {
		Dispatch *delete = aux;
		aux = aux->next;
		free(delete);
	}
	free(aux);
	free(stiva);
}

#endif
