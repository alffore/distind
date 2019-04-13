//
// Created by alfonso on 12/04/19.
//

#include "distloc.h"

extern void escribeSalida(void);

extern const double RT;
extern PLocalidad ploc;
extern PDiccionario pdic;
extern int cantiloc;
extern const int cantidadTiposOC;


/**
* Función que escribe la salida de todas las localidades con sus casos
*/
void escribeSalida(void){

    FILE * fh;
    int i,j;

    int e,m,l;

    fh=fopen("./salida.txt","w");
    for(i=0;i<cantiloc;i++){
        PLocalidad p=(ploc+i);

        for(j=0;j<cantidadTiposOC;j++){
            e=(int)(p->c[j]/10000000);
            m=(int)((p->c[j]-e*10000000)/10000);
            l=p->c[j]-e*10000000-m*10000;
            fprintf(fh,"%d,%d,%d,%s,%d,%lf,%d,%d,%d,0\n",p->est,p->mun,p->loc,(pdic+j)->nombre,p->pob,RT*p->dist[j],e,m,l);
        }

    }

    fclose(fh);

}
