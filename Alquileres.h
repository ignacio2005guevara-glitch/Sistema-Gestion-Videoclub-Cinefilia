#ifndef ALQUILERES_H_INCLUDED
#define ALQUILERES_H_INCLUDED

#define MAX_ALQUILERES_BASIC 2

typedef struct
{
    long dni;
    int  idPelicula;
    int  cantidadAlquileres;
    int  activo;   /* 1 = tiene la pelicula ahora, 0 = devuelta */
} t_alquiler;

typedef struct
{
    t_alquiler *datos;
    unsigned    cantidad;
    unsigned    capacidad;
} t_historial;

#endif /* ALQUILERES_H_INCLUDED */
