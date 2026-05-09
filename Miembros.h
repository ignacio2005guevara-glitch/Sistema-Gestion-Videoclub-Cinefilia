#ifndef MIEMBROS_H_INCLUDED
#define MIEMBROS_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Auditoria.h"
#include "Validaciones.h"

#define MAX_NOMBRE 60
#define MAX_PLAN   10
#define MAX_EMAIL  30
#define MAX_CUIL   15

typedef struct
{
    long    dni;
    char    cuil[MAX_CUIL];
    char    nombre[MAX_NOMBRE];
    t_fecha fechaNacimiento;
    char    sexo;
    t_fecha fechaAfiliacion;
    char    categoria[10];      /* "MENOR" o "ADULTO" */
    t_fecha fechaUltimaCuota;
    char    estado;             /* 'A' o 'B' */
    char    plan[MAX_PLAN];
    char    emailTutor[MAX_EMAIL];
} t_miembro;

/* Carga y validacion */
void cargarMiembros(const char *path, t_indice *indice, t_auditoria *auditoria, t_fecha fechaProceso);

/* ABM */
int  altaMiembro(t_indice *indice, t_auditoria *auditoria, t_fecha fechaProceso);
int  bajaMiembro(t_indice *indice);
int  modificarMiembro(t_indice *indice, t_fecha fechaProceso);
void mostrarMiembro(const t_miembro *m);

/* Listados */
void listarMiembrosPorDNI (const t_indice *indice);
void listarMiembrosPorPlan(const t_indice *indice);

/* Persistencia */
void persistirMiembros(const t_indice *indice, t_fecha fechaProceso);

/* Comparador para el indice */
int cmpMiembro(const void *a, const void *b);

#endif /* MIEMBROS_H_INCLUDED */
