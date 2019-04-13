//
// Created by alfonso on 12/04/19.
//

#include "distloc.h"

extern PLocalidad ploc;
extern PRecurso prec;
extern PDiccionario pdic;
extern const int cantidadTiposOC;

extern void cargaArchivoLocs(char *);
extern void cargaArchivoRecs(char *);




int eml2conapo(int, int, int);
double deg2rad(double);
void cesfe2carte(double lat, double lng, double *res);


/**
* Función que carga los datos de localidades
*/
void cargaArchivoLocs(char * archlocs){

    printf("Carga Localidades\n");

    int est;
    int mun;
    int loc;

    double lat,lng;
    double latr,lngr;


    double *res2=(double *)malloc(3*sizeof(double));


    int pob;

    FILE *fh=fopen(archlocs,"r");

    PLocalidad p;

    int j=0;
    int i=0;
    while(fscanf(fh,"%d %d %d %lf %lf %d",&est,&mun,&loc,&lat,&lng,&pob)!=EOF){


        latr=deg2rad(lat);
        lngr=deg2rad(lng);
        cesfe2carte(latr,lngr,res2);

        p=(ploc+i);

        p->est=est;
        p->mun=mun;
        p->loc=loc;
        p->cconapo=eml2conapo(est,mun,loc);

        p->lat=lat;
        p->lng=lng;

        p->x=*(res2);
        p->y=*(res2+1);
        p->z=*(res2+2);

        p->pob=pob;

        for(j=0;j<cantidadTiposOC;j++){
            p->dist[j]=M_PI;
        }

        i++;
    }

    printf("Localidades : %d\n",i);

    fclose(fh);

    free(res2);

}

/**
* Función que se encarga de cargar los recursos en la localidades
*/
void cargaArchivoRecs(char * archrecs){

    printf("Carga Recursos\n");

    int est;
    int mun;
    int loc;

    double lat,lng;
    double latr,lngr;

    char stipo[22];
    int tipo;
    int id;

    double *res2=(double *)malloc(3*sizeof(double));

    FILE *fh=fopen(archrecs,"r");

    PRecurso p;



    int i=0;
    while(fscanf(fh,"%d %d %d %lf %lf %s %d %d",&est,&mun,&loc,&lat,&lng,stipo,&tipo,&id)!=EOF){


        latr=deg2rad(lat);
        lngr=deg2rad(lng);
        cesfe2carte(latr,lngr,res2);

        p=(prec+i);

        p->est=est;
        p->mun=mun;
        p->loc=loc;
        p->cconapo=eml2conapo(est,mun,loc);

        p->lat=lat;
        p->lng=lng;

        p->x=*(res2);
        p->y=*(res2+1);
        p->z=*(res2+2);

        strcpy(p->stipo,stipo);

        if((pdic+tipo)->nombre[0]=='\0'){
            strcpy((pdic+tipo)->nombre,stipo);
        }

        p->tipo=tipo;
        p->id=id;



        i++;
    }

    printf("Recursos : %d\n",i);
    free(res2);
}

/**
* Función que convierte las claves de Estado, Municipio y Localidad a la clave Conapo
*/
int eml2conapo(int e, int m, int l){
    return (e*1000+m)*10000+l;
}

/**
* Función que convierte los DEG a RAD
*/
double deg2rad(double x){
    return M_PI*x/180.00;
}

/**
* Función convierte coordenadas geográficas a coordenadas cartesianas espaciales
*/
void cesfe2carte(double lat, double lng, double *res){
    double r2=0;

    *(res)=sin(lng)*cos(lat);
    *(res+1)=cos(lng)*cos(lat);
    *(res+2)=sin(lat);

    r2=pow(*(res),2)+pow(*(res+1),2)+pow(*(res+2),2);
    r2=sqrt(r2);

    *(res)/=r2;
    *(res+1)/=r2;
    *(res+2)/=r2;
}

