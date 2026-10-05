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
    printf("l. Listar tipos de errores de miembros\n");
    printf("m. Listar tipos de errores de titulos\n");
    printf("n. Mostrar auditoria de miembros\n");
    printf("o. Mostrar auditoria de titulos\n");
    printf("1. Implementacion nueva 1.Listar titulos con stock cero ordenados por título alfabeticamente\n"); ///1.
    printf("2. Implementacion nueva 2.Listar miembros dado de baja ordenado por DNI\n"); ///2
    printf("3. Implementacion nueva 3. Mostrar cantidad de miembros por categoria(MENOR/ADULTO)\n"); ///3.
    printf("4. Implementacion nueva 4. Dado un genero ingresado por teclado, listar los titulos de ese genero ordenados por titulo\n");///4.
    printf("5. Implementacion nueva 5.Listar los miembros que tienen al menos un alquiler registrado, ordenados por apellido y nombre\n");///5.
    printf("6. Implementacion nueva 6.Listar titulos ordenados por stock descendente\n");///6.
    printf("7. Implementacion nueva 7.Dado un plan ingresado por teclado, mostrar cuántos miembros activos tiene ese plan\n");///7.
    printf("8. Implementacion nueva 8.Listar los titulos que nunca fueron alquilados\n");///8.
    printf("9. Implementacion nueva 9.Dado un DNI, mostrar todos los titulos que alquilo con su cantidad de veces\n");///9.
    printf("0(es el 10.)Implementacion nueva 10.Listar los miembros activos cuya ultima cuota paga supera los 90 dias desde la fecha de proceso, ordenados por fecha de ultima cuota ascendente.\n");///10.
    printf("q(es el 11.)Implementacion nueva 11.Dado un DNI ingresado por teclado, mostrar si existe o no en el índice y su posición\n");///11.
    printf("r(es el 12.)Implementacion nueva 12.Mostrar todos los DNIs del indice en orden\n");///12.
    printf("s(es el 13.)Implementacion nueva 13.Dado un DNI mostrar todos los datos del miembro utilizando indice\n");///13.
    printf("t(es el 14.)Implementacion nueva 14.Contar cuantos miembros activos hay en el indice\n");///14.
    printf("u(es el 15.)Implementacion nueva 15.Dado un rango de DNIs, listar todos los miembros de ese rango\n");///15.
    printf("p. Salir\n");
    printf("Opcion: ");
}

void menu_principal(
    t_indice *indice_miembros,
    t_indice *indice_titulos,
    t_vector *vec_miembros,
    t_vector *vec_titulos,
    t_vector *vec_alquileres,
    t_vector *vec_auditoria_miem,
    t_vector *vec_auditoria_tit,
    t_fecha fecha_proceso
)
{
    char opcion;
    do
    {
        mostrar_menu();
        scanf(" %c", &opcion);
        opcion = MI_TOLOWER(opcion);
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
            case 'l': listar_incidencias_miembros(vec_auditoria_miem); break;
            case 'm': listar_incidencias_titulos(vec_auditoria_tit); break;
            case 'n': listar_auditoria_miembros(vec_auditoria_miem);break;
            case 'o': listar_auditoria_titulos(vec_auditoria_tit);break;
            case '1': listar_titulos_stock_cero(vec_titulos);break;
            case '2': listar_miembros_dados_baja_por_DNI(vec_miembros);break;
            case '3': mostrar_cantidad_miembros_por_categoria(vec_miembros);break;
            case '4': listar_titulos_del_genero_ingresado_ordenadoxtitulo(vec_titulos);break;
            case '5': listar_miembros_con_almenos_un_alquiler_ordenadoxApellidoyNombre(vec_miembros,vec_alquileres);break;
            case '6': listar_titulos_ordenados_por_stock_descendente(vec_titulos);break;
            case '7': listar_miembros_activos_por_PlanIngresado(vec_miembros);break;
            case '8': listar_titulos_nunca_alquilados(vec_titulos,vec_alquileres);break;
            case '9': PorIngresoDNI_mostrar_titulos_alquilados(indice_miembros,vec_alquileres,vec_titulos);break;
            case '0': listar_miembros_con_ultima_cuota_paga_superior_a_90_desde_fecha_proceso(vec_miembros,fecha_proceso);break;
            case 'q': PorIngresoDNI_mostrar_si_existe_en_indice_miembros(indice_miembros);break;
            case 'r': mostrar_dni_del_indice_en_orden(indice_miembros);break;
            case 's': PorIngresoDNI_mostrar_datos_completos_miembro_con_indice(indice_miembros,vec_miembros);break;
            case 't': Contar_miembros_activos_con_indice(indice_miembros);break;
            case 'u': listar_miembros_por_rango_dni_ingresado_con_indice(indice_miembros,vec_miembros);break;
            case 'p': printf("Saliendo...\n"); break;
            default:  printf("Opcion invalida.\n");
        }
    } while (opcion != 'p');
}

void persistir_datos(
    const t_vector *vec_miembros,
    const t_vector *vec_titulos,
    const t_vector *vec_alquileres,
    const t_vector *vec_auditoria_miem,
    const t_vector *vec_auditoria_tit,
    t_fecha fecha_proceso
)
{
    persistir_miembros(vec_miembros, fecha_proceso);
    persistir_titulos(vec_titulos, fecha_proceso);
    persistir_alquileres(vec_alquileres, fecha_proceso);
    persistir_auditorias(vec_auditoria_miem, vec_auditoria_tit, fecha_proceso);
}
