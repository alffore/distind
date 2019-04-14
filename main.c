/**
*
*/
#include "distloc.h"


extern void cargaArchivoLocs(char *);
extern void cargaArchivoRecs(char *);
extern void escribeSalida(char *,int);
extern void calculoP(int);


const double RT=6371008.8;
const int NumHilos=8;
const int cantidadTiposOC=CANT_TIPOS_OC;

PLocalidad ploc;
PRecurso prec;
PDiccionario pdic;

int cantiloc;
int cantirec;
int epoca;

//extern int indexIP[cantidadTiposOC+1];


/**
*
*/
int main(int cargs, char **args){

    pid_t *childPids = NULL;
    pid_t p;


    int pos;
    int stillWaiting;
    int ii;


    if(cargs<7){
        fprintf(stderr, "No estan completos los parámetros:\n" );
        fprintf(stderr, "\ndistlocrec.exe CantiLocs CantiRecs ArchivoLoc ArchivoRec ArchivoSal Epoca\n\n" );
        fprintf(stderr, "\t CantiLocs:\tCantidad de localidades\n" );
        fprintf(stderr, "\t CantiRecs:\tCantidad de eventos\n" );
        fprintf(stderr, "\t ArchivoLoc:\tArchivo de localidades\n");
        fprintf(stderr, "\t ArchivoRec:\tArchivo de eventos\n");
        fprintf(stderr,"\t ArchivoSal:\tArchivo de salida\n");
        fprintf(stderr,"\t Epoca:\tIdentificador de epoca\n");
        return 1;
    }

    childPids = (pid_t*) malloc(NumHilos * sizeof(pid_t));

    cantiloc=atoi(*(args+1));
    cantirec=atoi(*(args+2));
    char * archlocs=*(args+3);
    char * archrecs=*(args+4);
    char * archsal= *(args+5);
    epoca = atoi(*(args+6));

    pdic = (PDiccionario)malloc(sizeof(sDiccionario)*cantidadTiposOC);

    for(ii=0;ii<cantidadTiposOC;ii++){
        (pdic+ii)->nombre[0]='\0';
    }


    ploc=(PLocalidad) mmap(NULL, sizeof(sLocalidad)*cantiloc, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    prec=(PRecurso) mmap(NULL, sizeof(sRecurso)*cantirec, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if(prec!=NULL && ploc!=NULL){

        cargaArchivoLocs(archlocs);
        cargaArchivoRecs(archrecs);


        for (pos = 0; pos < NumHilos; ++pos) {
            if ((p = fork()) == 0) {

                printf("Hijo: %d %d\n",pos,getpid());
                calculoP(pos);
                exit(0);
            }
            else {
                childPids[pos] = p;
            }
        }


        do {
            stillWaiting = 0;
            for (ii = 0; ii < NumHilos; ++ii) {
                if (childPids[ii] > 0) {
                    if (waitpid(childPids[ii], NULL, WNOHANG) != 0) {

                        printf("hijo termino: %d %d\n",ii, childPids[ii]);

                        childPids[ii] = 0;

                    }
                    else {

                        stillWaiting = 1;

                    }
                }
                /* Damos una rebanada de tiempo Unix */
                sleep(0);
            }
        } while (stillWaiting);



    }

    escribeSalida(archsal,epoca);


    munmap(prec,sizeof(sRecurso)*cantirec);
    munmap(ploc,sizeof(sLocalidad)*cantiloc);

    free(pdic);

    if(childPids!=NULL)free(childPids);
    return 0;
}
