#ifndef PETERSON_H_
#define PETERSON_H_

	extern void pLock( int* flag, int* turno, int nTh, int id );
	extern void pUnlock( int* flag, int id );

#endif
