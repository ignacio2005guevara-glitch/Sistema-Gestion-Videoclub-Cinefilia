#ifndef MIEMBROS_H_INCLUDED
#define MIEMBROS_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Auditoria.h"
#include "VectorDinamico.h"

#define MAX_NOMBRE 60
#define MAX_PLAN   10
#define MAX_EMAIL  30
#define MAX_CUIL   15
#define MAX_CARACTERES_MIEMBRO_CSV 300
#define CANT_CAMPOS_MIEMBRO 16

typedef struct {
    void *vmiembros;        /* void * si fuera genérico, acá es tipado */
    unsigned   cantidad_actual;
    unsigned   cantidad_maxima;
} t_vector_miembros;

/* Carga y validacion */
void cargar_miembros(const char *path, void* vec, t_indice *indice, t_error *auditoria, t_fecha fecha_proceso);

/* ABM */
int  alta_miembro(t_indice *indice, t_vector *vec, t_fecha fecha_proceso);
int  baja_miembro(t_indice *indice,t_vector *vec);
int  modificar_miembro(t_indice *indice, t_vector *vec, t_fecha fechaProceso);
//void mostrar_miembro(t_indice *indice);
void mostrar_miembro(const void *elem);
void mostrar_encabezado_miembros(void);
void mostrar_miembro_por_dni(const t_indice *indice, const t_vector *vec);

/* Listados */
void listar_miembros_por_DNI(const t_indice *indice, const t_vector *vec);
void listar_miembros_por_plan(const t_indice *indice, const t_vector *vec);
void mostrar_reg_indice_miembro(const void *elem);

int mostrar_archivo_miembros(const char* nombre_binario);
void miembro_mostrar_linea(const t_miembro* m);


/* Persistencia */
void persistir_miembros(const t_indice *indice, t_fecha fecha_proceso);

/* Comparador para el indice */
int cmp_miembro(const void *a, const void *b);

/* Comparador para el indice */
int cmp_DNI(const void *valor, const void *valor2);

int cmp_nombre(const void *a, const void *b);

#endif /* MIEMBROS_H_INCLUDED */
