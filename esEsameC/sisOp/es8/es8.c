#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> // pid_t

#define MAXLINE 128

char** parse(char* str,const char* delim);

int main(void){
	char buf[MAXLINE];
	char** cmd;
	char** args;
	char** temp;
	pid_t pid;
	int status;
	int j,k;

	printf("%% ");	//prompt
	
	while (fgets(buf, MAXLINE, stdin) != NULL){
		if (buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1]=0;
	
		//PARSER
		cmd=parse(buf,"|");
	
//		if((int)*cmd!=1){//pipe the shit out of cmd
		for(j=1;j<(int)*cmd + 1;j++){
			temp=NULL;
			args=NULL;
			temp=parse(*(cmd+j)," -");
			args=(char **)malloc(((int)*temp + 1)*sizeof(char *));
			for(k=0;k<(int)*temp;k++){
				*(args+k)=*(temp+1+k);
			}
			k++;
			*(args+k)=NULL;
			
			if ((pid=fork()) < 0){
				printf("errore di fork ");
				exit(1);
			}

			else if (pid==0){	//child
				execvp(args[0],args);
				printf("non posso eseguire: %s",args[0]);
				exit(127);
			}
			
			else
				if ((pid=waitpid(pid,&status,0))<0){
					printf("errore di waitpid");
					exit(1);
				}
		}
		//free?
		printf("%% ");
	}
	exit(0);
}
//parser
char** parse(char* str,const char* delim){
	char** parsedcmd;
	int cmdsize=0;
	int j;
	char auxstr[MAXLINE];
	char* cmdtoken;
	char* auxtoken;
	//strtok() modifica la stringa in input, quindi devo farne una copia per poterlo fare di nuovo
	strcpy(auxstr,str);

	auxtoken=strtok(auxstr,delim);
	do{	
		cmdsize++;
		auxtoken = strtok(NULL,delim);
//		printf("token %d, %s\n",cmdsize,auxtoken);
	}while(auxtoken!=NULL);
//	printf("alloco parsedcmd\n");
	parsedcmd=(char**)malloc((cmdsize+1)*sizeof(char*));
//	printf("allocato;\n");
//	printf("estraggo primo token:\n");
	//come primo elemento inserisco il num di token, con un typecast che dovro' invertire nel main
	*parsedcmd=(char*)cmdsize;
	*(parsedcmd + 1) = strtok(str,delim);
	for(j=2;j<cmdsize+1;j++){
		parsedcmd[j]=strtok(NULL,delim);
//		printf("token %d, %s\n",j,parsedcmd[j]);
	}
	return parsedcmd;
}
