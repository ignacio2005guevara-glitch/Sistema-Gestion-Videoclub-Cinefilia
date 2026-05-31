#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Indice.h"

void indice_crear(t_indice *indice, size_t nmemb, size_t tam)
{
    if(indice == NULL)
    {
        return;
    }

    indice->vindice = malloc(nmemb * tam);

    if(indice->vindice == NULL)
    {
        indice->cantidad_elementos_actual = 0;
        indice->cantidad_elementos_maxima = 0;
        return;
    }

    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = nmemb;
}

void indice_redimensionar(t_indice *indice, size_t nmemb, size_t tam)
{
    if(nmemb == indice->cantidad_elementos_maxima)
    {
        nmemb++; // + 1 xq si pasan 3 * 1,3 = 3 (No aumentaria la capacidad)
    }

    void* indiceAux = realloc(indice->vindice, nmemb * tam);

    if(indiceAux == NULL)
    {
        return; // No hay memoria disponible
    }

    indice->vindice = indiceAux;
    indice->cantidad_elementos_maxima = nmemb;
}

int  indice_insertar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    int izq = 0;
    int der = (int)indice->cantidad_elementos_actual - 1;
    int comp, medio;
    //int posicion_insercion = 0;

    if(indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima)
    {
        indice_redimensionar(indice, (size_t)indice->cantidad_elementos_actual * INCREMENTO, tamanyo); // + 1 xq 3 * 1,3 = 3 (No aumentaria la capacidad)
        if(indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima)
        {
            return ERROR; // No hay espacio
        }
    }

    t_reg_indice *vectorIndice = (t_reg_indice*)indice->vindice;
    const t_reg_indice *nuevoRegistro = (const t_reg_indice*)registro;



    while(izq <= der)
    {
        medio = izq + (der - izq) / 2;
        comp = cmp(nuevoRegistro, &vectorIndice[medio]);

        if(comp == 0) // Se encontró
        {
            return 0; // La clave ya existe en el indice
        }

        if(comp > 0 )
        {
            izq = medio + 1; //
        }

        if(comp < 0)
        {
            der = medio - 1;
        }
    }

    // 'izq' es la posicion donde tengo que insertar el registro

    // Tengo que desplazar a la derecha
    for(int i = (int)indice->cantidad_elementos_actual ; i > izq; i--)
    {
        vectorIndice[i] = vectorIndice[i-1];
    }

    vectorIndice[izq] = *nuevoRegistro;

    indice->cantidad_elementos_actual++;

    return 1;
}


int  indice_eliminar(t_indice *indice, const void *registro, size_t tam, int (*cmp)(const void *, const void *))
{
    int pos;
    t_reg_indice *vectorIndice = (t_reg_indice*) indice->vindice;

    if(indice->cantidad_elementos_actual == 0)
    {
        return ERROR;
    }

    pos = indice_buscar(indice, registro, indice->cantidad_elementos_actual, tam, cmp);

    if(pos == NO_EXISTE) // No se encontró el registro
    {
        return ERROR;
    }

    for(int i = pos; i < indice->cantidad_elementos_actual-1; i++)
    {
        vectorIndice[i] = vectorIndice[i+1];
    }

    indice->cantidad_elementos_actual--;
    return OK;
}

int  indice_buscar(const t_indice *indice, const void *registro, size_t nmemb, size_t tam, int (*cmp)(const void *, const void *))
{
    int i = 0;
    const t_reg_indice *vectorIndice = (const t_reg_indice*) indice->vindice;

    if(indice->cantidad_elementos_actual == 0)
    {
        return NO_EXISTE;
    }

    while(i < indice->cantidad_elementos_actual)
    {
        if(cmp(registro, &vectorIndice[i]) == 0) // Lo encontré
        {
            return i;
        }
        else if(cmp(registro, &vectorIndice[i]) > 0) // Sigo buscando
        {
            i++;
        }
        else // cmp(registro, &vectorIndice[i]) < 0
        {
            return NO_EXISTE;
        }
        //i++; /// AGREGADO
    }
    return NO_EXISTE;
}

int  indice_vacio(const t_indice *indice)
{
    return indice->cantidad_elementos_actual == 0 ? OK : ERROR;
}

int  indice_lleno(const t_indice *indice)
{
    return indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima ? OK : ERROR;
}

void indice_vaciar(t_indice *indice)
{
    if(indice->cantidad_elementos_actual == 0)
    {
        return;
    }
    else
    {
        free(indice->vindice);
        indice->cantidad_elementos_actual = 0;
        indice->cantidad_elementos_maxima = 0;
        return;
    }
}

int  indice_cargar(const char *path, t_indice *indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    FILE* pfArchivo = fopen(path, "rb");
    if(!pfArchivo)
    {
        return ERROR;
    }

    t_reg_indice reg_indice;
    unsigned nro_reg_archivo = 0;  // Contador de posición en el archivo */

    while(fread(vreg_ind, tamanyo, 1, pfArchivo))
    {
        reg_indice.nro_reg = nro_reg_archivo;
        reg_indice.dni = *(long*)vreg_ind;
        if(indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima)
        {
            indice_redimensionar(indice, indice->cantidad_elementos_actual * INCREMENTO, tamanyo);
        }

        indice_insertar(indice, &reg_indice, tamanyo, cmp);
    }

    fclose(pfArchivo);

    return 1;
}

void indice_mostrar(const t_indice *indice, size_t tam,void (*mostrar)(const void *))
{
    if (indice == NULL || mostrar == NULL)
        return;

    for (unsigned i = 0; i < indice->cantidad_elementos_actual; i++)
    {
        mostrar(indice->vindice + i * tam);
    }
}

int indice_obtener (const t_indice *indice, unsigned pos, void *destino, size_t tam)
{
    if(indice == NULL || destino == NULL)
        return ERROR;

    if(pos >= indice->cantidad_elementos_actual)
        return ERROR;

    memcpy(destino, indice->vindice + pos * tam, tam);

    return OK;
}
