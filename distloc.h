//
// Created by alfonso on 12/04/19.
//
/**
 * @file distloc.h
 *
 * Archivo con definiciones de las estructuras de datos
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef DISTIND_DISTLOC_H
#define DISTIND_DISTLOC_H


#define CANT_TIPOS_OC 1

//Definición de la estructura de Localidad
struct Localidad {
    int est;
    int mun;
    int loc;

    int cconapo;
    double lat;
    double lng;

    //coordenas cartesianas
    double x;
    double y;
    double z;

    int pob;

    //id de los objetos
    int aid[CANT_TIPOS_OC];
    
    //distancias mínimas a recursos
    double dist[CANT_TIPOS_OC];

    //claves conapo a las localidades de los recursos
    int c[CANT_TIPOS_OC];
};

typedef struct Localidad sLocalidad;
typedef struct Localidad *PLocalidad;


struct Recurso {
    int est;
    int mun;
    int loc;

    int cconapo;
    double lat;
    double lng;

    double x;
    double y;
    double z;

    char stipo[22];
    int tipo;

    int id;
};

typedef struct Recurso sRecurso;
typedef struct Recurso *PRecurso;


struct Diccionario {
    char nombre[22];
};

typedef struct Diccionario sDiccionario;
typedef struct Diccionario *PDiccionario;


#endif //DISTIND_DISTLOC_H
