#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Funzione che calcola il floor del log base 2 dell'input e 
 *lo stampa su STDOUT.
 */
int main(int argc, const char *argv[] ){
	double in = atof( argv[1] );
	int out = in<1 ? 0:floor( log2( in ) ); 
	printf( "%d",out );
	return 0;
}
