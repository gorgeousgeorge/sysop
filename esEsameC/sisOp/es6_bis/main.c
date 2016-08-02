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

//semaforo che 
pthread_mutex_t db_lock = PTHREAD_MUTEX_INITIALIZER;
//semaforo che garantisce mutua esclusione all'accesso degli operatori al db
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

//inizializza trip[] e op[]
void *inizializza(){
	int j;

	for (j=0;j<OPERATORS;j++){
		pthread_create( &op[j],NULL,operatore,NULL );
	}
}

void *operatore(){
	while(1){
		//generatore di numeri random usato da wait_client() e ask_confirmation()
		srand(time(NULL));
		//attende un cliente
		wait_client();
		//proteggo l'accesso dell'operatore al db 
		pthread_mutex_lock(&op_lock);
		active_op++;
		if (active_op==1){
			pthread_mutex_lock(&db_lock);
		}
		pthread_mutex_unlock(&op_lock);
		//controllo disponibilita'
		if (check_availability()){
			pthread_mutex_lock(&op_lock);
			active_op--;
			if (active_op==0){
				pthread_mutex_unlock(&db_lock);
			}
			pthread_mutex_unlock(&op_lock);
			//se il posto e' disponibile chiedo conferma della prenotazione
			if (ask_confirmation()){
				pthread_mutex_lock(&db_lock);
				//avuta la conferma dal cliente, ricontrollo se il posto sia ancora libero
				if (check_availability()){	
					//ricevuta la conferma, prenoto il posto e rilascio il lock sul db
					book_seat();
					pthread_mutex_unlock(&db_lock);
					//confermo al cliente l'avvenuta prenotazione
					confirm_booking();
				}
				else {
				pthread_mutex_unlock(&db_lock);
				printf("Ci dispiace, qualcuno ha prenotato il posto prima di lei\n");
				}
			}
			else {
				//prenotazione non confermata, rilascio il lock sul db
				pthread_mutex_lock(&op_lock);
				active_op--;
				if (active_op==0){
					pthread_mutex_unlock(&db_lock);
				}
				pthread_mutex_unlock(&op_lock);
				printf("Ci dispiace che abbia cambiato idea, speriamo di rivederla al piu' presto!\n");
			}
		}
		else {	
			pthread_mutex_lock(&op_lock);
			active_op--;
			if (active_op==0){
				pthread_mutex_unlock(&db_lock);
			}
			pthread_mutex_unlock(&op_lock);
			printf("Ci dispiace, il viaggio che le interessa e' gia' al completo! Speriamo di rivederla presto.\n");
			break;
		}
	}
}

void *wait_client(){
	int i;
	int delay;
	delay = rand();
	for (i=0;i<delay;i++) {
		//attendiamo un cliente!
	}
}

int check_availability(){
	//ritorna 1 se ci sono posti disponibili, 0 altrimenti
	int availability = seats>0 ? 1 : 0;
	return availability;	
}

int ask_confirmation(){
	//la decisione viene simulata generando un numero float random compreso tra [0;1]: 
	//se appartiene a [0;0.75) viene confermata la prenotazione.
	int confirmation = (float)((double)rand()/(double)RAND_MAX)<0.75 ? 1:0;	
	return confirmation;
}

void *book_seat(){
	seats--;
}

void *confirm_booking(){
	printf("L'operatore %u effettua una prenotazione!\n",pthread_self());
}
