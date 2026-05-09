#ifndef AUDITORIA_H_INCLUDED
#define AUDITORIA_H_INCLUDED

#define MAX_TIPO_ERROR 20
#define MAX_ERRORES    200

typedef struct
{
    char tipoValidacion[MAX_TIPO_ERROR]; /* "DNI", "Fecha", "Email", etc. */
    int  cantidadIncidencias;
    long clave;                          /* DNI o ID pelicula */
} t_error;

typedef struct
{
    t_error  errores[MAX_ERRORES];
    unsigned cantidad;
} t_auditoria;

#endif /* AUDITORIA_H_INCLUDED */
