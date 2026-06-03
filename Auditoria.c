#include <stdio.h>
#include <string.h>
#include "Auditoria.h"

void registrar_error_miembro(t_vector *auditoria, char* tipo_error, long* dni, t_fecha *fecha_proceso)
{
    t_error_miembro error;

    strcpy(error.tipo_error, tipo_error);
    error.dni = *dni;
    error.fecha = *fecha_proceso;

    vector_insertar_ordenado(auditoria, &error, sizeof(t_error_miembro), cmp_error_miembro);
}

void registrar_error_titulo(t_vector *auditoria, char* tipo_error, int* id_pelicula, t_fecha *fecha_proceso)
{
     t_error_titulo error;

    strcpy(error.tipo_error, tipo_error);
    error.id_pelicula = *id_pelicula;
    error.fecha = *fecha_proceso;

    vector_insertar_ordenado(auditoria, &error, sizeof(t_error_titulo), cmp_error_titulo);
}

void mostrar_encabezado_error_miembro(void)
{
    printf("==================================================\n");
    printf("%-*s | %-9s | %-10s\n",
           MAX_TIPO_ERROR, "TIPO ERROR",
           "DNI",
           "FECHA");
    printf("=================================================\n");
}

void mostrar_error_miembro(const void *elem)
{
    const t_error_miembro *e = (const t_error_miembro *)elem;

    printf("%-*.*s | %-9ld | %02d/%02d/%04d\n",
           MAX_TIPO_ERROR, MAX_TIPO_ERROR, e->tipo_error,
           e->dni,
           e->fecha.dia, e->fecha.mes, e->fecha.anio);
}

void mostrar_encabezado_error_titulo(void)
{
    printf("================================================\n");
    printf("%-*s | %-9s | %-10s\n",
           MAX_TIPO_ERROR, "TIPO ERROR",
           "ID PELICULA",
           "FECHA");
    printf("================================================\n");
}

void mostrar_error_titulo(const void *elem)
{
    const t_error_titulo *e = (const t_error_titulo *)elem;

    printf("%-*.*s | %-9d | %02d/%02d/%04d\n",
           MAX_TIPO_ERROR, MAX_TIPO_ERROR, e->tipo_error,
           e->id_pelicula,
           e->fecha.dia, e->fecha.mes, e->fecha.anio);
}

int cmp_error_miembro(const void *valor1, const void *valor2)
{
    const t_error_miembro *v1 = (const t_error_miembro *)valor1;
    const t_error_miembro *v2 = (const t_error_miembro *)valor2;

    return strcmp(v1->tipo_error, v2->tipo_error);
}

int cmp_error_titulo(const void *valor1, const void *valor2)
{
    const t_error_titulo *v1 = (const t_error_titulo *)valor1;
    const t_error_titulo *v2 = (const t_error_titulo *)valor2;

    return strcmp(v1->tipo_error, v2->tipo_error);
}

