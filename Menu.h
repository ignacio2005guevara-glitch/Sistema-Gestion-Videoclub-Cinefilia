#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Alquileres.h"
#include "Auditoria.h"
#include "Miembros.h"

void menu_principal(t_indice *indice_miembros, t_indice *indice_titulos, t_vector *vec_miembros, t_vector *vec_titulos, t_vector *vec_alquileres, t_fecha fecha_proceso);
void persistir_datos(t_indice *indice_miembros, t_indice *indice_titulos, t_fecha fecha_proceso);

#endif /* MENU_H_INCLUDED */
