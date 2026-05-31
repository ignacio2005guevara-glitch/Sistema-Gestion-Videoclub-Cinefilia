#ifndef AUDITORIA_H_INCLUDED
#define AUDITORIA_H_INCLUDED

#include "Estructuras.h"

#define MAX_TIPO_ERROR 20
#define MAX_ERRORES    200

//typedef struct
//{
//    char tipoValidacion[MAX_TIPO_ERROR]; /* "DNI", "Fecha", "Email", etc. */
//    int  cantidadIncidencias;
//    long clave;                          /* DNI o ID pelicula */
//} t_error;
//
//typedef struct
//{
//    t_error  errores[MAX_ERRORES];
//    unsigned cantidad;
//} t_auditoria;

typedef struct
{
    char tipo_error[MAX_TIPO_ERROR]; /* "DNI", "Fecha", "Email", etc. */
    long dni;
    t_fecha fecha;
}t_error;


void registrar_error(t_error *auditoria, char* error, long* dni);

#endif /* AUDITORIA_H_INCLUDED */
