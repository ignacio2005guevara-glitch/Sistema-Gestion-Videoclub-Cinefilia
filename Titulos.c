#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Titulos.h"

void cargar_titulos(const char *path, void *vec_titulos, t_indice *indice, void *vec_auditoria_titulos, t_fecha fecha_proceso)
{
    const char *Generos[] = {"Accion", "Drama", "Comedia", "Terror"};
    const char Estados[] = {'A','B'};
    int campos_leidos;

    FILE    *pfArchivo = fopen(path, "rt");
    t_titulo registro;
    t_reg_indice_titulo reg_indice;
    unsigned nro_reg = 0;
    char linea[MAX_CARACTERES_TITULO_CSV];

    if(!pfArchivo)
    {
        printf("Error al abrir archivo %s", path);
        return;
    }

    while(fgets(linea, sizeof(linea), pfArchivo))
    {
        // ídem para títulos
        memset(&registro, 0, sizeof(t_titulo));
        campos_leidos = sscanf(linea, "%d;%[^;];%[^;];%d;%c",
                                &registro.id_pelicula,
                                registro.titulo,
                                registro.genero,
                                &registro.stock,
                                &registro.estado);

        // Si no leyó los 5 campos esperados, el registro está incompleto
        if(campos_leidos != CANT_CAMPOS_TITULO)
        {
            continue;
        }

        // Validar ID
        if(!validar_id(&registro.id_pelicula, indice))
        {
            registrar_error_titulo(vec_auditoria_titulos, "ID", &registro.id_pelicula, &fecha_proceso);
            continue;
        }

        // Validar Titulo
//        if(!validar_nombre(registro.titulo))
//        {
//            //registrar_error(auditoria, "TITULO", &registro.id_pelicula);
//            continue;
//        }

        // Validar Genero
        if(!validar_plan_genero(registro.genero, Generos, CANT_PLANES_GENEROS))
        {
            registrar_error_titulo(vec_auditoria_titulos, "GENERO", &registro.id_pelicula, &fecha_proceso);
            continue;
        }

        // Validar Stock (>= 0, por defecto 0)
        if(!validar_stock(registro.stock, 1, 0))
        {
            registrar_error_titulo(vec_auditoria_titulos, "STOCK", &registro.id_pelicula, &fecha_proceso);
            continue;
        }

        // Validar Estado
        if(!validar_caracter(registro.estado, Estados, CANT_ESTADOS))
        {
            registrar_error_titulo(vec_auditoria_titulos, "ESTADO", &registro.id_pelicula, &fecha_proceso);
            continue;
        }

        // Normalizar titulo (idem nombres de miembros)
        char titulo_normalizado[MAX_TITULO];
        normalizar(registro.titulo, titulo_normalizado, 0);
        strcpy(registro.titulo, titulo_normalizado);

        // Revisar si la clave no existe en el indice
        reg_indice.id      = registro.id_pelicula;
        //reg_indice.nro_reg = 0;   /* nro_reg no importa para la busqueda */

        if(indice_buscar(indice, &reg_indice, indice->cantidad_elementos_actual, sizeof(t_reg_indice_titulo), cmp_id_pelicula) == NO_EXISTE)
        {
            // Nueva clave -> escribir en archivo e insertar en indice
//            archivo_bin_agregar(pf_titulos_bin, &registro,
//                                sizeof(t_titulo), &nro_reg);
//            reg_indice.nro_reg = nro_reg;
//            indice_insertar(indice, &reg_indice,
//                            sizeof(t_reg_indice_titulo), cmp_id);

            // Nueva clave -> escribir en archivo e insertar en indice
            vector_agregar(vec_titulos, &registro, sizeof(t_titulo));
            reg_indice.nro_reg = nro_reg;
            indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_titulo), cmp_id_pelicula);
            nro_reg++;
        }
        else
        {
            // ID duplicado
            //registrar_error(auditoria, "ID DUPLICADO", &registro.id_pelicula);
            continue;
        }
    }

    fclose(pfArchivo);
}

int  alta_titulo(t_indice *indice, t_vector *vec)
{
    t_titulo nuevo_titulo;
    t_reg_indice_titulo reg_indice;
    char buffer[MAX_CARACTERES_TITULO_CSV];
    const char *Generos[] = {"Accion", "Drama", "Comedia", "Terror"};

    /* ── Pedir todos los datos ── */
    printf("Ingrese Titulo: ");
    fflush(stdin);
    fgets(nuevo_titulo.titulo, MAX_TITULO, stdin);
    nuevo_titulo.titulo[strcspn(nuevo_titulo.titulo, "\n")] = '\0';


    printf("Ingrese Genero (Accion ,Drama, Comedia, Terror): ");
    fgets(nuevo_titulo.genero, MAX_GENERO, stdin);
    nuevo_titulo.genero[strcspn(nuevo_titulo.genero, "\n")] = '\0';
    fflush(stdin);

    printf("Ingrese Stock: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &nuevo_titulo.stock);
    fflush(stdin);

    /* ── Validaciones ── */

    // Validar Titulo existente
//    if(!validar_nombre(nuevo_titulo.titulo, vec))
//    {
//        printf("ERROR: Titulo repetido.\n");
//        return ERROR;
//    }

    // Validar titulo duplicado
    if(vector_buscar(vec, &nuevo_titulo, sizeof(t_titulo), cmp_titulo) != NO_EXISTE)
    {
        printf("ERROR: Ya existe una pelicula con ese titulo.\n");
        return ERROR;
    }

    // Validar Genero
    if(!validar_plan_genero(nuevo_titulo.genero, Generos, CANT_PLANES_GENEROS))
    {
        printf("ERROR: Genero inválido.\n");
        return ERROR;
    }
    //strcpy(nuevo_titulo.genero, Generos[opcion - 1]);

    // Validar Stock
    if(!validar_stock(nuevo_titulo.stock, 1, 0))
    {
        printf("ERROR: Stock invalido. Debe ser >= 0.\n");
        return ERROR;
    }

    // ID autoincremental
    nuevo_titulo.id_pelicula = generar_id_titulo(indice);
    reg_indice.id =  nuevo_titulo.id_pelicula;
    reg_indice.nro_reg = vec->cantidad_actual;

    // Normalizar titulo
    char titulo_normalizado[MAX_TITULO];
    normalizar(nuevo_titulo.titulo, titulo_normalizado, 0);
    strcpy(nuevo_titulo.titulo, titulo_normalizado);

    // Estado = 'A' por defecto
    nuevo_titulo.estado = 'A';

    vector_agregar(vec, &nuevo_titulo, sizeof(t_titulo));
    indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_titulo), cmp_id_pelicula);

    ///PRUEBA mostrar vector
    printf("\n================================================================================================================================================================\n");
    printf("%-9s | %-60s | %-20s | %-5s | %s\n",
           "ID", "TITULO", "GENERO", "STOCK", "ESTADO");
    printf("===================================================================================================================================================================\n");
    vector_mostrar(vec, sizeof(t_titulo), mostrar_titulo);
    //archivo_bin_mostrar(pf_miembros, sizeof(t_miembro), mostrar_miembro);

    /// PRUEBA mostrar indice
    printf("\nIndice Titulos:\n");
    indice_mostrar(indice, sizeof(t_reg_indice_titulo), mostrar_reg_indice_titulo);

    return OK;
}

int  baja_titulo(t_indice *indice, t_vector *vec)
{
    char validacion;
    int pos_indice;
    unsigned nro_reg;
    const t_reg_indice_titulo *indice_lectura = (const t_reg_indice_titulo*) indice->vindice;
    t_reg_indice_titulo titulo_a_buscar;
    t_titulo titulo;
    do{
    printf("\n\nIngrese el codigo del titulo que desea dar de baja:");
    scanf("%d",&titulo_a_buscar.id);
    if(titulo_a_buscar.id<1)
        printf("\nCodigo invalido, ingrese nuevamente\n");
    }while (titulo_a_buscar.id<1);
    pos_indice=indice_buscar(indice,&titulo_a_buscar,indice->cantidad_elementos_actual,sizeof(t_reg_indice_titulo),cmp_id_pelicula);
    if(pos_indice == -1)
    {
        printf("\nEl codigo de titulo ingresado no se encuentra en la base de datos\n");
        return ERROR;
    }
    else
    {
        nro_reg=indice_lectura[pos_indice].nro_reg;
        void* pos = vec->vdatos + nro_reg * sizeof(t_titulo);
        memcpy(&titulo,pos,sizeof(t_titulo));
        printf("DESEA BORRAR EL TITULO %s (Y/N):",titulo.titulo);
        fflush(stdin);
        scanf("%c",&validacion);
        if(validacion == 'y' || validacion == 'Y')
        {
            titulo.estado = 'B';
            memcpy(pos,&titulo,sizeof(t_titulo));
            indice_eliminar(indice,&titulo_a_buscar,sizeof(t_reg_indice_titulo),cmp_id_pelicula);
            printf("\nBAJA CONFIRMADA\n");
        }
        else
                        printf("\nBAJA CANCELADA\n");
    }

    /// PRUEBA mostar vec titulos
    printf("\n================================================================================================================================================================\n");
    printf("%-9s | %-60s | %-20s | %-5s | %s\n",
           "ID", "TITULO", "GENERO", "STOCK", "ESTADO");
    printf("===================================================================================================================================================================\n");
    vector_mostrar(vec, sizeof(t_titulo), mostrar_titulo);

    /// PRUEBA mostrar indice titulos
    printf("\nIndice Titulos:\n");
    indice_mostrar(indice, sizeof(t_reg_indice_titulo), mostrar_reg_indice_titulo);
    return 1;
}

int  modificar_titulo(t_indice *indice,t_vector *vec)
{
     const char *Generos[] = {"Accion", "Drama", "Comedia", "Terror"};
    int id_buscar;
    t_reg_indice reg_busqueda;
    int posicion_indice;

    printf("Ingrese el ID de la pelicula a modificar: ");
    scanf("%d", &id_buscar);

    reg_busqueda.dni = (long)id_buscar;
    posicion_indice = indice_buscar(indice, &reg_busqueda, indice->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_titulo);

    if (posicion_indice == NO_EXISTE) {
        printf("Error: El ID ingresado no pertenece a una pelicula activa.\n");
        return ERROR;
    }

    // 2. Acceder de forma correcta a la memoria usando el vdatos de tu t_vector
    t_reg_indice *vectorIndice = (t_reg_indice*)indice->vindice;
    unsigned pos_vector_titulos = vectorIndice[posicion_indice].nro_reg;

    t_titulo *pelicula = (t_titulo*)vec->vdatos + pos_vector_titulos;

    long id_viejo = (long)pelicula->id_pelicula; // por si cambiamos la clave
    int id_nuevo;
    int cambio_id = 0;

    int opcion;
    printf("\n--- CAMPOS MODIFICABLES DE TITULO ---\n");
    printf("1. ID Pelicula (Clave)\n");
    printf("2. Titulo de la Pelicula\n");
    printf("3. Genero\n");
    printf("4. Stock\n");
    printf("Elija el número del campo que desea modificar: ");
    scanf("%d", &opcion);

    switch(opcion) {
        case 1: { // Modificar ID
            printf("Ingrese el nuevo ID de la pelicula: ");
            scanf("%d", &id_nuevo);

            if (id_nuevo <= 0) {
                printf("Error: El ID debe ser un numero positivo.\n");
                return ERROR;
            }

            // Verificamos que no este duplicado
            t_reg_indice reg_duplicado;
            reg_duplicado.dni = (long)id_nuevo;
            if (indice_buscar(indice, &reg_duplicado, indice->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_titulo) != NO_EXISTE) {
                printf("Error: El ID nuevo ya pertenece a otra pelicula.\n");
                return ERROR;
            }

            pelicula->id_pelicula = id_nuevo;
            cambio_id = 1; // bandera de cambio
            break;
        }

        case 2: { // Modificar Título
            char nuevo_titulo[60];
            printf("Ingrese el nuevo Titulo: ");
            fflush(stdin);
            fgets(nuevo_titulo, 60, stdin);

            char titulo_normalizado[60];
            normalizar(nuevo_titulo, titulo_normalizado,0);
            strcpy(pelicula->titulo, titulo_normalizado);
            break;
        }

        case 3: { // Modificar Género
            char nuevo_genero[20];
            printf("Ingrese el nuevo Genero (Accion / Drama / Comedia / Terror): ");
            fflush(stdin);
            fgets(nuevo_genero, 20, stdin);

            if (!validar_plan_genero(nuevo_genero, Generos, CANT_PLANES_GENEROS))
                {
                printf("Error: Genero no valido.\n");
                return ERROR;
                }

            strcpy(pelicula->genero, nuevo_genero);
            break;
        }

        case 4: { // Modificar Stock
            int nuevo_stock;
            printf("Ingrese la nueva cantidad de Stock (>= 0): ");
            scanf("%d", &nuevo_stock);

            if (!validar_stock(nuevo_stock, 1, 0))
                {
                printf("Error: El stock debe ser mayor o igual a 0.\n");
                return ERROR;
                }

            pelicula->stock = nuevo_stock;
            break;
        }

        default:
            printf("Opcion invalida. Volviendo al menu.\n");
            return ERROR;
    }

    // Si modificamos el id entonces actualizamos el indice
    if (cambio_id == 1) {
        t_reg_indice reg_viejo;
        reg_viejo.dni = id_viejo;
        reg_viejo.nro_reg = pos_vector_titulos;

        indice_eliminar(indice, &reg_viejo, sizeof(t_reg_indice), cmp_titulo);

        t_reg_indice reg_nuevo;
        reg_nuevo.dni = (long)id_nuevo;
        reg_nuevo.nro_reg = pos_vector_titulos; // Apunta al mismo casillero del vector

        indice_insertar(indice, &reg_nuevo, sizeof(t_reg_indice), cmp_titulo);
    }

    printf("¡Titulo de pelicula modificado con exito en memoria!\n");

        /// PRUEBA mostar vec titulos
    printf("\n================================================================================================================================================================\n");
    printf("%-9s | %-60s | %-20s | %-5s | %s\n",
           "ID", "TITULO", "GENERO", "STOCK", "ESTADO");
    printf("===================================================================================================================================================================\n");
    vector_mostrar(vec, sizeof(t_titulo), mostrar_titulo);

    /// PRUEBA mostrar indice titulos
    printf("\nIndice Titulos:\n");
    indice_mostrar(indice, sizeof(t_reg_indice_titulo), mostrar_reg_indice_titulo);


    return OK;
}

void persistir_titulos(const t_indice *indice, t_fecha fechaProceso)
{

}

int cmp_id_pelicula(const void *valor1, const void *valor2)
{
    const t_reg_indice_titulo *v1 = (const t_reg_indice_titulo*) valor1;
    const t_reg_indice_titulo *v2 = (const t_reg_indice_titulo*) valor2;

    if(v1->id < v2->id)
        return -1;
    else if(v1->id > v2->id)
        return 1;
    else
        return 0;
}

void mostrar_titulo(const void *elem)
{
    const t_titulo *t = (const t_titulo *)elem;

    printf("%-9d | %-*.*s | %-*.*s | %-5d | %c\n",
           t->id_pelicula,
           (int)MAX_TITULO, (int)MAX_TITULO, t->titulo,
           (int)MAX_GENERO, (int)MAX_GENERO, t->genero,
           t->stock,
           t->estado);
}

void mostrar_reg_indice_titulo(const void *elem)
{
    const t_reg_indice_titulo *r = (const t_reg_indice_titulo *)elem;
    printf("nro_reg: %2u | ID Pelicula: %d\n", r->nro_reg, r->id);
}

void mostrar_encabezado_titulos(void)
{
    printf("\n============================================================================================================\n");
    printf("%-9s | %-60s | %-20s | %-5s | %s\n",
           "ID", "TITULO", "GENERO", "STOCK", "ESTADO");
    printf("===============================================================================================================\n");
}

void mostrar_titulo_por_id(const t_indice *indice, const t_vector *vec)
{
    t_reg_indice_titulo clave;
    //t_reg_indice_miembro reg;
    t_titulo titulo;
    int pos, id;

    printf("Ingrese ID de la pelicula: ");
    if(scanf("%d", &id) != 1)
    {
        printf("DNI invalido.\n");
        return;
    }

    clave.id     = id;
    clave.nro_reg = 0;

    pos = indice_buscar(indice, &clave, indice->cantidad_elementos_actual, sizeof(t_reg_indice_titulo), cmp_id_pelicula);
    if(pos == NO_EXISTE)
    {
        printf("ERROR: No existe una pelicula con el ID %d.\n", id);
        return;
    }

    t_reg_indice *vectorIndice = (t_reg_indice*)indice->vindice;
    unsigned pos_vector_peliculas = vectorIndice[pos].nro_reg;
    memcpy(&titulo, vec->vdatos + pos_vector_peliculas * sizeof(t_titulo), sizeof(t_titulo));
    mostrar_encabezado_titulos();
    mostrar_titulo(&titulo);
}

int generar_id_titulo(const t_indice *indice)
{
    t_reg_indice_titulo ultimo;

    if(indice_vacio(indice) == OK)
    {
        return 1;   /* primer titulo */
    }

    /* el ultimo elemento del indice tiene el mayor id */
    indice_obtener(indice, indice->cantidad_elementos_actual - 1, &ultimo, sizeof(t_reg_indice_titulo));

    return ultimo.id + 1;
}

int cmp_titulo(const void *valor1, const void *valor2)
{
    const t_titulo *v1 = (const t_titulo *)valor1;
    const t_titulo *v2 = (const t_titulo *)valor2;

    return strcmp(v1->titulo, v2->titulo);
}

