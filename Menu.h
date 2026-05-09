#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Alquileres.h"
#include "Auditoria.h"

void menuPrincipal  (t_indice *indiceMiembros, t_indice *indiceTitulos, t_fecha fechaProceso);
void persistirDatos (t_indice *indiceMiembros, t_indice *indiceTitulos, t_fecha fechaProceso);

#endif /* MENU_H_INCLUDED */
