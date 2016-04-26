/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Algoritmo genetico. Ver instrucciones.txt para las especificaciones.
 *
 *        Version:  1.0
 *        Created:  25/04/16 11:07:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Monzalvo (A01021514), Abraham Esses (A01018853), Marcos Lopez (A01020023), Juan Carlos (A01010200)
 *   Organization:  
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include    <stdio.h>
#include    <pthread.h>
#include    <omp.h>
#include    <time.h>

#define Poblacion 10000


struct numero {
	short int entero;
	short int decimal;
};				/* ----------  end of struct numero  ---------- */

typedef struct numero numero_t;

/* ===  FUNCTION MAIN ===================================================================*/
int main ( int argc, char *argv[] ){
	srand((unsigned)time(NULL));
	int i;
	numero_t * arreglo = (numero_t*) malloc (Poblacion * sizeof(numero_t));
	for (i = 0; i < Poblacion; ++i){
		(arreglo+i)->entero = rand() % 1000000;
		(arreglo+i)->decimal = rand() % 1000000;
	}

	free(arreglo);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
