//Richiede l'utilizzo della Gnu Scientific Library. Testata con la versione gsl-2.1-2 per processori x86_64 del repository extra di Arch Linux.
//Se si usa il compilatore gcc, compilare con: 
//gcc -lgsl -lgslcblas -o main
//Per eseguire il main, bisogna che prima sia stato compilato anche receiver.c 
//con l'istruzione:
//gcc receiver.c -o receiver

#include <stdlib.h>
#include <gsl/gsl_randist.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <gsl/gsl_rng.h>
#include <signal.h>
#include <time.h>

//dimensione del buffer
#define BUFSIZE 1000

//Decommentare la linea corrispondente alla distribuzione voluta

//#define randomNumber (int)(gsl_rng_uniform(r)*BUFSIZE)
//#define randomNumber (int)(gsl_ran_gaussian(r,BUFSIZE/4)+BUFSIZE/2)
#define randomNumber (int)(gsl_ran_exponential(r,BUFSIZE/10))

void popola(int *buffer);
void leggi(int *buffer,pid_t pid);
void check(int *buffer);

void main(void){
	pid_t pid;
	int buff[BUFSIZE];
	switch(pid=fork()){
		case -1:
			printf("fork failure\n");
			break;
		case 0:
			execv("receiver",(char* const*)NULL);

			printf("execlp failure\n");
			break;
		default:
			printf("receiver ha pid %u\n",pid);
			printf("popolo il buffer\n");	
			popola(buff);
			printf("leggo il buffer\n");	
			leggi(buff,pid);
			kill(pid,3);
			exit(0);
	}
}

//popola il buffer con numeri casuali generati tramite un random number generator della libreria Gnu Scientific Library.
//come seed viene utilizzato l'output della funzione time().
void popola(int* buffer){
	int i;

	const gsl_rng_type *T;
	gsl_rng *r;

	gsl_rng_env_setup();

	T=gsl_rng_default;
	r=gsl_rng_alloc(T);
	//imposto il seed con time()
	gsl_rng_set( r,time(NULL));
	
	//i numeri vengono generati secondo la distribuzione impostata decommentando il relativo define
	for(i=0;i<BUFSIZE;i++) *(buffer + i)=randomNumber;
	
	gsl_rng_free(r);
}

//legge ciascun numero nel buffer: se numero pari -> invia SIGHUP al PID pid, se numero dispari -> invia SIGINT al PID pid.
void leggi(int *buffer,pid_t pid){
	int i;
	for(i=0;i<BUFSIZE;i++){
		usleep(1000);
	        *(buffer+i)%2==0 ? kill(pid,1) : kill(pid,2) ;
	}
}
