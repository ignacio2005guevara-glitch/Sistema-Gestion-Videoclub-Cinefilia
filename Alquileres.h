#ifndef ALQUILERES_H_INCLUDED
#define ALQUILERES_H_INCLUDED

#define MAX_ALQUILERES_BASIC 2

typedef struct {
    long dni;
    int id_pelicula;
    int cant_alquileres;
} t_alquiler;

int alquilar_pelicula(t_indice *indice_miembros, t_indice *indice_titulos,t_vector *vec_miembros, t_vector *vec_titulos,t_vector *vec_alquileres);
int contar_alquileres(t_alquiler *alquileres,const int cant_elementos,long dni);
void agregar_alquiler(t_vector *vec_alquileres,t_alquiler *alquileres,long dni,int id_pelicula);
int buscar_alquiler(int ce,long dni, int id,t_alquiler *alquileres);
void mostrar_alquiler(const void *elem);
void mostrar_encabezado_alquiler(void);

#endif /* ALQUILERES_H_INCLUDED */
