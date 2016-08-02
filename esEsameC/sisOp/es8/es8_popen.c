#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//lunghezza massima della stringa in input
#define MAXLINE 128

int main(void){
	char promptbuf[MAXLINE];
	FILE *pipein;
	char readbuf[MAXLINE];
	//mostra un prompt
	printf("%% ");
	//fintanto che l'input e' non nullo...
	while (fgets(promptbuf, MAXLINE, stdin) != NULL){
		//se l'ultimo char dell'input e' un newline lo rimpiazza con un NULL byte come richiesto da popen()
		if (promptbuf[strlen(promptbuf)-1] == '\n') promptbuf[strlen(promptbuf)-1]=0;
		//...chiama popen() in lettura con il comando ricevuto in input
		if ((pipein = popen(promptbuf,"r")) == NULL){
			perror("popen");
			exit(1);
		}	
		//legge dalla pipe creata da popen() MAXLINE caratteri e li scrive in readbuf
		while(fgets(readbuf, MAXLINE, pipein))
			printf("%s",readbuf);
		printf("%% ");
	}
	//distrugge stream e pipe creati da popen()
	pclose(pipein);
	exit(0);
}
