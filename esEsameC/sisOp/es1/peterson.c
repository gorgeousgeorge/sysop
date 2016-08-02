#include "peterson.h"

void pLock(int* flag,int* turno,int nTh,int id){    //sizeof(flag) oppure passo il n di thread come argomento
    int j,k;
    for(j=0;j<nTh -1;j++){
        *(flag+id)=j;
        *(turno+j)=id;
        for(k=0;k<nTh;k++){
            if(k!=id)
                while(*(flag+k)>=*(flag+id) && *(turno+j)==id){};
            }
        }
    }

void pUnlock(int* flag,int id){
    *(flag+id)=-1;
    }
