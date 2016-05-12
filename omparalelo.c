/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : main.c
* Creation Date : 09-05-2016
* Last Modified : Wed 11 May 2016 06:02:54 PM CDT
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

void mezclar(numero_t*, numero_t*, numero_t*);
void mutate(numero_t*);

int main(){
    srand((unsigned)time(NULL));

    numero_t *numeros = (numero_t*)malloc(N*sizeof(numero_t)); //arreglo de la población
    double especimen; //espesemen

    printf("num >> ");
    scanf("%lf", &especimen);
    int op;
    printf("Opciones >> ");
    scanf("%d", &op);

    /*definicion de variables auxiliares */
    int i, j;
    char lost = 1;
    double ladeseada = sqrt(especimen); //el error deseado
    unsigned short prob_cruza;
    unsigned short prob_mutar;
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
    double *errores = (double*)malloc(N*sizeof(double));
    double tmp, tmp2;
    unsigned int tmp3, tmp4;
    int pendientes;
    numero_t* pareja;
    /* fin de variables auxiliares */

    #pragma omp parallel for
        for (i = 0; i < N; ++i){
            (numeros+i)->entera = rand()%1000000;
            (numeros+i)->decimal = rand()%1000000;
            (numeros+i)->total = ((double)(numeros+i)->entera) + (((double)(numeros+i)->decimal) / 100000);
        }

    while (lost){
        #pragma omp parallel for
            for ( i = 0; i < N; ++i){
                *(errores+i) = pow((ladeseada - (numeros+i)->total),2) / 2;

                #pragma omp critical
                {
                    if(*(errores+i) < error && lost)
                    {
                        printf("El resultado es: %lf, con un error de %lf\n", (numeros+i)->total, *(errores+i));
                        lost = 0;
                    }
                }
            }

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

/* Funcion que mezclara con llamadas mamadoras */
void mezclar(numero_t *por_cambiar, numero_t *pareja, numero_t *extra){
    unsigned int mascara_entera = 0xFF000;
    unsigned int mascara_decimal = 0x000FF;

    por_cambiar->entera = (pareja->entera & ~mascara_entera) & extra->entera;
    por_cambiar->decimal = (pareja->decimal & ~mascara_decimal) & extra->decimal;

    por_cambiar->total = por_cambiar->entera + (((double)por_cambiar->decimal) / 1000000);
}

void mutate(numero_t *por_mutar){
    por_mutar->entera = ~(por_mutar->entera);
    por_mutar->decimal = ~(por_mutar->decimal);
    por_mutar->total = ((double)por_mutar->entera) + (((double)por_mutar->decimal) / 1000000);
}
