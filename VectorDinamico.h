#ifndef VECTORDINAMICO_H_INCLUDED
#define VECTORDINAMICO_H_INCLUDED

#include <stddef.h>
#include "Indice.h"

typedef struct
{
    void     *vdatos;
    unsigned  cantidad_actual;
    unsigned  cantidad_maxima;
} t_vector;

void vector_crear(t_vector *vec, size_t nmemb, size_t tam);
void vector_vaciar(t_vector *vec);
void vector_redimensionar(t_vector *vec, size_t nmemb, size_t tam);
int vector_agregar(t_vector *vec, const void *registro, size_t tam);
int vector_insertar_ordenado(t_vector *vec, const void *registro, size_t tam, int (*cmp)(const void*, const void*));
int vector_modificar(t_vector *vec, unsigned nro_reg, const void *modificado, size_t tam);
int vector_vacio(const t_vector *vec);
int vector_lleno(const t_vector *vec);
void vector_mostrar(const t_vector *vec, size_t tam, void (*mostrar)(const void *));
int vector_buscar(const t_vector *vec, const void *clave, size_t tam, int (*cmp)(const void *, const void *));

#endif // VECTORDINAMICO_H_INCLUDED
