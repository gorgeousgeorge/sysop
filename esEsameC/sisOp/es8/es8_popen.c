#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 128

int main(void){
	char promptbuf[MAXLINE];
	FILE *pipein;
	char readbuf[MAXLINE];
	printf("%% ");	//prompt
	while (fgets(promptbuf, MAXLINE, stdin) != NULL){
		if (promptbuf[strlen(promptbuf)-1] == '\n') promptbuf[strlen(promptbuf)-1]=0;

		if ((pipein = popen(promptbuf,"r")) == NULL){
			perror("popen");
			exit(1);
		}	
		while(fgets(readbuf, MAXLINE, pipein))
				printf("%s",readbuf);
		printf("%% ");
	}
	pclose(pipein);
	exit(0);
}
