/**
* @file main.c
 *
 * Código principal para la carga de datos de eventos en localidades y localidades del territorio nacional, permite la
 * estimación de la distancia promedio que recorre una persona para alcanzar una localidad con un evento
 *
 *  distind 304652 1115 /home/alfonso/NetBeansProjects/RENIC/utiles/indicadorPND/salidas/localidades.txt \
 *  /home/alfonso/NetBeansProjects/RENIC/utiles/indicadorPND/salidas/e6_m15mil.txt /tmp/salida_e6.csv 6
 *
 * @autor AAFR <alffore@gmail.com>
 * @date 15 de abril de 2019
*/
#include "distloc.h"


extern void cargaArchivoLocs(char *);

extern void cargaArchivoEventos(char *);

extern void escribeSalida(char *, int);

extern void escribeSalidaR(char *, int);

extern void calculoP(int);


const double RT = 6371008.8;
const int NumHilos = 2;
const int cantidadTiposOC = CANT_TIPOS_OC;

PLocalidad ploc;
PRecurso prec;
PDiccionario pdic;

int cantiloc;
int cantirec;
int epoca;

//extern int indexIP[cantidadTiposOC+1];


/**
* Rutina principal, carga los parámetros y en su caso los inicializa, define los espacios compartidos de memoria y carga
* la información
*/
int main(int cargs, char **args) {

    pid_t *childPids = NULL;
    pid_t p;

    int resumen = 0;

    int pos;
    int stillWaiting;
    int ii;


    if (cargs < 8) {
        fprintf(stderr, "No estan completos los parámetros:\n");
        fprintf(stderr, "\ndistind CantiLocs CantiEventos ArchivoLoc ArchivoEve ArchivoSal Epoca\n\n");
        fprintf(stderr, "\t CantiLocs:\tCantidad de localidades\n");
        fprintf(stderr, "\t CantiEventos:\tCantidad de eventos\n");
        fprintf(stderr, "\t ArchivoLoc:\tArchivo de localidades\n");
        fprintf(stderr, "\t ArchivoEve:\tArchivo de eventos\n");
        fprintf(stderr, "\t ArchivoSal:\tArchivo de salida\n");
        fprintf(stderr, "\t Epoca:\tIdentificador de epoca\n");
        fprintf(stderr, "\t Resumen: 0|1 Analisis resumido\n");
        return 1;
    }

    childPids = (pid_t *) malloc(NumHilos * sizeof(pid_t));

    cantiloc = atoi(*(args + 1));
    cantirec = atoi(*(args + 2));
    char *archlocs = *(args + 3);
    char *archrecs = *(args + 4);
    char *archsal = *(args + 5);
    epoca = atoi(*(args + 6));
    resumen = 0 + atoi(*(args + 7));


    pdic = (PDiccionario) malloc(sizeof(sDiccionario) * cantidadTiposOC);

    for (ii = 0; ii < cantidadTiposOC; ii++) {
        (pdic + ii)->nombre[0] = '\0';
    }


    ploc = (PLocalidad) mmap(NULL, sizeof(sLocalidad) * cantiloc, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
                             -1, 0);
    prec = (PRecurso) mmap(NULL, sizeof(sRecurso) * cantirec, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,
                           0);

    if (prec != NULL && ploc != NULL) {

        cargaArchivoLocs(archlocs);
        cargaArchivoEventos(archrecs);


        for (pos = 0; pos < NumHilos; ++pos) {
            if ((p = fork()) == 0) {

                printf("Hijo: %d %d\n", pos, getpid());
                calculoP(pos);
                exit(0);
            } else {
                childPids[pos] = p;
            }
        }


        do {
            stillWaiting = 0;
            for (ii = 0; ii < NumHilos; ++ii) {
                if (childPids[ii] > 0) {
                    if (waitpid(childPids[ii], NULL, WNOHANG) != 0) {

                        printf("hijo termino: %d %d\n", ii, childPids[ii]);

                        childPids[ii] = 0;

                    } else {

                        stillWaiting = 1;

                    }
                }
                /* Damos una rebanada de tiempo Unix */
                sleep(0);
            }
        } while (stillWaiting);


    }

    if (resumen == 0) {
        escribeSalida(archsal, epoca);
    } else {
        escribeSalidaR(archsal, epoca);
    }

    munmap(prec, sizeof(sRecurso) * cantirec);
    munmap(ploc, sizeof(sLocalidad) * cantiloc);

    free(pdic);

    if (childPids != NULL)free(childPids);
    return 0;
}
