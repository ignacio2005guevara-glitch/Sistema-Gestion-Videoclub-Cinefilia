#include <stdio.h>
#include "Menu.h"
#include "Miembros.h"
#include "Titulos.h"

static void mostrar_menu(void)
{
    printf("\n===== CINEFILIA =====\n");
    printf("a. Alta de miembro\n");
    printf("b. Alta de titulo\n");
    printf("c. Baja de miembro\n");
    printf("d. Baja de titulo\n");
    printf("e. Modificacion de miembro\n");
    printf("f. Modificacion de titulo\n");
    printf("g. Mostrar informacion de miembro\n");
    printf("h. Mostrar informacion de titulo\n");
    printf("i. Alquiler de titulo\n");
    printf("j. Listado de miembros por DNI\n");
    printf("k. Listado de miembros por Plan\n");
    printf("l. Salir\n");
    printf("Opcion: ");
}

void menu_principal(t_indice *indice_miembros, t_indice *indice_titulos, t_vector *vec_miembros, t_vector *vec_titulos, t_vector *vec_alquileres, t_fecha fecha_proceso)
{
    char opcion;
    do
    {
        mostrar_menu();
        scanf(" %c", &opcion);

        switch (opcion)
        {
            case 'a': alta_miembro(indice_miembros, vec_miembros, fecha_proceso); break;
            case 'b': alta_titulo(indice_titulos, vec_titulos); break;
            case 'c': baja_miembro(indice_miembros, vec_miembros); break;
            case 'd': baja_titulo(indice_titulos, vec_titulos); break;
            case 'e': modificar_miembro(indice_miembros, vec_miembros, fecha_proceso); break;
            case 'f': modificar_titulo(indice_titulos, vec_titulos); break;
            case 'g': mostrar_miembro_por_dni(indice_miembros, vec_miembros); break;
            case 'h': mostrar_titulo_por_id(indice_titulos, vec_titulos); break;
            case 'i': alquilar_pelicula(indice_miembros, indice_titulos, vec_miembros, vec_titulos, vec_alquileres); break;
            case 'j': listar_miembros_por_DNI(indice_miembros, vec_miembros); break;
            case 'k': listar_miembros_por_plan(indice_miembros, vec_miembros); break;
            case 'l': printf("Saliendo...\n"); break;
            default:  printf("Opcion invalida.\n");
        }
    } while (opcion != 'k');
}

void persistir_datos(t_indice *indice_miembros, t_indice *indice_titulos,
                    t_fecha fecha_proceso)
{
    persistir_miembros(indice_miembros, fecha_proceso);
    persistir_titulos(indice_titulos,  fecha_proceso);
}
