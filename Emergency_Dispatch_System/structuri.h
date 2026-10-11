/*Vizdoaga Emilia-Ioana - 314CD*/

#ifndef STRUCTURI_H
#define STRUCTURI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// SALVAREA ECHIPAJELOR

// struct init - echipajele salvate in lista
typedef struct Echipaj {
	int id;
	char type;
	int  availability;
	struct Echipaj *next;
	struct Echipaj *prev;
} Echipaj;

// lista in care memorez echipajele
typedef struct ListEchipaje {
	Echipaj *first;
	Echipaj *last;
} ListEchipaje;

// creez lista de echipaje
ListEchipaje *createListEchipaje(void)
{
	ListEchipaje *list = malloc(sizeof(ListEchipaje));
	list->first = NULL;
	list->last = NULL;
	return list;
}

// creez un echipaj
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

// adaugarea echipajului la finalul listei
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

// eliberarea memoriei listei cu echipaje
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

// SALVAREA INCIDENTELOR

// struct incident - incidente salvate in lista
typedef struct Incident {
	int id;
	char priority[7];     //   "high"/"medium"/"low"
	char *description;
	char status[11];      //   "queued"/"intervened"/"solved"
	struct Incident *next;
	struct Incident *prev;
} Incident;

// incident fictiv-folosit si ca santinela
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

// lista in care pastrez santinela
typedef struct ListIncidente {
	Incident *santinela; 
} ListIncidente;

// lista cu incidente in care am memorat incidentul fictiv
ListIncidente *createListIncidente(void)
{
	ListIncidente *list = malloc(sizeof(ListIncidente));
	list->santinela = fictivIncident();
	list->santinela->next = list->santinela;
	list->santinela->prev = list->santinela;
	return list;
}

// creez un incident
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

// adaugarea incidentului la finalul listei
void insertIncident(ListIncidente *list, int id, char *priority,
					char *description)
{
	Incident *new_incident = createIncident(id, priority, description);
	Incident *last = list->santinela->prev; // ultimul incident memorat
	new_incident->next = list->santinela;
	new_incident->prev = last;
	last->next = new_incident;
	list->santinela->prev = new_incident;
}

// eliberez memoria pentru lista de incidente
void destroyListIncident(ListIncidente *list)
{
	if (list == NULL) {
		return;
	}
	Incident *current = list->santinela->next;
	while (current != list->santinela) {
// se parcurg toate incidentele pana la santinela
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

// INTERVENTIILE

// struct intervention - interventiile salvate in lista
typedef struct Interventie {
// struct incident *incident - detalii incident
	Incident *incident;
// struct unit *unit - detalii echipaj de interventie
	Echipaj *echipaj;
	struct Interventie *next;
	struct Interventie *prev;
} Interventie;

// lista in care pastrez santinela
typedef struct ListInterventii {
	Interventie *santinela; 
} ListInterventii;

// creez o interventie
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

// creez lista cu interventii
ListInterventii *createListInterventii(void)
{
	ListInterventii *list = malloc(sizeof(ListInterventii));
	list->santinela = createInterventie(NULL, NULL);
	list->santinela->next = list->santinela;
	list->santinela->prev = list->santinela;
	return list;
}

// adaugarea interventiei la finalul listei
void insertInterventie(ListInterventii *list, Incident *incident,
						Echipaj *echipaj)
{
	Interventie *new_interventie = createInterventie(incident, echipaj);
// ultima interventie memorata
	Interventie *last = list->santinela->prev;
	new_interventie->next = list->santinela;
	new_interventie->prev = last;
	last->next = new_interventie;
	list->santinela->prev = new_interventie;
}

// stergerea unei interventii din lista
void deleteInterventie(ListInterventii *list, Interventie *interventie)
{
	if (list->santinela->next == list->santinela) {
		return;
	}
	interventie->prev->next = interventie->next;
	interventie->next->prev = interventie->prev;
	free(interventie);
}

// eliberez memoria pentru lista de interventii
void destroyListInterventii(ListInterventii *list)
{
	if (list == NULL) {
		return;
	}
	Interventie *current = list->santinela->next;
	while (current != list->santinela) {
// se parcurg toate incidentele pana la santinela
		Interventie *delete = current;
		current = current->next;
		free(delete);
	}
	free(list->santinela);
	free(list);
}

// COZILE DE ASTEPTARE

// memorez pointerul catre incidentul care asteapta
typedef struct IncidentAsteptare {
	Incident *incident;
	struct IncidentAsteptare *next;
}IncidentAsteptare;

// cozile in care memorez incidentul in functie de gravitatea lui
typedef struct QueueAsteptare {
	IncidentAsteptare *front;
	IncidentAsteptare *rear;
	long size;
}QueueAsteptare;

// creez coada de asteptare
QueueAsteptare* createQueueAsteptare(void)
{
	QueueAsteptare *coada= malloc(sizeof(QueueAsteptare));
	coada->front = NULL;
	coada->rear = NULL;
	coada->size = 0;
	return coada;
}

// adaug incidentului la finalul cozii
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

// adaugarea incidentului la inceputul cozii
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

// stergerea incidentului de la inceputul cozii
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

// eliberez memoria pentru coada de asteptare
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

// COADA DE DISPONIBILITATE


// memorez pointerul catre echipajul disponibil
typedef struct EchipajDisponibil {
	Echipaj *echipaj;
	struct EchipajDisponibil *next;
}EchipajDisponibil;

// cozile in care memorez incidentul in functie de gravitatea lui
typedef struct QueueDisponibilitate {
	EchipajDisponibil *front;
	EchipajDisponibil*rear;
	long size;
}QueueDisponibilitate;

// creez o coada pentru echipajele disponibile
QueueDisponibilitate* createQueueDisponibilitate(void)
{
	QueueDisponibilitate *coada= malloc(sizeof(QueueDisponibilitate));
	coada->front = NULL;
	coada->rear = NULL;
	coada->size = 0;
	return coada;
}

// adaugarea echipajului disponibil la finalul cozii
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

// stergerea echipajului disponibil de la inceputul cozii
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

// eliberez memoria pentru echipajul disponibil
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

// STIVA PENTRU DISPATCH

// memorez pointerul catre interventie si echipaj
typedef struct Dispatch {
	Interventie *interventie;
	struct Dispatch *next;
} Dispatch;

// stiva in care se memoreaza interventiile care au fost demarate
typedef struct StackDispatch {
	struct Dispatch *head;  
	long size;
} StackDispatch;

// creez stiva cu interventiile demarate
StackDispatch *createStack(void)
{
	StackDispatch *stiva = malloc(sizeof(StackDispatch));
	stiva->head = NULL;
	stiva->size = 0;
	return stiva;
}

// memorez o noua interventie in stiva
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

// elimin ultima interventie memorata in stiva
void popDispatch(StackDispatch *stiva)
{
	if (stiva == NULL || stiva->size == 0) {
		return;
	}
	Dispatch *delete= stiva->head;
	stiva->head = delete->next;
	free(delete);
	stiva->size--;
}

// eliberez memoria alocata stivei pentru dispatch
void destroyStackDispatch(StackDispatch *stiva)
{
	if (stiva == NULL) {
		return;
	}
	Dispatch *aux= stiva->head;
	for (int i = 1; i < stiva->size; i++) {
		Dispatch*delete = aux;
		aux = aux->next;
		free(delete);
	}
	free(aux);
	free(stiva);
}

#endif
