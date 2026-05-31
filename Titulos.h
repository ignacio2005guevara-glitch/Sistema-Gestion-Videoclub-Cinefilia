#ifndef TITULOS_H_INCLUDED
#define TITULOS_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Auditoria.h"
#include "Miembros.h"
#include "VectorDinamico.h"

#define MAX_CARACTERES_TITULO_CSV 300
#define CANT_CAMPOS_TITULO  5


void cargar_titulos(const char *path, void* vec, t_indice *indice, t_error *auditoria, t_fecha fecha_proceso);
int  alta_titulo(t_indice *indice, t_vector *vec);
int  baja_titulo(t_indice *indice, t_vector *vec);
int  modificar_titulo(t_indice *indice,t_vector *vec);
void persistir_titulos(const t_indice *indice, t_fecha fechaProceso);
int cmp_id_pelicula(const void *valor1, const void *valor2);
int cmp_titulo(const void *valor1, const void *valor2);
void mostrar_reg_indice_titulo(const void *elem);
void mostrar_titulo(const void *elem);
void mostrar_encabezado_titulos(void);
void mostrar_titulo_por_id(const t_indice *indice, const t_vector *vec);
int generar_id_titulo(const t_indice *indice);

#endif /* TITULOS_H_INCLUDED */
