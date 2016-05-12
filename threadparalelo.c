/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : main.c
* Creation Date : 09-05-2016
* Last Modified : Wed 11 May 2016 06:48:21 PM CDT
* Created By : shiro-saber

KNOW LEARN        .==.
C LANGUAGE       ()''()-.
      .---.       ;--; /
    .'_:___". _..'.  __'.
    |__ --==|'-''' \'...;
   [  ]  :[|       |---\
    |__| I=[|     .'    '.
    / / ____|     :       '._
   |-/.____.'      | :       :
  /___\ /___\      '-'._----'
_._._._._._._._._._._._._._._._._._._._._.*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <omp.h>
#include <time.h>
#include <math.h>

/* Este es el algoritmo genetico paralelo */

#define N 10000
#define error 0.001

struct numero {
    unsigned int entera;
    unsigned int decimal;
    double total;
};

typedef struct numero numero_t;

struct argt {
  int ini;
  int final;
  void* ptr;
};

typedef struct argt arg_t;

void *llenar(void*);
void *llenar_errores(void*);
void mezclar(numero_t*, numero_t*, numero_t*);
void mutate(numero_t*);

numero_t *numeros;
double *errores;
double ladeseada;
char lost = 1;

int main(){
    srand((unsigned)time(NULL));
	int op;

    numeros = (numero_t*)malloc(N*sizeof(numero_t)); //arreglo de la población
    double especimen; //espesemen

    printf("num >> ");
    scanf("%lf", &especimen);
	printf("Opciones >> ");
	scanf("%d", &op);

    /*definicion de variables auxiliares */
    int i, j;
	unsigned short prob_cruza;
    unsigned short prob_mutar;

    ladeseada = sqrt(especimen); //el error deseado
	switch (op){
		case 1:
			prob_cruza = 80;
			prob_mutar = 5;
			break;
		case 2:
			prob_cruza = 85;
			prob_mutar = 10;
			break;
		case 3:
			prob_cruza = 40;
			prob_mutar = 7;
			break;
		case 4:
			prob_cruza = 30;
			prob_mutar = 50;
			break;
		case 5:
			prob_cruza = 90;
			prob_mutar = 5;
			break;
		default:
			printf("Error");
			return 0;
	}
	errores = (double*)malloc(N*sizeof(double));
    double tmp, tmp2;
    unsigned int tmp3, tmp4;
    int pendientes;
    numero_t* pareja;
    pthread_t *threads = (pthread_t*)malloc(4*sizeof(pthread_t));
    arg_t arrgs[4];
    /* fin de variables auxiliares */

    for (i = 0; i < 4; ++i){
      arrgs[i].ini = (N/4)*(i);
      arrgs[i].final = (N/4) + arrgs[i].ini;
      arrgs[i].ptr = numeros;
      pthread_create((threads+i), 0, &llenar, &arrgs[i]);
    }

    for (i = 0; i < 4; ++i)
      pthread_join(*(threads+i), NULL);


    while (lost){

      for (i = 0; i < 4; ++i){
        arrgs[i].ini = (N/4)*(i);
        arrgs[i].final = (N/4) + arrgs[i].ini;
        arrgs[i].ptr = errores;
        pthread_create((threads+i), 0, &llenar_errores, &arrgs[i]);
      }

      for (i = 0; i < 4; ++i)
        pthread_join(*(threads+i), NULL);


        if(!lost) break;

        // BUBBLE SORT
            for ( i = 0; i < N; i++){
                for ( j = 0; j < N ; ++j){
                    if (*(errores+j) < *(errores+i) ){
                        /*Esta parte de errores*/
                        tmp = *(errores+j);
                        *(errores+j) = *(errores+i);
                        *(errores+i) = tmp;

                        /*Esta parte de total*/
                        tmp2 = (numeros+j)->total;
                        (numeros+j)->total = (numeros+i)->total;
                        (numeros+i)->total = tmp2;

                        /*Esta parte decimal*/
                        tmp3 = (numeros+j)->decimal;
                        (numeros+j)->decimal = (numeros+i)->decimal;
                        (numeros+i)->decimal = tmp3;

                        /*Esta parte entera*/
                        tmp4 = (numeros+j)->entera;
                        (numeros+j)->entera = (numeros+i)->entera;
                        (numeros+i)->entera = tmp4;
                    }
                }
            }

        /* Asignacion de valores a las varaibles auxiliares */
        pendientes = N / 2;
        pareja = NULL;
        /* FIN */

        while(pendientes > 0){
            for(i = N/2; i < N; ++i){
                if (rand() % 100 <= prob_cruza && pendientes > 0){
                    if (pareja == NULL)
                        pareja = (numeros+i);
                    else{
                        mezclar((numeros+pendientes-1), pareja, (numeros+i));
                        pareja = numeros+i;
                        pendientes--;
                    }
                }
            }
        }



        for (i = 0; i < N; ++i){
            if (rand() % 100 <= prob_mutar)
                mutate((numeros+i));
        }
    }

    free(numeros);
    free(errores);
    return 0;
}

void* llenar(void* args){
  arg_t* arg = (arg_t*)args;
  int i;

  for (i = arg->ini; i < arg->final; ++i){
            (numeros+i)->entera = rand()%1000000;
            (numeros+i)->decimal = rand()%1000000;
            (numeros+i)->total = ((double)(numeros+i)->entera) + (((double)(numeros+i)->decimal) / 100000);
        }

}

void* llenar_errores(void* args){
  arg_t* arg = (arg_t*)args;
  int i;

  for (i = arg->ini; i < arg->final; ++i){
    *(errores+i) = pow((ladeseada - (numeros+i)->total),2) / 2;
    if(*(errores+i) < error && lost)
    {
      printf("El resultado es: %lf\n", (numeros+i)->total);
      lost = 0;
    }
  }
}

/* Funcion que mezclara con llamadas mamadoras */
void mezclar(numero_t *por_cambiar, numero_t *pareja, numero_t *extra){
    unsigned int mascara_entera = 0xFF000;
    unsigned int mascara_decimal = 0x000FF;

    por_cambiar->entera = (pareja->entera & ~mascara_entera) & extra->entera;
    por_cambiar->decimal = (pareja->decimal & ~mascara_decimal) & extra->decimal;

    por_cambiar->total = por_cambiar->entera + (((double)por_cambiar->decimal) / 1000000);
}

void mutate(numero_t* por_mutar){
    por_mutar->entera = ~(por_mutar->entera);
    por_mutar->decimal = ~(por_mutar->decimal);
    por_mutar->total = por_mutar->entera + (((double)por_mutar->decimal) / 1000000);
}
