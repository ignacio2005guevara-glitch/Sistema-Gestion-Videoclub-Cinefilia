#ifndef TITULOS_H_INCLUDED
#define TITULOS_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Auditoria.h"

#define MAX_TITULO 60
#define MAX_GENERO 20

typedef struct
{
    int  idPelicula;
    char titulo[MAX_TITULO];
    char genero[MAX_GENERO];
    int  stock;
} t_titulo;

void cargarTitulos(const char *path, t_indice *indice, t_auditoria *auditoria);
int  altaTitulo(t_indice *indice);
int  bajaTitulo(t_indice *indice);
int  modificarTitulo(t_indice *indice);
void mostrarTitulo(const t_titulo *t);
void persistirTitulos(const t_indice *indice, t_fecha fechaProceso);
int  cmpTitulo         (const void *a, const void *b);

#endif /* TITULOS_H_INCLUDED */
