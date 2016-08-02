#include "lamport.h"
int compare(int a, int b, int c, int d){
    return a<c || (a==c && b<d);
}

int maxTicket(int *ticket, int nTh){
    int j;
    int max=0;
    for(j=0;j<nTh;j++){
        max=(max<*(ticket+j))?*(ticket+j):max;
    }
    return max;
}
void lLock(int *ticket,int *turno,int nTh,int id){
    int i;
    *(turno+id)=1;
    *(ticket+id)=1+maxTicket(ticket,nTh);
    *(turno+id)=0;
    for(i=0;i<nTh;i++){
        while(*(turno+i)==1){/*printf("bloccato in while su turno[%d]\n",i);*/};
        while(*(ticket+i)!=-1 && compare(*(ticket+i),i,*(ticket+id),id) ){/*printf("bloccato in while su *ticket[%d]\n",i);*/};
        
    }
}

void lUnlock(int *ticket, int id){
    *(ticket+id)=-1;
}
