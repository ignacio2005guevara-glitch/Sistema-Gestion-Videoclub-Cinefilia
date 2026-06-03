#ifndef VECTORDINAMICO_H_INCLUDED
#define VECTORDINAMICO_H_INCLUDED

#include <stddef.h>   /* size_t */
#include "indice.h"   /* OK, ERROR, CANTIDAD_ELEMENTOS, INCREMENTO */

typedef struct
{
    void     *vdatos;
    unsigned  cantidad_actual;
    unsigned  cantidad_maxima;
} t_vector;

/**************************************************************************
 Descripción : Toma memoria para nmemb elementos e inicializa la
               estructura vacía.
 Parámetros  : vec  : TDA vector.
               nmemb   : cantidad inicial de elementos.
               tamanyo : espacio en bytes ocupado por cada elemento.
 Retorno     : n/a.
 Observaciones: Equivalente a indice_crear().
**************************************************************************/
void vector_crear(t_vector *vec, size_t nmemb, size_t tam);

/**************************************************************************
 Descripción : Libera la memoria del vector y resetea la estructura.
 Parámetros  : vec : TDA vector.
 Retorno     : n/a.
 Observaciones: Debe llamarse antes de terminar el programa.
**************************************************************************/
void vector_vaciar(t_vector *vec);

/**************************************************************************
 Descripción : Redimensiona el vector aumentando su capacidad en un 30%.
 Parámetros  : ve  : TDA vector.
               tamanyo : espacio en bytes ocupado por cada elemento.
 Retorno     : n/a.
 Observaciones: Se invoca automáticamente desde vector_agregar() cuando
                el vector está lleno.
**************************************************************************/
void vector_redimensionar(t_vector *vec, size_t nmemb, size_t tam);

/**************************************************************************
 Descripción : Agrega un nuevo elemento al final del vector. Si está
               lleno, redimensiona automáticamente.
 Parámetros  : vec   : TDA vector.
               registro : puntero al elemento a agregar.
               tamanyo  : espacio en bytes ocupado por el elemento.
 Retorno     : OK si la operación fue exitosa, ERROR en caso contrario.
 Observaciones: No mantiene orden; el orden lo gestiona el índice.
**************************************************************************/
int vector_agregar(t_vector *vec, const void *registro, size_t tam);

int vector_insertar_ordenado(t_vector *vec, const void *registro, size_t tam, int (*cmp)(const void*, const void*));

/**************************************************************************
 Descripción : Sobreescribe el elemento ubicado en nro_reg con los
               datos de modificado.
 Parámetros  : vec     : TDA vector.
               nro_reg    : posición del elemento a modificar.
               modificado : puntero al elemento con los nuevos datos.
               tamanyo    : espacio en bytes ocupado por el elemento.
 Retorno     : OK si la operación fue exitosa, ERROR en caso contrario.
 Observaciones: -
**************************************************************************/
int vector_modificar(t_vector *vec, unsigned nro_reg, const void *modificado, size_t tam);

/**************************************************************************
 Descripción : Determina si el vector contiene 0 (cero) elementos.
 Parámetros  : vec : TDA vector.
 Retorno     : OK si está vacío, cualquier otro valor si no lo está.
 Observaciones: -
**************************************************************************/
int vector_vacio(const t_vector *vec);

/**************************************************************************
 Descripción : Determina si el vector alcanzó su capacidad máxima.
 Parámetros  : vec : TDA vector.
 Retorno     : OK si está lleno, cualquier otro valor si no lo está.
 Observaciones: -
**************************************************************************/
int vector_lleno(const t_vector *vec);

/*************************************************************************
 Descripción : Recorre el vector y llama a mostrar() por cada elemento.
 Parámetros  : vector  : TDA vector.
               tamanyo : espacio en bytes ocupado por cada elemento.
               mostrar : función que sabe imprimir el tipo concreto.
 Retorno     : n/a.
*************************************************************************/
void vector_mostrar(const t_vector *vec, size_t tam, void (*mostrar)(const void *));

/**************************************************************************
 Descripción : Busca un elemento en el vector usando una función de
               comparación provista.
 Parámetros  : vector  : TDA vector.
               clave   : puntero al elemento a buscar.
               tam     : tamaño en bytes del elemento.
               cmp     : función de comparación provista.
 Retorno     : posición del elemento si existe, NO_EXISTE en caso contrario.
**************************************************************************/
int vector_buscar(const t_vector *vec, const void *clave, size_t tam, int (*cmp)(const void *, const void *));


#endif // VECTORDINAMICO_H_INCLUDED
