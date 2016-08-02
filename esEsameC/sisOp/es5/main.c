#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define righe 20
#define colonne 80

//thread lettori
pthread_t t1,t2,t3;
//mutex usato per garantire mutua esclusione nell'accesso all'indice riga della matrice da leggere
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

typedef struct
    {
        char** pt[righe][colonne];
        int index;
    } T_text;
//struttura T_text condivisa dai thread
T_text testo;
T_text *testoPt=&testo;

void *readWriteFunction (void* text);

int main(int argc, char** argv)
{
    int i,j;

    //inizializza la matrice da leggere
    for(i=0; i<righe; i++){
        for(j=0; j<colonne; j++){            
            testoPt->pt[i][j]='#';
            }
        }
    
    pthread_create(&t1,NULL,readWriteFunction,(void *)testoPt);
    pthread_create(&t2,NULL,readWriteFunction,(void *)testoPt);
    pthread_create(&t3,NULL,readWriteFunction,(void *)testoPt);
    
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    
    return 0;
}

//body dei thread lettori
void *readWriteFunction(void *text){
    double t;
    int k;
    T_text *testo=(T_text *)text;

    while(1){
	//tenta di acquisire il lock sul mutex
        pthread_mutex_lock(&mutex);
	//se si e' raggiunta l'ultima riga della matrice
        if(testo->index==righe){
	    //rilascia il lock sul mutex ed esce dal while
            pthread_mutex_unlock(&mutex);
            break;
        }
	//altrimenti stampa la riga corrente	
        printf("Sono il thread %lu. Stampo riga %d:    ",pthread_self(),testo->index);
        for(k=0;k<colonne;k++){
            printf("%c",testo->pt[testo->index][k]);
        }
        printf("\n");
	//incrementa l'indice della prossima riga da leggere
        testo->index++;
	//rilascia il lock sul mutex
        pthread_mutex_unlock(&mutex);
	//il thread dorme per un periodo random inferiore ad 1 secondo
	t=(((float)rand())/RAND_MAX)*1e6;
	usleep(t);
	}
    }
