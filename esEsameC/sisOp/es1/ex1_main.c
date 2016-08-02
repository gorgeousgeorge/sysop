#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//numero di cicli della body di ogni thread
#define ITER 50

//decommentare uno dei due seguenti blocchi in base all'algoritmo di mutua esclusione desiderato, e compilare:

//decommentare il seguente blocco  per implementare la mutua esclusione tramite l'algoritmo di Peterson
//compilare con: gcc -o nomeEseguibile main.c peterson.c -lpthread

#include "peterson.h"
//numero di thread 
#define nTh 3
//definisco gli alias per le funzioni di lock e unlock della libreria peterson.c
#define lock pLock(flag,turno,nTh,id)
#define unlock pUnlock(flag,id)
int flag[nTh];
int turno[nTh-1];


//decommentare il seguente blocco per implementare la mutua esclusione tramite l'algoritmo di Lamport
//compilare con: gcc -o nomeEseguibile main.c lamport.c -lpthread

/*
#include "lamport.h"
//numero di thread 
#define nTh 3
//definisco gli alias per le funzioni di lock e unlock della libreria peterson.c
#define lock lLock(ticket,turno,nTh,id)
#define unlock lUnlock(ticket,id)
int ticket[nTh];
int turno[nTh];
*/

//variabile globale condivisa tra i thread
int var;

pthread_t t1,t2,t3;

void *body1(void *arg);
void *body2(void *arg);
void *body3(void *arg);
void *usa(int tid,int var);

int main(int argc, char **argv){

	pthread_create(&t1,NULL,body1,(void *)0);
	pthread_create(&t2,NULL,body2,(void *)1);
	pthread_create(&t3,NULL,body3,(void *)2);
   
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
    
	return 0;
}

//i tre body dei thread: ciasuno chiama, in mutua esclusione con gli altri, due volte per ciclo 
//il metodo usa(int tid,int var) scambiando la variabile in utilizzo fra le due chiamate.
void *body1(void *arg){
	int id=(int)arg;
	while ( 1 ){
		lock;
		var=id;
		usa(id,var);
		unlock;
		lock;
		var=(id+1)%3;
		usa(id,var);
		unlock;
	}
}

void *body2(void *arg){
	int id=(int)arg;
	while( 1 ){
		lock;
		var=id;
		usa(id,var);
		unlock;
		lock;
		var=(id+1)%3;
		usa(id,var);
		unlock;        
	}
}

void *body3(void *arg){
	int id=(int)arg;
	while( 1 ){
		lock;
		var=id;
		usa(id,var);
		unlock;
		lock;
		var=(id+1)%3;
		usa(id,var);
		unlock;
	}
}

//chiama sleep() per il thread chiamante per un tempo random, poi stampa a schermo quale variabile sta venendo utilizzata e quale no
void *usa(int tid,int var){
	double t;
	t=(((float)rand())/RAND_MAX)*1e6;
	usleep(t);
	printf("|thread con id=%d usa var%d; non usa var%d|\n",tid+1,var,(var==tid)?(var+1)%3:(var-1==-1)?2:var-1);
}
