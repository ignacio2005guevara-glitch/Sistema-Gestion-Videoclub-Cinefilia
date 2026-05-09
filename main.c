#include <stdio.h>
#include <stdlib.h>
#include "Indice.h"
#include "Validaciones.h"
#include "Miembros.h"
#include "Titulos.h"
#include "Alquileres.h"
#include "Menu.h"
#include "Auditoria.h"

int main()
{
    t_fecha fechaProceso;
    t_indice indiceMiembros;
    t_indice indiceTitulos;
    t_auditoria auditoriaMiembros;
    t_auditoria auditoriaTitulos;

    /* 1. Solicitar y validar fecha de proceso */
    //obtenerFechaProceso(&fechaProceso);

    /* 2. Crear indices */
    indice_crear(&indiceMiembros, CANTIDAD_ELEMENTOS, sizeof(t_reg_indice));
    indice_crear(&indiceTitulos,  CANTIDAD_ELEMENTOS, sizeof(t_reg_indice));

    /* 3. Cargar y validar miembros desde CSV */
    cargarMiembros("miembros.csv", &indiceMiembros, &auditoriaMiembros, fechaProceso);

    /* 4. Cargar y validar titulos desde CSV */
    cargarTitulos("titulos.csv", &indiceTitulos, &auditoriaTitulos);

    /* 5. Menu principal */
    menuPrincipal(&indiceMiembros, &indiceTitulos, fechaProceso);

    /* 6. Persistir datos y liberar memoria */
    persistirDatos(&indiceMiembros, &indiceTitulos, fechaProceso);
    indice_vaciar(&indiceMiembros);
    indice_vaciar(&indiceTitulos);

    return 0;
}
