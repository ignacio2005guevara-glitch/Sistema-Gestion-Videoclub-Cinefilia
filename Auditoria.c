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

void persistir_auditorias(const t_vector *vec_auditoria_miem, const t_vector *vec_auditoria_tit, t_fecha fecha_proceso)
{
    char path_miem[100];
    char path_tit[100];
    sprintf(path_miem, "Archivos\\auditoria_miembros_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);
    sprintf(path_tit, "Archivos\\auditoria_titulos_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);

    FILE *pf_miem = fopen(path_miem, "wb");
    if (pf_miem)
    {
        fwrite(vec_auditoria_miem->vdatos, sizeof(t_error_miembro), vec_auditoria_miem->cantidad_actual, pf_miem);
        fclose(pf_miem);
        printf("Auditoria de miembros persistida en: %s\n", path_miem);
    }
    else
    {
        printf("Error al crear archivo de auditoria de miembros: %s\n", path_miem);
    }

    FILE *pf_tit = fopen(path_tit, "wb");
    if (pf_tit)
    {
        fwrite(vec_auditoria_tit->vdatos, sizeof(t_error_titulo), vec_auditoria_tit->cantidad_actual, pf_tit);
        fclose(pf_tit);
        printf("Auditoria de titulos persistida en: %s\n", path_tit);
    }
    else
    {
        printf("Error al crear archivo de auditoria de titulos: %s\n", path_tit);
    }
}

int cargar_auditoria_miembros_bin(const char *path, t_vector *vec)
{
    FILE *pf = fopen(path, "rb");
    if (!pf) {
        printf("No se pudo abrir archivo de auditoria de miembros: %s\n", path);
        return ERROR;
    }
    t_error_miembro e;
    int count = 0;
    while (fread(&e, sizeof(t_error_miembro), 1, pf) == 1)
    {
        vector_agregar(vec, &e, sizeof(t_error_miembro));
        count++;
    }
    fclose(pf);
    printf("Cargadas %d incidencias de auditoria de miembros desde: %s\n", count, path);
    return OK;
}

int cargar_auditoria_titulos_bin(const char *path, t_vector *vec)
{
    FILE *pf = fopen(path, "rb");
    if (!pf) {
        printf("No se pudo abrir archivo de auditoria de titulos: %s\n", path);
        return ERROR;
    }
    t_error_titulo e;
    int count = 0;
    while (fread(&e, sizeof(t_error_titulo), 1, pf) == 1)
    {
        vector_agregar(vec, &e, sizeof(t_error_titulo));
        count++;
    }
    fclose(pf);
    printf("Cargadas %d incidencias de auditoria de titulos desde: %s\n", count, path);
    return OK;
}

void listar_auditoria_miembros(const t_vector *vec_auditoria_miem)
{
    mostrar_encabezado_error_miembro();
    vector_mostrar(vec_auditoria_miem, sizeof(t_error_miembro), mostrar_error_miembro);
}

void listar_auditoria_titulos(const t_vector *vec_auditoria_tit)
{
    mostrar_encabezado_error_titulo();
    vector_mostrar(vec_auditoria_tit, sizeof(t_error_titulo), mostrar_error_titulo);
}

void listar_incidencias_miembros(t_vector *vec_auditoria_miem)
{
    int i=0,cant;

    int ce=vec_auditoria_miem->cantidad_actual;
    t_error_miembro *error_miembro = (t_error_miembro*) vec_auditoria_miem->vdatos;

    while(i<ce)
    {
        char *tipo = error_miembro[i].tipo_error;
        cant=0;

        while(i<ce && strcmp(error_miembro[i].tipo_error,tipo)==0)
        {
            cant++;
            i++;
        }

        printf("\n%s : %d\n",tipo,cant);
    }
}
void listar_incidencias_titulos(t_vector *vec_auditoria_tit)
{
    int i=0,cant;

    int ce=vec_auditoria_tit->cantidad_actual;
    t_error_titulo *error_titulo = (t_error_titulo*) vec_auditoria_tit->vdatos;

    while(i<ce)
    {
        char *tipo = error_titulo[i].tipo_error;
        cant=0;

        while(i<ce && strcmp(error_titulo[i].tipo_error,tipo)==0)
        {
            cant++;
            i++;
        }

        printf("\n%s : %d\n",tipo,cant);
    }
}
