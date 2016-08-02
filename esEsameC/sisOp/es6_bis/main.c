#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#define DELAY 10000
//numero di posti prenotabili per il viaggio
#define TRIP_SIZE 20	
//numero di operatori nell'agenzia di viaggio
#define OPERATORS 3 

pthread_mutex_t db_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t op_lock = PTHREAD_MUTEX_INITIALIZER;

int seats=TRIP_SIZE;
int active_op;

pthread_t op[OPERATORS];	

void *confirm_booking();
void *inizializza();
void *operatore();
void *wait_client();
int check_availability();
int ask_confirmation();
void *book_seat();

void main(int argc, char *args){
	int k;

	inizializza();
	
	for(k=0;k<OPERATORS;k++){
		pthread_join(op[k],NULL);
	}

	printf("Agenzia chiusa!\n");
}

//inizializza i threads operatore 
void *inizializza(){
	int j;

	for (j=0;j<OPERATORS;j++){
		pthread_create( &op[j],NULL,operatore,NULL );
	}
}

//body dei threads operatore 
void *operatore(){
	while(1){
		//inizializza il generatore di numeri random 
		//usato dai metodi wait_client() e ask_confirmation()
		srand(time(NULL));

		wait_client();

		pthread_mutex_lock(&op_lock);
		active_op++;
		//se e' il primo operatore attivo sul database ne acquisisce il lock
		//per consultarlo in cerca di posti liberi
		if (active_op==1){
			pthread_mutex_lock(&db_lock);
		}
		pthread_mutex_unlock(&op_lock);

		//controllo disponibilita' posti
		if (check_availability()){
			//Posto disponibile.
			//terminata la consultazione del db, rilascia il lock sul db se
			//e' l'ultimo operatore attivo
			pthread_mutex_lock(&op_lock);
			active_op--;
			if (active_op==0){
				pthread_mutex_unlock(&db_lock);
			}
			pthread_mutex_unlock(&op_lock);

			//chiedo conferma della prenotazione
			if (ask_confirmation()){
				//prenotazione confermata
				//tenta di acquisire lock sul db per prenotare il posto
				pthread_mutex_lock(&db_lock);
				//l'operatore ha il lock: ricontrolla se il posto sia ancora libero
				if (check_availability()){	
					//prenota il posto e rilascia il lock sul db
					book_seat();
					pthread_mutex_unlock(&db_lock);
					//conferma al cliente l'avvenuta prenotazione
					confirm_booking();
				}
				else {
					//qualcuno e' stato piu' rapido a confermare e sono finiti i posti.
					//Rilascia il lock sul db
					pthread_mutex_unlock(&db_lock);
					printf("Ci dispiace, qualcuno ha prenotato il posto prima di lei\n");
				}
			}
			else {
				//prenotazione non confermata

				pthread_mutex_lock(&op_lock);
				active_op--;
				//se era l'ultimo operatore attivo sul database, rilascia il lock sul db
				if (active_op==0){
					pthread_mutex_unlock(&db_lock);
				}
				pthread_mutex_unlock(&op_lock);

				printf("Ci dispiace che abbia cambiato idea, speriamo di rivederla al piu' presto!\n");
			}
		}
		else {	
			//non ci sono posti disponibili.
			pthread_mutex_lock(&op_lock);
			active_op--;
			//se era l'ultimo operatore attivo sul database, rilascia il lock sul db
			if (active_op==0){
				pthread_mutex_unlock(&db_lock);
			}
			pthread_mutex_unlock(&op_lock);
			printf("Ci dispiace, il viaggio che le interessa e' gia' al completo! Speriamo di rivederla presto.\n");

			break;
		}
	}
}

//simula l'attesa di un cliente
void *wait_client(){
	int i;
	int delay;
	delay = rand();
	for (i=0;i<delay;i++) {
		//attendiamo un cliente!
	}
}

//ritorna 1 se ci sono posti disponibili, 0 altrimenti
int check_availability(){
	int availability = seats>0 ? 1 : 0;
	return availability;	
}

//genera un numero float random compreso tra [0;1]: 
//se appartiene a [0;0.75) viene confermata la prenotazione.
int ask_confirmation(){
	int confirmation = (float)((double)rand()/(double)RAND_MAX)<0.75 ? 1:0;	
	return confirmation;
}

//prenota un posto 
void *book_seat(){
	seats--;
}

//conferma l'avvenuta prenotazione
void *confirm_booking(){
	printf("L'operatore %u effettua una prenotazione!\n",pthread_self());
}
