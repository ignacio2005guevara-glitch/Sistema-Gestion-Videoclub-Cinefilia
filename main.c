#include <stdio.h>
#include <stdlib.h>
#include "Indice.h"
#include "Validaciones.h"
#include "Miembros.h"
#include "Titulos.h"
#include "Alquileres.h"
#include "Menu.h"
#include "Auditoria.h"
#include "VectorDinamico.h"
#include "Estructuras.h"

int main()
{
    t_fecha fecha_proceso;
    t_indice indice_miembros;
    t_indice indice_titulos;
    t_error auditoria_miembros;
    //t_error auditoria_titulos;
    t_vector vec_miembros;
    t_vector vec_titulos;
    t_vector vec_alquileres;

    char ruta_archivo_miembros[] = "Archivos\\miembros.csv";
    char ruta_archivo_titulos[] = "Archivos\\titulos.csv";
    //char ruta_archivo_miembros_bin[50];
    //char ruta_archivo_miembros_indice_bin[50];


    /* 1. Solicitar y validar fecha de proceso */
    fecha_proceso = obtener_fecha_proceso();
    printf("Fecha de proceso: %02d/%02d/%04d\n", fecha_proceso.dia, fecha_proceso.mes, fecha_proceso.anio);

    /* 2. Crear indices y vectores para Miembros y Peliculas*/
    indice_crear(&indice_miembros, CANTIDAD_ELEMENTOS, sizeof(t_reg_indice));
    indice_crear(&indice_titulos,  CANTIDAD_ELEMENTOS, sizeof(t_reg_indice));
    vector_crear(&vec_miembros, CANTIDAD_ELEMENTOS, sizeof(t_miembro));
    vector_crear(&vec_titulos, CANTIDAD_ELEMENTOS, sizeof(t_titulo));
    vector_crear(&vec_alquileres, CANTIDAD_ELEMENTOS, sizeof(t_alquiler));


    /* 3. Cargar y validar miembros desde CSV */
    //sprintf(ruta_archivo_miembros_bin, "miembros_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);
    cargar_miembros(ruta_archivo_miembros, &vec_miembros, &indice_miembros, &auditoria_miembros, fecha_proceso);

    /// PRUEBA archivo bin miembros (YA NO LO CARGAMOS EN UN ARHIVO BINARIO DURANTE EJECUCION, SOLO AL FINAL)
    //mostrar_archivo_miembros(ruta_archivo_miembros_bin);

    /// PRUEBA mostrar vec miembros
//    printf("\n=========================================================================================================================================================================================\n");
//    printf("%-9s | %-13s | %-60s | %-10s | %s | %-10s | %-10s | %-10s | %s | %-10s | %-30s\n",
//           "DNI", "CUIL", "APELLIDO Y NOMBRE", "F. NAC", "S", "F. AFIL", "CATEGORIA", "U. PAGO", "E", "PLAN", "EMAIL TUTOR");
//    printf("=========================================================================================================================================================================================\n");
//    vector_mostrar(&vec_miembros, sizeof(t_miembro), mostrar_miembro);
//
//    /// PRUEBA mostrar indice miembros
//    printf("\nIndice Miembros:\n");
//    indice_mostrar(&indice_miembros, sizeof(t_reg_indice_miembro), mostrar_reg_indice_miembro);
//
    /* 4. Cargar y validar titulos desde CSV */
    cargar_titulos(ruta_archivo_titulos, &vec_titulos, &indice_titulos, &auditoria_miembros, fecha_proceso);

    /// PRUEBA mostar vec titulos
    printf("\n================================================================================================================================================================\n");
    printf("%-9s | %-60s | %-20s | %-5s | %s\n",
           "ID", "TITULO", "GENERO", "STOCK", "ESTADO");
    printf("===================================================================================================================================================================\n");
    vector_mostrar(&vec_titulos, sizeof(t_titulo), mostrar_titulo);

    /// PRUEBA mostrar indice titulos
    printf("\nIndice Titulos:\n");
    indice_mostrar(&indice_titulos, sizeof(t_reg_indice_titulo), mostrar_reg_indice_titulo);



    /* 5. Menu principal */
    menu_principal(&indice_miembros, &indice_titulos, &vec_miembros, &vec_titulos, &vec_alquileres,  fecha_proceso);

    /* 6. Persistir datos y liberar memoria */
    //persistir_datos(&vec, &indice_titulos, fecha_proceso);
    indice_vaciar(&indice_miembros);
    indice_vaciar(&indice_titulos);
    vector_vaciar(&vec_miembros);
    vector_vaciar(&vec_titulos);
    vector_vaciar(&vec_alquileres);

    return 0;
}
