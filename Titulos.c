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

        if(indice_buscar(indice, &reg_indice, indice->cantidad_elementos_actual, sizeof(t_reg_indice_titulo), cmp_id_pelicula) == NO_EXISTE)
        {
            vector_agregar(vec_titulos, &registro, sizeof(t_titulo));
            reg_indice.nro_reg = nro_reg;
            indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_titulo), cmp_id_pelicula);
            nro_reg++;
        }
        else
        {
            // ID duplicado
            continue;
        }
    }

    fclose(pfArchivo);
}

int  alta_titulo(t_indice *indice, t_vector *vec)
{
    t_titulo nuevo_titulo;
    t_reg_indice_titulo reg_indice;
    const char *Generos[] = {"Accion", "Drama", "Comedia", "Terror"};

    printf("Ingrese Titulo: ");
    fflush(stdin);
    fgets(nuevo_titulo.titulo, MAX_TITULO, stdin);
    nuevo_titulo.titulo[strcspn(nuevo_titulo.titulo, "\n")] = '\0';
    normalizar(nuevo_titulo.titulo,nuevo_titulo.titulo,0);

    printf("Ingrese Genero (Accion ,Drama, Comedia, Terror): ");
    fflush(stdin);
    fgets(nuevo_titulo.genero, MAX_GENERO, stdin);
    nuevo_titulo.genero[strcspn(nuevo_titulo.genero, "\n")] = '\0';
    normalizar(nuevo_titulo.genero,nuevo_titulo.genero,0);


    printf("Ingrese Stock: ");
    scanf("%d",&nuevo_titulo.stock);


    // Validar titulo duplicado
    if(vector_buscar(vec, &nuevo_titulo, sizeof(t_titulo), cmp_titulo) != NO_EXISTE)
    {
        printf("ERROR: YA EXISTE UNA PELICULA CON ESE TITULO.\n");
        return ERROR;
    }

    // Validar Genero
    if(!validar_plan_genero(nuevo_titulo.genero, Generos, CANT_PLANES_GENEROS))
    {
        printf("ERROR: GENERO INVALIDO\n");
        return ERROR;
    }

    // Validar Stock
    if(!validar_stock(nuevo_titulo.stock, 1, 0))
    {
        printf("ERROR: STOCK INVALIDO. DEBE SER >= 0.\n");
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

    printf("\nTITULO DADO DE ALTA CORRECTAMENTE CON ID %d\n",nuevo_titulo.id_pelicula);
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
        validacion = MI_TOUPPER(validacion);
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
    return OK;
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
    posicion_indice = indice_buscar(indice, &reg_busqueda, indice->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_id_pelicula);

    if (posicion_indice == NO_EXISTE) {
        printf("Error: El ID ingresado no pertenece a una pelicula activa.\n");
        return ERROR;
    }

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
    printf("Elija el numero del campo que desea modificar: ");
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
            cambio_id = 1; // flag de cambio
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

    printf("Titulo de pelicula modificado con exito en memoria\n");
    return OK;
}

void persistir_titulos(const t_vector *vec, t_fecha fecha_proceso)
{
    char path[100];
    sprintf(path, "Archivos\\titulos_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);

    FILE *pf = fopen(path, "wb");
    if (!pf)
    {
        printf("Error al crear archivo de persistencia de titulos: %s\n", path);
        return;
    }

    fwrite(vec->vdatos, sizeof(t_titulo), vec->cantidad_actual, pf);
    fclose(pf);
    printf("Titulos persistidos en: %s\n", path);
}

int cargar_titulos_bin(const char *path, t_vector *vec_titulos, t_indice *indice)
{
    FILE *pf = fopen(path, "rb");
    if (!pf)
    {
        return ERROR;
    }

    t_titulo t;
    t_reg_indice_titulo reg_indice;
    unsigned nro_reg = 0;

    while (fread(&t, sizeof(t_titulo), 1, pf) == 1)
    {
        vector_agregar(vec_titulos, &t, sizeof(t_titulo));

        reg_indice.id = t.id_pelicula;
        reg_indice.nro_reg = nro_reg;

        if (t.estado == 'A')
        {
            indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_titulo), cmp_id_pelicula);
        }
        nro_reg++;
    }

    fclose(pf);
    return OK;
}

int cmp_id_pelicula(const void *valor1, const void *valor2)
{
    const t_reg_indice_titulo *v1 = (const t_reg_indice_titulo*) valor1;
    const t_reg_indice_titulo *v2 = (const t_reg_indice_titulo*) valor2;

    if(v1->id < v2->id)
        return -1;
    else if(v1->id > v2->id)
        return OK;
    else
        return ERROR;
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
        return ERROR;
    }


    indice_obtener(indice, indice->cantidad_elementos_actual - 1, &ultimo, sizeof(t_reg_indice_titulo));

    return ultimo.id + 1;
}


///1.Listar todos los títulos con stock 0, ordenados por título alfabéticamente

void listar_titulos_stock_cero(const t_vector *vec_titulos)
{
    int i,encontrados=0;
    t_titulo *veccopia = malloc(vec_titulos->cantidad_actual * sizeof(t_titulo));
    memcpy(veccopia,vec_titulos->vdatos,vec_titulos->cantidad_actual * sizeof(t_titulo));

    qsort(veccopia, vec_titulos->cantidad_actual, sizeof(t_titulo), cmp_titulo);

    printf("\nTitulos con Stock CERO:");
    for(i=0;i<vec_titulos->cantidad_actual;i++)
    {
        if(veccopia[i].stock == 0)
        {
            printf("\n%s",veccopia[i].titulo);
            encontrados ++;
        }
    }
    if(encontrados==0)
    {
        printf("\nNINGUN titulo cuenta con stock CERO");
    }
    free(veccopia);
}
///----------------------------------------------------------------------------

///4.Dado un género ingresado por teclado, listar los títulos de ese género ordenados por título

void listar_titulos_del_genero_ingresado_ordenadoxtitulo(const t_vector *vec_titulos)
{
    int i,encontrados=0;
    char genero[MAX_GENERO];
    const char *Generos[]={"Accion","Drama","Comedia","Terror"};

    t_titulo *veccopia = malloc(vec_titulos->cantidad_actual * sizeof(t_titulo));
    memcpy(veccopia,vec_titulos->vdatos,vec_titulos->cantidad_actual * sizeof(t_titulo));

    qsort(veccopia,vec_titulos->cantidad_actual,sizeof(t_titulo),cmp_titulo);


    while(getchar() != '\n'); //fflush(stdin);
    do{
        printf("\nIngrese el GENERO: Accion-Drama-Comedia-Terror\n");
        fflush(stdin);
        fgets(genero, MAX_GENERO, stdin);
        genero[strcspn(genero, "\n")] = '\0';
        normalizar(genero,genero,0);
        if(!validar_plan_genero(genero,Generos,CANT_PLANES_GENEROS))
        {
            printf("\nGENERO INCORRECTO, VOLVER A INGRESAR...");
        }
    }while(!validar_plan_genero(genero,Generos,CANT_PLANES_GENEROS));


    printf("\nTITULOS con genero:%s",genero);
    for(i=0;i<vec_titulos->cantidad_actual;i++)
    {
        if(strcmp(veccopia[i].genero,genero) == 0)
        {
            encontrados++;
            printf("\n%s\t%s", veccopia[i].titulo, veccopia[i].genero);
            printf("\n");
        }
    }

    if(encontrados==0)
    {
        printf("\nNinguno...");
    }

    free(veccopia);
}

///----------------------------------------------------------------------------------------------

///6.Listar títulos ordenados por stock descendente

void listar_titulos_ordenados_por_stock_descendente(const t_vector *vec_titulos)
{
    int i;
    t_titulo *veccopia = malloc(vec_titulos->cantidad_actual * sizeof(t_titulo));
    memcpy(veccopia,vec_titulos->vdatos,vec_titulos->cantidad_actual * sizeof(t_titulo));

    qsort(veccopia,vec_titulos->cantidad_actual,sizeof(t_titulo),cmp_stock_descendente);
    printf("\nTitulos ordenados por stock descendente:");
    for(i=0;i<vec_titulos->cantidad_actual;i++)
    {
        printf("\n%d,%s,%s,STOCK:%d,%c",veccopia[i].id_pelicula,veccopia[i].titulo,veccopia[i].genero,veccopia[i].stock,veccopia[i].estado);
    }

    free(veccopia);
}



int cmp_stock_descendente(const void *a,const void *b)
{
    const t_titulo *ta = (const t_titulo*)a;
    const t_titulo *tb = (const t_titulo*)b;
    if(ta->stock > tb->stock)
    {
        return -1;
    }
    else if(ta->stock < tb->stock)
    {
        return 1;
    }
    else //stock_a == stock_b
    {
        return 0;
    }
}

///----------------------------------------------------------------------------------------------

///8.Listar los títulos que nunca fueron alquilados

void listar_titulos_nunca_alquilados(const t_vector *vec_titulos,const t_vector *vec_alquileres)
{
    int i,pos,no_encontrados=0;
    t_titulo *titulo = (t_titulo*)vec_titulos->vdatos;
    t_alquiler *alquiler =(t_alquiler*)vec_alquileres->vdatos;

    printf("\nLISTADO DE TITULOS QUE NUNCA FUERON ALQUILADOS:");
    printf("\nID de pelicula,NOMBRE de pelicula");

    for(i=0;i<vec_titulos->cantidad_actual;i++)
    {
        pos = buscar_titulo_en_alquileres(vec_alquileres->cantidad_actual,titulo[i].id_pelicula,alquiler);
        if(pos == -1)
        {
            no_encontrados++;
            printf("\n%d,%s",titulo[i].id_pelicula,titulo[i].titulo);
        }
    }

    if(no_encontrados == 0)
    {
        printf("\nTodos los titulos tienen al menos 1 alquiler...");
    }
}

int buscar_titulo_en_alquileres(int ce,int id_pelicula,t_alquiler *alquiler)
{
    int i=0,pos = -1;

    while(i<ce && pos == -1)
    {
        if(alquiler[i].id_pelicula == id_pelicula)
        {
            pos = i;
        }
        else
        {
            i++;
        }
    }

    return pos;
}


///----------------------------------------------------------------------------------------------

int cmp_titulo(const void *valor1, const void *valor2)
{
    const t_titulo *v1 = (const t_titulo *)valor1;
    const t_titulo *v2 = (const t_titulo *)valor2;

    return strcmp(v1->titulo, v2->titulo);
}


