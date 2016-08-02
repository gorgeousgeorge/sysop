#ifndef LAMPORT_H
#define LAMPORT_H

int compare(int a, int b, int c, int d);
int maxTicket(int *ticket, int nTh);
void lLock(int *ticket, int *turno, int nTh, int id);
void lUnlock(int *ticket, int id);

#endif
