#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Alquileres.h"
#include "Auditoria.h"
#include "Miembros.h"

void menu_principal(
    t_indice *indice_miembros,
    t_indice *indice_titulos,
    t_vector *vec_miembros,
    t_vector *vec_titulos,
    t_vector *vec_alquileres,
    t_vector *vec_auditoria_miem,
    t_vector *vec_auditoria_tit,
    t_fecha fecha_proceso
);
void persistir_datos(
    const t_vector *vec_miembros,
    const t_vector *vec_titulos,
    const t_vector *vec_alquileres,
    const t_vector *vec_auditoria_miem,
    const t_vector *vec_auditoria_tit,
    t_fecha fecha_proceso
);

#endif /* MENU_H_INCLUDED */
