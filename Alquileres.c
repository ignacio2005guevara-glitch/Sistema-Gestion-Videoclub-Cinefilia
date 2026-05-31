#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Indice.h"
#include "VectorDinamico.h"
#include "Alquileres.h"
#include "Estructuras.h"
#include "Titulos.h"

int alquilar_pelicula(t_indice *indice_miembros, t_indice *indice_titulos,t_vector *vec_miembros, t_vector *vec_titulos,t_vector *vec_alquileres)
{
    long dni;
    int id_pelicula,nro_reg_titulo,nro_reg, pos_miembro=-1, pos_titulo=-1, pos_alquiler=-1;

    t_reg_indice_miembro reg_buscar_miembro;
    t_reg_indice_titulo reg_buscar_pelicula;
    t_reg_indice_miembro *indice = (t_reg_indice_miembro*)indice_miembros->vindice;
    t_reg_indice_titulo *indice_pel = (t_reg_indice_titulo*)indice_titulos->vindice;

    t_miembro *miembros = (t_miembro*)vec_miembros->vdatos;
    t_titulo *titulos = (t_titulo*)vec_titulos->vdatos;
    t_alquiler *alquileres = (t_alquiler*)vec_alquileres->vdatos;

    do
    {
        printf("\nIngrese el dni del miembro (0 para cancelar): ");
        scanf("%ld",&dni);
        if(dni==0)
            return ERROR;
        reg_buscar_miembro.dni = dni;
        pos_miembro=indice_buscar(indice_miembros, &reg_buscar_miembro,indice_miembros->cantidad_elementos_actual,sizeof(t_reg_indice_miembro),cmp_DNI);
        if(pos_miembro==NO_EXISTE)
            printf("\nEl miembro no existe");
    }
    while(pos_miembro==NO_EXISTE);

    do
    {
        printf("\nIngrese el ID de la pelicula (0 para cancelar): ");
        scanf("%d",&id_pelicula);
        if(id_pelicula==0)
            return ERROR;
        reg_buscar_pelicula.id = id_pelicula;
        pos_titulo=indice_buscar(indice_titulos, &reg_buscar_pelicula,indice_titulos->cantidad_elementos_actual,sizeof(t_reg_indice_titulo),cmp_id_pelicula);
        if(pos_titulo==NO_EXISTE)
            printf("\nLa pelicula no existe");
    }while(pos_titulo==NO_EXISTE);

    nro_reg = indice[pos_miembro].nro_reg;
    nro_reg_titulo = indice_pel[pos_titulo].nro_reg;

    if(titulos[nro_reg_titulo].stock==0)
    {
        printf("\nNo esta disponible el titulo, sin stock");
        return ERROR;
    }

    pos_alquiler=buscar_alquiler(vec_alquileres->cantidad_actual,dni,id_pelicula,alquileres);

    if(!strcmp(miembros[nro_reg].plan,"BASIC"))

    {
        if(contar_alquileres(alquileres,vec_alquileres->cantidad_actual,dni) >= MAX_ALQUILERES_BASIC)
        {
            printf("\nPLAN BASIC, no puede alquilar mas de 2 peliculas a la vez");
            return ERROR;
        }
    }

    if(pos_alquiler != -1)
        alquileres[pos_alquiler].cant_alquileres++;
    else
        agregar_alquiler(vec_alquileres,alquileres,dni,id_pelicula);

    titulos[nro_reg_titulo].stock--;

    printf("\nAlquilado con exito.\n");

    mostrar_encabezado_alquiler();
    vector_mostrar(vec_alquileres, sizeof(t_alquiler), mostrar_alquiler);

    return OK;
}

int contar_alquileres(t_alquiler *alquileres,const int cant_elementos,long dni)
{
    int i,cont=0;

    for(i=0; i<cant_elementos; i++)
    {
        if(dni==alquileres[i].dni)
            cont+=alquileres[i].cant_alquileres;
    }
    return cont;
}

void agregar_alquiler(t_vector *vec_alquileres,t_alquiler *alquileres,long dni,int id_pelicula)
{
    int pos;
    if(vec_alquileres->cantidad_actual == vec_alquileres->cantidad_maxima)
    {
        vector_redimensionar(vec_alquileres, vec_alquileres->cantidad_maxima * INCREMENTO, sizeof(t_alquiler));
        alquileres = (t_alquiler*)vec_alquileres->vdatos; // actualizas el puntero
    }
    pos = vec_alquileres->cantidad_actual;
    alquileres[pos].dni = dni;
    alquileres[pos].cant_alquileres = 1;
    alquileres[pos].id_pelicula = id_pelicula;

    vec_alquileres->cantidad_actual++;
}

int buscar_alquiler(int ce,long dni, int id,t_alquiler *alquileres)
{
    int i=0,pos=-1;

    while(pos==-1 && i<ce)
    {
        if(dni==alquileres[i].dni && id==alquileres[i].id_pelicula)
            pos=i;
        i++;
    }

    return pos;
}

void mostrar_alquiler(const void *elem)
{
    const t_alquiler *a = (const t_alquiler *)elem;

    printf("%-9ld | %-9d | %d\n",
           a->dni,
           a->id_pelicula,
           a->cant_alquileres);
}

void mostrar_encabezado_alquiler(void)
{
    printf("%-9s | %-9s | %s\n",
           "DNI", "ID PELICULA", "CANT ALQUILERES");
}
