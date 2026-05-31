#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VectorDinamico.h"

void vector_crear(t_vector *vec, size_t nmemb, size_t tam)
{
    if(vec == NULL)
    {
        return;
    }

    vec->vdatos = malloc(tam * nmemb);

    if(vec->vdatos == NULL)
    {
        vec->cantidad_actual = 0;
        vec->cantidad_maxima = 0;
        return;
    }

    vec->cantidad_actual = 0;
    vec->cantidad_maxima = nmemb;
}

void vector_vaciar(t_vector *vec)
{
    if(vec->cantidad_actual != 0)
    {
        free(vec->vdatos);
        vec->cantidad_actual = 0;
        vec->cantidad_maxima = 0;
    }

}

void vector_redimensionar(t_vector *vec, size_t nmemb, size_t tam)
{
    if(vec->cantidad_maxima == nmemb)
    {
        nmemb++;
    }

    void* aux = realloc(vec->vdatos, nmemb * tam);
    if(aux == NULL)
    {
        return; // No hay memoria
    }

    vec->vdatos = aux;
    vec->cantidad_maxima = nmemb;
}

int vector_agregar(t_vector *vec, const void *registro, size_t tam)
{
    if(vec->cantidad_actual == vec->cantidad_maxima)
    {
        vector_redimensionar(vec, vec->cantidad_maxima * INCREMENTO, tam);
        if(vec->cantidad_actual == vec->cantidad_maxima)
        {
            return ERROR; // No hay espacio
        }
    }

    void* pos = vec->vdatos + vec->cantidad_actual * tam;
    memcpy(pos, registro, tam);
    vec->cantidad_actual++;
    return OK;
}

int vector_modificar(t_vector *vec, unsigned nro_reg, const void *modificado, size_t tam)
{
    void* aux = vec->vdatos + nro_reg * tam;
    memcpy(aux, modificado, tam);
    return OK;
}

int vector_vacio(const t_vector *vec)
{
    return vec->cantidad_actual == 0 ? OK : ERROR;
}

int vector_lleno(const t_vector *vec)
{
    return vec->cantidad_actual == vec->cantidad_maxima ? OK : ERROR;
}

void vector_mostrar(const t_vector *vec, size_t tam, void (*mostrar)(const void *))
{
    for (unsigned i = 0; i < vec->cantidad_actual; i++)
    {
        mostrar(vec->vdatos + i * tam);
    }
}

int vector_buscar(const t_vector *vec, const void *clave, size_t tam, int (*cmp)(const void *, const void *))
{
    if(vec == NULL || clave == NULL)
    {
        return NO_EXISTE;
    }

    for(unsigned i = 0; i < vec->cantidad_actual; i++)
    {
        if(cmp(vec->vdatos + i * tam, clave) == 0)
            return (int)i;
    }
    return NO_EXISTE;
}

