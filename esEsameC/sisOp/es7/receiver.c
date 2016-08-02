#include <signal.h>
#include <sys/types.h>	//struct sigaction
//#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int sighup=0;
int sigint=0;

void func(int signo){
	switch(signo){
		case 1:
			sighup++;
			break;
		case 2:
			sigint++;
			break;
		default:
			printf("sighup no= %d\nsigint no=%d\n",sighup,sigint);
			exit(0);		
	}
}

void main(void){
	struct sigaction act, oldact;

	act.sa_handler=func;		//qui
	sigemptyset(&act.sa_mask);	//definisco
	act.sa_flags=0;			//la nuova azione*/

	sigaction(SIGHUP,NULL,&oldact);
	/*if(oldact.sa_handler!=SIG_IGN)*/ sigaction(SIGHUP,&act,NULL);
	
	sigaction(SIGINT,NULL,&oldact);
	/*if(oldact.sa_handler!=SIG_IGN)*/ sigaction(SIGINT,&act,NULL);

	sigaction(SIGQUIT,NULL,&oldact);
	/*if(oldact.sa_handler!=SIG_IGN)*/ sigaction(SIGQUIT,&act,NULL);

	while(1);
}
