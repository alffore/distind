//
// Created by alfonso on 12/04/19.
//

#include "distloc.h"

extern void escribeSalida(char *, int);

void escribeSalidaR(char *nomarch, int epoca);

extern const double RT;
extern PLocalidad ploc;
extern PDiccionario pdic;
extern int cantiloc;
extern const int cantidadTiposOC;


/**
* Función que escribe la salida de todas las localidades con sus casos
*/
void escribeSalida(char *nomarch, int epoca) {

    FILE *fh;
    int i, j;

    int e, m, l;

    fh = fopen(nomarch, "w");
    for (i = 0; i < cantiloc; i++) {
        PLocalidad p = (ploc + i);

        for (j = 0; j < cantidadTiposOC; j++) {
            e = (int) (p->c[j] / 10000000);
            m = (int) ((p->c[j] - e * 10000000) / 10000);
            l = p->c[j] - e * 10000000 - m * 10000;
            fprintf(fh, "%d,%d,%d,%s,%d,%lf,%d,%d,%d,0,%d\n", p->est, p->mun, p->loc, (pdic + j)->nombre, p->pob,
                    RT * p->dist[j], e, m, l, epoca);
        }

    }

    fclose(fh);

}

/**
 * Función que escribe salida resumen de calculo
 * @param nomarch
 * @param epoca
 */
void escribeSalidaR(char *nomarch, int epoca) {

    FILE *fh;
    size_t i;

    double distancia = 0.0;
    double distpob = 0.0;
    double pob = 0.0;

    double dist_pp = 0.0;
    double dist_ps = 0.0;


    fh = fopen(nomarch, "w");
    for (i = 0; i < cantiloc; i++) {
        PLocalidad p = (ploc + i);
        distancia += p->dist[0];
        distpob += p->pob * p->dist[0];
        pob += p->pob;
    }

    dist_pp = RT * distpob / pob;
    dist_ps = RT * distancia / cantiloc;

    fprintf(fh, "%lf,%lf,%lf,%lf,%lf, %d\n", pob, distpob, distancia, dist_pp, dist_ps,epoca);

    fclose(fh);
}
