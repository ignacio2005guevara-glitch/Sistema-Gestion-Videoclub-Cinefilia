#include <stdio.h>
#include "Menu.h"
#include "Miembros.h"
#include "Titulos.h"

static void mostrarMenu(void)
{
    printf("\n===== CINEFILIA =====\n");
    printf("a. Alta de miembro\n");
    printf("b. Alta de titulo\n");
    printf("c. Baja de miembro\n");
    printf("d. Baja de titulo\n");
    printf("e. Modificacion de miembro\n");
    printf("f. Modificacion de titulo\n");
    printf("g. Mostrar informacion de miembro\n");
    printf("h. Alquiler de titulo\n");
    printf("i. Listado de miembros por DNI\n");
    printf("j. Listado de miembros por Plan\n");
    printf("k. Salir\n");
    printf("Opcion: ");
}

void menuPrincipal(t_indice *indiceMiembros, t_indice *indiceTitulos, t_fecha fechaProceso)
{
    char opcion;
    do
    {
        mostrarMenu();
        scanf(" %c", &opcion);

        switch (opcion)
        {
            case 'a': altaMiembro(indiceMiembros, NULL, fechaProceso); break;
            case 'b': altaTitulo(indiceTitulos); break;
            case 'c': bajaMiembro(indiceMiembros); break;
            case 'd': bajaTitulo(indiceTitulos); break;
            case 'e': modificarMiembro(indiceMiembros, fechaProceso); break;
            case 'f': modificarTitulo(indiceTitulos); break;
            case 'g': listarMiembrosPorDNI(indiceMiembros); break;
            case 'h': printf("[Alquiler] pendiente de implementacion\n"); break;
            case 'i': listarMiembrosPorDNI(indiceMiembros); break;
            case 'j': listarMiembrosPorPlan(indiceMiembros); break;
            case 'k': printf("Saliendo...\n"); break;
            default:  printf("Opcion invalida.\n");
        }
    } while (opcion != 'k');
}

void persistirDatos(t_indice *indiceMiembros, t_indice *indiceTitulos,
                    t_fecha fechaProceso)
{
    persistirMiembros(indiceMiembros, fechaProceso);
    persistirTitulos(indiceTitulos,  fechaProceso);
}
