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
    t_vector vec_miembros;
    t_vector vec_titulos;
    t_vector vec_alquileres;
    t_vector vec_auditoria_miem;
    t_vector vec_auditoria_tit;

    char ruta_archivo_miembros[] = "Archivos\\miembros.csv";
    char ruta_archivo_titulos[] = "Archivos\\titulos.csv";



    // Solicitar y validar fecha de proceso
    fecha_proceso = obtener_fecha_proceso();
    printf("Fecha de proceso: %02d/%02d/%04d\n", fecha_proceso.dia, fecha_proceso.mes, fecha_proceso.anio);

    // Crear indices y vectores para Miembros y Peliculas
    indice_crear(&indice_miembros, CANTIDAD_ELEMENTOS, sizeof(t_reg_indice));
    indice_crear(&indice_titulos,  CANTIDAD_ELEMENTOS, sizeof(t_reg_indice));
    vector_crear(&vec_miembros, CANTIDAD_ELEMENTOS, sizeof(t_miembro));
    vector_crear(&vec_titulos, CANTIDAD_ELEMENTOS, sizeof(t_titulo));
    vector_crear(&vec_alquileres, CANTIDAD_ELEMENTOS, sizeof(t_alquiler));
    vector_crear(&vec_auditoria_miem, CANTIDAD_ELEMENTOS, sizeof(t_error_miembro));
    vector_crear(&vec_auditoria_tit, CANTIDAD_ELEMENTOS, sizeof(t_error_titulo));


    // Cargar miembros, titulos y alquileres (desde binarios si existen, sino desde CSV)
    char path_miembros_bin[100];
    char path_titulos_bin[100];
    char path_alquileres_bin[100];
    sprintf(path_miembros_bin, "Archivos\\miembros_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);
    sprintf(path_titulos_bin, "Archivos\\titulos_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);
    sprintf(path_alquileres_bin, "Archivos\\alquileres_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);


    if (archivo_existe(path_miembros_bin, "rb") && archivo_existe(path_titulos_bin, "rb"))
    {
        printf("Archivos binarios encontrados para la fecha. Cargando datos desde archivos binarios.\n");
        cargar_miembros_bin(path_miembros_bin, &vec_miembros, &indice_miembros);
        cargar_titulos_bin(path_titulos_bin, &vec_titulos, &indice_titulos);
        cargar_alquileres_bin(path_alquileres_bin, &vec_alquileres);

        char path_auditoria_miem_bin[100];
        char path_auditoria_tit_bin[100];
        sprintf(path_auditoria_miem_bin, "Archivos\\auditoria_miembros_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);
        sprintf(path_auditoria_tit_bin, "Archivos\\auditoria_titulos_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);
        cargar_auditoria_miembros_bin(path_auditoria_miem_bin, &vec_auditoria_miem);
        cargar_auditoria_titulos_bin(path_auditoria_tit_bin, &vec_auditoria_tit);
    }
    else
    {
        if (!archivo_existe(ruta_archivo_miembros, "rt") || !archivo_existe(ruta_archivo_titulos, "rt"))
        {
            printf("Error: No se encontraron los archivos binarios ni los archivos CSV de origen (%s / %s).\nNo se puede iniciar la aplicacion.\n", ruta_archivo_miembros, ruta_archivo_titulos);
            indice_vaciar(&indice_miembros);
            indice_vaciar(&indice_titulos);
            vector_vaciar(&vec_miembros);
            vector_vaciar(&vec_titulos);
            vector_vaciar(&vec_alquileres);
            vector_vaciar(&vec_auditoria_miem);
            vector_vaciar(&vec_auditoria_tit);
            return 1;
        }

        printf("Archivos binarios no encontrados. Cargando y validando desde archivos CSV.\n");
        cargar_miembros(ruta_archivo_miembros, &vec_miembros, &indice_miembros, &vec_auditoria_miem, fecha_proceso);
        cargar_titulos(ruta_archivo_titulos, &vec_titulos, &indice_titulos, &vec_auditoria_tit, fecha_proceso);
    }


    // Menu principal
    menu_principal(&indice_miembros, &indice_titulos, &vec_miembros, &vec_titulos, &vec_alquileres, &vec_auditoria_miem, &vec_auditoria_tit, fecha_proceso);

    // Persistir datos y liberar memoria
    persistir_datos(&vec_miembros, &vec_titulos, &vec_alquileres, &vec_auditoria_miem, &vec_auditoria_tit, fecha_proceso);
    indice_vaciar(&indice_miembros);
    indice_vaciar(&indice_titulos);
    vector_vaciar(&vec_miembros);
    vector_vaciar(&vec_titulos);
    vector_vaciar(&vec_alquileres);
    vector_vaciar(&vec_auditoria_miem);
    vector_vaciar(&vec_auditoria_tit);

    return 0;
}
