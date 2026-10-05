#ifndef AUDITORIA_H_INCLUDED
#define AUDITORIA_H_INCLUDED

#include "Estructuras.h"
#include "VectorDinamico.h"

#define MAX_TIPO_ERROR 20
#define MAX_ERRORES    200

typedef struct
{
    char tipo_error[MAX_TIPO_ERROR]; // "DNI", "Fecha", "Email", etc.
    long dni;
    t_fecha fecha;
}t_error_miembro;

typedef struct
{
    char tipo_error[MAX_TIPO_ERROR];
    int id_pelicula;
    t_fecha fecha;
}t_error_titulo;


void registrar_error_miembro(t_vector *auditoria, char* tipo_error, long* dni, t_fecha *fecha_proceso);
void registrar_error_titulo(t_vector *auditoria, char* tipo_error, int* id_pelicula, t_fecha *fecha_proceso);
void mostrar_encabezado_error_miembro(void);
void mostrar_error_miembro(const void *elem);
void mostrar_encabezado_error_titulo(void);
void mostrar_error_titulo(const void *elem);
int cmp_error_miembro(const void *valor1, const void *valor2);
int cmp_error_titulo(const void *valor1, const void *valor2);
void persistir_auditorias(const t_vector *vec_auditoria_miem, const t_vector *vec_auditoria_tit, t_fecha fecha_proceso);
int cargar_auditoria_miembros_bin(const char *path, t_vector *vec);
int cargar_auditoria_titulos_bin(const char *path, t_vector *vec);
void listar_auditoria_miembros(const t_vector *vec_auditoria_miem);
void listar_auditoria_titulos(const t_vector *vec_auditoria_tit);
void listar_incidencias_titulos(t_vector *vec_auditoria_tit);
void listar_incidencias_miembros(t_vector *vec_auditoria_miem);

#endif /* AUDITORIA_H_INCLUDED */
