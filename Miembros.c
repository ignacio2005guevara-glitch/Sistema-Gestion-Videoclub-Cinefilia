#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Miembros.h"

void cargar_miembros(const char *path, void *vec_miembros, t_indice *indice, void *vec_auditoria, t_fecha fecha_proceso)
{
    const char *Planes[]  = {"BASIC", "PREMIUM", "VIP", "FAMILY"};
    const char Sexos[]    = {'F', 'M', 'O'};
    const char Estados[] = {'A','B'};
    int campos_leidos;

    FILE* pfArchivo = fopen(path, "rt");
    t_miembro registro;
    t_reg_indice reg_indice;
    unsigned nro_reg = 0;
    char linea[MAX_CARACTERES_MIEMBRO_CSV];

    if(!pfArchivo)
    {
        printf("Error al abrir archivo %s", path);
        return;
    }

    while(fgets(linea, sizeof(linea), pfArchivo))
    {

        // Limpiar registro
        memset(&registro, 0, sizeof(t_miembro));
        campos_leidos = sscanf(linea, "%ld;%[^;];%[^;];%d/%d/%d;%c;%d/%d/%d;%[^;];%d/%d/%d;%c;%[^;];%[^;\n]",
                                &registro.dni,
                                registro.cuil,
                                registro.nombre,
                                &registro.fecha_nacimiento.dia,
                                &registro.fecha_nacimiento.mes,
                                &registro.fecha_nacimiento.anio,
                                &registro.sexo,
                                &registro.fecha_afiliacion.dia,
                                &registro.fecha_afiliacion.mes,
                                &registro.fecha_afiliacion.anio,
                                registro.categoria,
                                &registro.fecha_ultima_cuota.dia,
                                &registro.fecha_ultima_cuota.mes,
                                &registro.fecha_ultima_cuota.anio,
                                &registro.estado,
                                registro.plan,
                                registro.email_tutor);


        // Si no leyó los 17 campos esperados, el registro está incompleto
        if(campos_leidos < CANT_CAMPOS_MIEMBRO)
        {
            continue;
        }
        // Validar DNI
        if(!validar_DNI(&registro.dni))
        {
            registrar_error_miembro(vec_auditoria, "DNI", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar CUIL
        if(!validar_CUIL(&registro.dni, &registro.sexo, registro.cuil))
        {
            registrar_error_miembro(vec_auditoria, "CUIL", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Nombre
        if(!validar_nombre(registro.nombre))
        {
            registrar_error_miembro(vec_auditoria, "NOMBRE", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Fecha de nacimiento
        t_fecha fecha_limite; // fecha del registro - 10 años
        fecha_limite.dia = fecha_proceso.dia;
        fecha_limite.mes = fecha_proceso.mes;
        fecha_limite.anio = fecha_proceso.anio - 10;

        if(!es_fecha_valida(registro.fecha_nacimiento) || cmp_fecha(&registro.fecha_nacimiento, &fecha_limite) >= 0)
        {
            registrar_error_miembro(vec_auditoria, "FEC NACIMIENTO", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Sexo
        if(!validar_caracter(registro.sexo, Sexos, CANT_SEXOS))
        {
            registrar_error_miembro(vec_auditoria, "SEXO", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Fecha de afiliacion
        if(!es_fecha_valida(registro.fecha_afiliacion) || cmp_fecha(&registro.fecha_afiliacion, &registro.fecha_nacimiento) < 0 || cmp_fecha(&registro.fecha_afiliacion, &fecha_proceso) > 0 )
        {
            registrar_error_miembro(vec_auditoria, "FEC AFILIACION", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Categoria
        if(!validar_categoria(registro.categoria, &registro, &fecha_proceso))
        {
            registrar_error_miembro(vec_auditoria, "CATEGORIA", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Fecha ultima cuota paga
        if(!es_fecha_valida(registro.fecha_ultima_cuota) || cmp_fecha(&registro.fecha_ultima_cuota, &registro.fecha_afiliacion) < 0 || cmp_fecha(&registro.fecha_ultima_cuota, &fecha_proceso) > 0)
        {
            registrar_error_miembro(vec_auditoria, "FEC ULT CUOTA PAGA", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Estado
        if(!validar_caracter(registro.estado, Estados, CANT_ESTADOS))
        {
            registrar_error_miembro(vec_auditoria, "ESTADO", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Plan
        if(!validar_plan_genero(registro.plan, Planes, CANT_PLANES_GENEROS))
        {
            registrar_error_miembro(vec_auditoria, "PLAN", &registro.dni, &fecha_proceso);
            continue;
        }

        // Validar Email Tutor
        if(registro.email_tutor[0] != '\0' && !validar_email(registro.email_tutor))
        {
            registrar_error_miembro(vec_auditoria, "EMAIL TUTOR", &registro.dni, &fecha_proceso);
            continue;
        }

        // Normalizar nombre
        char nombre_normalizado[MAX_NOMBRE];
        normalizar(registro.nombre, nombre_normalizado, 1);
        strcpy(registro.nombre, nombre_normalizado);


        // Revisar si la clave no existe en el indice. Armar reg_indice con la clave a buscar
        reg_indice.dni     = registro.dni;
        reg_indice.nro_reg = 0;

        if(indice_buscar(indice, &reg_indice, indice->cantidad_elementos_actual, sizeof(t_reg_indice_miembro), cmp_DNI) == NO_EXISTE)
        {
            // Nueva clave -> escribir en archivo e insertar en indice
            vector_agregar(vec_miembros, &registro, sizeof(t_miembro));
            reg_indice.nro_reg = nro_reg;
            indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_miembro), cmp_DNI);
            nro_reg++;
        }
    }

    fclose(pfArchivo);
}

int  alta_miembro(t_indice *indice, t_vector *vec, t_fecha fecha_proceso)
{
    t_miembro nuevo_miembro;
    t_reg_indice_miembro reg_indice;
    char buffer[MAX_CARACTERES_MIEMBRO_CSV];
    const char *Planes[] = {"BASIC", "PREMIUM", "VIP", "FAMILY"};
    const char  Sexos[]  = {'F', 'M', 'O'};

    printf("Ingrese DNI: ");
    scanf("%ld", &nuevo_miembro.dni);

    printf("Ingrese CUIL: ");
    fflush(stdin);
    fgets(nuevo_miembro.cuil, MAX_CUIL, stdin);
    nuevo_miembro.cuil[strcspn(nuevo_miembro.cuil, "\n")] = '\0';

    printf("Ingrese Apellido y Nombre (Apellido, Nombre): ");
    fflush(stdin);
    fgets(nuevo_miembro.nombre, MAX_NOMBRE, stdin);
    nuevo_miembro.nombre[strcspn(nuevo_miembro.nombre, "\n")] = '\0';

    printf("Ingrese Fecha de Nacimiento (dd/mm/aaaa): ");
    fflush(stdin);
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d/%d/%d", &nuevo_miembro.fecha_nacimiento.dia,
                               &nuevo_miembro.fecha_nacimiento.mes,
                               &nuevo_miembro.fecha_nacimiento.anio);

    printf("Ingrese Sexo (F/M/O): ");
    fflush(stdin);
    scanf("%c",&nuevo_miembro.sexo);
    nuevo_miembro.sexo = MI_TOUPPER(nuevo_miembro.sexo);

    printf("Ingrese Fecha de Afiliacion (dd/mm/aaaa): ");
    fflush(stdin);
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d/%d/%d", &nuevo_miembro.fecha_afiliacion.dia,
                               &nuevo_miembro.fecha_afiliacion.mes,
                               &nuevo_miembro.fecha_afiliacion.anio);

    printf("Ingrese Categoria (MENOR, ADULTO): ");
    fflush(stdin);
    fgets(nuevo_miembro.categoria, MAX_CATEGORIA, stdin);
    nuevo_miembro.categoria[strcspn(nuevo_miembro.categoria, "\n")] = '\0';
    str_to_upper(nuevo_miembro.categoria);

    printf("Ingrese Fecha Ultima Cuota Paga (dd/mm/aaaa): ");
    fflush(stdin);
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d/%d/%d", &nuevo_miembro.fecha_ultima_cuota.dia,
                               &nuevo_miembro.fecha_ultima_cuota.mes,
                               &nuevo_miembro.fecha_ultima_cuota.anio);

    printf("Ingrese el Plan (BASIC, PREMIUM, VIP, FAMILY): ");
    fflush(stdin);
    fgets(nuevo_miembro.plan, MAX_PLAN, stdin);
    nuevo_miembro.plan[strcspn(nuevo_miembro.plan, "\n")] = '\0';
    str_to_upper(nuevo_miembro.plan);

    if(strcmp(nuevo_miembro.categoria,"MENOR") == 0)
    {
        printf("Ingrese el mail del tutor:");
        fflush(stdin);
        fgets(nuevo_miembro.email_tutor,MAX_EMAIL,stdin);
        nuevo_miembro.email_tutor[strcspn(nuevo_miembro.email_tutor,"\n")] = '\0';

    }

    // Validar DNI
    if(!validar_DNI(&nuevo_miembro.dni))
    {
        printf("\nERROR: DNI INVALIDO\n");
        return ERROR;
    }

    // Validar DNI duplicado en el indice
    t_reg_indice_miembro reg_buscar;
    reg_buscar.dni = nuevo_miembro.dni;
    if(indice_buscar(indice, &reg_buscar,
                     indice->cantidad_elementos_actual,
                     sizeof(t_reg_indice_miembro), cmp_DNI) != NO_EXISTE)
    {
        printf("El DNI YA EXISTE EN EL SISTEMA\n");
        return ERROR;
    }

    // Validar CUIL
    if(!validar_CUIL(&nuevo_miembro.dni, &nuevo_miembro.sexo, nuevo_miembro.cuil))
    {
        printf("\nERROR: CUIL INVALIDO\n");
        return ERROR;
    }

    // Validar Nombre
    if(!validar_nombre(nuevo_miembro.nombre))
    {
        printf("\nERROR: NOMBRE INVALIDO\n");
        return ERROR;
    }
    // Validar Fecha de nacimiento
    t_fecha fecha_limite; // fecha del registro - 10 años
    fecha_limite.dia = fecha_proceso.dia;
    fecha_limite.mes = fecha_proceso.mes;
    fecha_limite.anio = fecha_proceso.anio - 10;

    if(!es_fecha_valida(nuevo_miembro.fecha_nacimiento) || cmp_fecha(&nuevo_miembro.fecha_nacimiento, &fecha_limite) >= 0)
    {
        printf("\nERROR: FECHA DE NACIMIENTO INVALIDA\n");
        return ERROR;
    }

    // Validar Sexo
    if(!validar_caracter(nuevo_miembro.sexo, Sexos, CANT_SEXOS))
    {
        printf("\nERROR: SEXO INVALIDO\n");
        return ERROR;
    }

    // Validar Fecha de afiliacion
    if(!es_fecha_valida(nuevo_miembro.fecha_afiliacion) || cmp_fecha(&nuevo_miembro.fecha_afiliacion, &nuevo_miembro.fecha_nacimiento) < 0 || cmp_fecha(&nuevo_miembro.fecha_afiliacion, &fecha_proceso) > 0 )
    {
        printf("\nERROR: FECHA DE AFILIACION INVALIDA\n");
        return ERROR;
    }

    // Validar Categoria
    if(!validar_categoria(nuevo_miembro.categoria, &nuevo_miembro, &fecha_proceso))
    {
        printf("\nERROR: CATEGORIA INVALIDA\n");
        return ERROR;
    }

    // Validar Fecha ultima cuota paga
    if(!es_fecha_valida(nuevo_miembro.fecha_afiliacion) || cmp_fecha(&nuevo_miembro.fecha_ultima_cuota, &nuevo_miembro.fecha_afiliacion) < 0 || cmp_fecha(&nuevo_miembro.fecha_ultima_cuota, &fecha_proceso) > 0)
    {
        printf("\nERROR: FECHA DE ULTIMA CUOTA PAGA INVALIDA\n");
        return ERROR;
    }

    // Validar Plan
    if(!validar_plan_genero(nuevo_miembro.plan, Planes, CANT_PLANES_GENEROS))
    {
        printf("\nERROR: PLAN INVALIDO\n");
        return ERROR;
    }

    // Validar Email Tutor
    if(nuevo_miembro.email_tutor[0] != '\0' && !validar_email(nuevo_miembro.email_tutor))
    {
        printf("\nERROR: MAIL INVALIDO\n");
        return ERROR;
    }

    // Normalizar Nombre
    char nombre_normalizado[MAX_NOMBRE];
    normalizar(nuevo_miembro.nombre, nombre_normalizado, 1);
    strcpy(nuevo_miembro.nombre, nombre_normalizado);

    // Estado = 'A'
    nuevo_miembro.estado = 'A';

    // Revisar si la clave no existe en el indice. Armar reg_indice con la clave a buscar
    reg_indice.dni     = nuevo_miembro.dni;
    reg_indice.nro_reg = vec->cantidad_actual;

    if(indice_buscar(indice, &reg_indice, indice->cantidad_elementos_actual, sizeof(t_reg_indice_miembro), cmp_DNI) == NO_EXISTE)
    {
        // Nueva clave -> escribir en archivo e insertar en indice
        vector_agregar(vec, &nuevo_miembro, sizeof(t_miembro));
        indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_miembro), cmp_DNI);
        printf("\nMIEMBRO DADO DE ALTA EXITOSAMENTE\n");
    }
    else
    {
        // Registro duplicado
        printf("\nERROR: DNI duplicado. Ya existe un miembro con el DNI ingresado.\n");
    }

    return OK;
}

int  baja_miembro(t_indice *indice,t_vector *vec)
{
    char validacion;
    int pos_indice;
    t_reg_indice registro_a_buscar;
    t_miembro miembro;
    const t_reg_indice_miembro *indice_lectura = (const t_reg_indice_miembro*) indice->vindice;
    unsigned nro_reg;
    do
    {
        printf("\n\nIngrese el DNI del miembro que desea dar de baja:");
        scanf("%ld",&registro_a_buscar.dni);
        if(registro_a_buscar.dni>100000000 || registro_a_buscar.dni<1000000)
            printf("DNI no valido, ingrese nuevamente\n");
    }while(registro_a_buscar.dni>100000000 || registro_a_buscar.dni<1000000)
    ;
    pos_indice = indice_buscar(indice,&registro_a_buscar,indice->cantidad_elementos_actual,sizeof(t_reg_indice),cmp_DNI);
    if(pos_indice==-1)
    {
        printf("\n\nEl DNI no se encuentra en la base de datos\n");
        return  0;
    }
    else
    {
        nro_reg=indice_lectura[pos_indice].nro_reg;
        void* pos = vec->vdatos + nro_reg * sizeof(t_miembro);
        memcpy(&miembro,pos,sizeof(t_miembro));
        printf("\nDESEA CONFIRMAR LA BAJA DEL USUARIO %s (Y/N): ",miembro.nombre);
        fflush(stdin);
        scanf("%c",&validacion);
        validacion = MI_TOUPPER (validacion);
        if(validacion == 'Y')
        {

            miembro.estado='B';
            memcpy(pos,&miembro,sizeof(t_miembro));
            indice_eliminar(indice,&registro_a_buscar,sizeof(t_reg_indice),cmp_DNI);
        }
        else
            printf("\nBAJA CANCELADA\n");
    }

    return OK;
}

int  modificar_miembro(t_indice *indice, t_vector *vec, t_fecha fechaProceso)
{
   const char *Planes[]  = {"BASIC", "PREMIUM", "VIP", "FAMILY"};
    const char Sexos[]    = {'F', 'M', 'O'};

    long dni_buscar;
    t_reg_indice reg_busqueda;
    int posicion_indice;


    printf("Ingrese el DNI del miembro a modificar: ");
    scanf("%ld", &dni_buscar);

    reg_busqueda.dni = dni_buscar;
    posicion_indice = indice_buscar(indice, &reg_busqueda, indice->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_DNI );

    if (posicion_indice == NO_EXISTE)
        {
        printf("Error: El DNI ingresado no pertenece a un miembro activo.\n");
        return ERROR;
        }

    //  Sacamos el vector del índice para obtener el nro_reg
    t_reg_indice *vectorIndice = (t_reg_indice*)indice->vindice;
    unsigned pos_vector_miembros = vectorIndice[posicion_indice].nro_reg;

    t_miembro *miembro = (t_miembro*)vec->vdatos+pos_vector_miembros;

    long dni_viejo = miembro->dni;
    long dni_nuevo = dni_viejo;
    int cambio_dni = 0;

    int opcion;
    printf("\n--- CAMPOS MODIFICABLES DE MIEMBRO ---\n");
    printf("1. DNI\n");
    printf("2. Apellidos y Nombres\n");
    printf("3. Fecha de Nacimiento\n");
    printf("4. Sexo\n");
    printf("5. Fecha de Afiliacion\n");
    printf("6. Plan\n");
    printf("7. Email del Tutor\n");
    printf("8. Fecha de ultima Cuota Paga\n");
    printf("Elija el numero del campo que desea modificar: ");
    scanf("%d", &opcion);

    switch(opcion) {
        case 1: {
            printf("Ingrese el NUEVO DNI: ");
            scanf("%ld", &dni_nuevo);


            if (dni_nuevo <= 1000000 || dni_nuevo >= 100000000)
                {
                printf("Error: DNI fuera de rango\n");
                return ERROR;
                }

            // Verificar duplicados
            t_reg_indice reg_duplicado;
            reg_duplicado.dni = dni_nuevo;
            if (indice_buscar(indice, &reg_duplicado, indice->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_DNI) != NO_EXISTE)
                {
                printf("Error: El DNI nuevo ya esta registrado.\n");
                return ERROR;
                }
            printf("%s\n",miembro->nombre);
            printf("%ld\n",miembro->dni);
            printf("%c\n",miembro->sexo);

            miembro->dni = dni_nuevo;
            cambio_dni = 1; // Flag para actualizar el índice al final

            calcular_CUIL(&miembro->dni, &miembro->sexo, miembro->cuil); //como cambio el dni tambien cambia el cuil
            printf("CUIL recalculado automaticamente: %s\n", miembro->cuil);
            break;
        }

        case 2: { // Modificar Nombre y Apellido
            char nuevo_nombre[60];
            printf("Ingrese el nuevo Apellido y Nombre: ");
            fflush(stdin);
            fgets(nuevo_nombre, 60, stdin);

            // normalizamos
            char nombre_normalizado[60];
            normalizar(nuevo_nombre, nombre_normalizado,1);
            strcpy(miembro->nombre, nombre_normalizado);
            break;
        }

        case 3: { // Modificar Fecha de Nacimiento
            t_fecha nueva_fecha;
            printf("Ingrese nueva Fecha de Nacimiento (DD MM AAAA, sin barras): ");
            scanf("%d %d %d", &nueva_fecha.dia, &nueva_fecha.mes, &nueva_fecha.anio);

            if(!es_fecha_valida(nueva_fecha) || cmp_fecha(&nueva_fecha,&fechaProceso) >= 0) //Validamos la fecha
            {
                printf("Error en carga de fecha");
                return ERROR;
            }

            miembro->fecha_nacimiento = nueva_fecha;

            validar_categoria(miembro->categoria, miembro,&fechaProceso); //  Si cambio la edad, puede cambiar la categoría

            break;
        }

        case 4: { // Modificar Sexo
            char nuevo_sexo;
            printf("Ingrese nuevo Sexo (F / M / O): ");
            fflush(stdin);
            scanf("%c", &nuevo_sexo);

           if(!validar_caracter(nuevo_sexo, Sexos, CANT_SEXOS))
            {
                printf("Error: Sexo no valido.\n");
                return ERROR;
            }

            miembro->sexo = nuevo_sexo;

            calcular_CUIL(&miembro->dni, &miembro->sexo, miembro->cuil); //como cambio el sexo, cambia el cuil
            printf("CUIL recalculado por cambio de sexo: %s\n", miembro->cuil);
            break;
        }

        case 5: { // Modificar Fecha de Afiliación
            t_fecha nueva_afiliacion;
            printf("Ingrese nueva Fecha de Afiliacion (DD MM AAAA): ");
            scanf("%d %d %d", &nueva_afiliacion.dia, &nueva_afiliacion.mes, &nueva_afiliacion.anio);

            if(!es_fecha_valida(nueva_afiliacion) || cmp_fecha(&nueva_afiliacion, &miembro->fecha_nacimiento) < 0 || cmp_fecha(&nueva_afiliacion, &fechaProceso) > 0 )
            {
                printf("\nERROR: FECHA DE AFILIACION INVALIDA\n");
                return ERROR;
            }
            miembro->fecha_afiliacion = nueva_afiliacion;
            break;
        }

        case 6: { // Modificar Plan
            char plan_nuevo[8];
            printf("Escriba el nuevo Plan:\n- BASIC\n- PREMIUM\n- VIP\n- FAMILY\nPlan: ");
            fflush(stdin);
            fgets(plan_nuevo, 8, stdin);

            if(!validar_plan_genero(plan_nuevo, Planes, CANT_PLANES_GENEROS))
                {
                printf("Opcion invalida.\n");
                return ERROR;
                }
            strcpy(miembro->plan,plan_nuevo);

            break;
        }

        case 7: { // Modificar Email Tutor
            // Solo tiene sentido o es obligatorio si es MENOR
            char nuevo_email[30];
            printf("Ingrese el Email del Tutor: ");
            fflush(stdin);
            fgets(nuevo_email, 30, stdin);

            if (validar_email(nuevo_email) == ERROR)
            {
                printf("Error: Formato de Email invalido.\n");
                return ERROR;
            }

            if (strcmp(miembro->categoria, "MENOR") == 0 && strlen(nuevo_email) == 0)
            {
                printf("Error: Al ser MENOR, el mail del tutor no puede estar vacio.\n");
                return ERROR;
            }

            strcpy(miembro->email_tutor, nuevo_email);
            break;
        }

        case 8: { // Modificar fecha de ultima cuota paga
            t_fecha nueva_cuota;
            printf("Ingrese Fecha de ultima Cuota (DD MM AAAA): ");
            scanf("%d %d %d", &nueva_cuota.dia, &nueva_cuota.mes, &nueva_cuota.anio);

            if(!es_fecha_valida(nueva_cuota) || cmp_fecha(&nueva_cuota,& miembro->fecha_afiliacion) < 0 || cmp_fecha(&nueva_cuota, &fechaProceso) > 0)
            {
                printf("\nERROR: FECHA DE ULTIMA CUOTA PAGA INVALIDA\n");
                return ERROR;
            }
            miembro->fecha_ultima_cuota = nueva_cuota;
            break;
        }


        default:
            printf("Opcion invalida. Volviendo al menu.\n");
            return ERROR;
    }
    // Si se modifico el dni actualizamo el indice
    if (cambio_dni==1)
        {
        t_reg_indice reg_viejo;
        reg_viejo.dni = dni_viejo;
        reg_viejo.nro_reg = pos_vector_miembros;

        indice_eliminar(indice, &reg_viejo, sizeof(t_reg_indice), cmp_DNI);

        t_reg_indice reg_nuevo;
        reg_nuevo.dni = dni_nuevo;
        reg_nuevo.nro_reg = pos_vector_miembros;

        indice_insertar(indice, &reg_nuevo, sizeof(t_reg_indice), cmp_DNI);
       }

    printf("MIEMBRO E INDICES MODIFICADOS CON EXITO\n");


    return OK;
}

void listar_miembros_por_DNI (const t_indice *indice,const t_vector *vec)
{
    int i;
    t_reg_indice_miembro *reg;
    t_miembro *miembro;

    mostrar_encabezado_miembros();

    for(i=0;i< indice->cantidad_elementos_actual;i++)
    {
        reg = (t_reg_indice_miembro*)indice->vindice + i; // Obtengo el registro del indice

        miembro = (t_miembro*)vec->vdatos + reg->nro_reg; // Con nro_reg accedo al miembro completo en el vector
        if(miembro->estado == 'A') // Solo muestro los activos
        {
            mostrar_miembro(miembro);
        }
    }
}

void listar_miembros_por_plan(const t_indice *indice,const t_vector *vec)
{
    int i;
    t_miembro *veccopia = malloc(vec->cantidad_actual * sizeof(t_miembro));
    memcpy(veccopia, vec->vdatos, vec->cantidad_actual * sizeof(t_miembro));

    qsort(veccopia, vec->cantidad_actual, sizeof(t_miembro), cmp_nombre);

    printf("\n%-30s%-12s%-12s%-12s%-12s\n", "Plan / Indice", "(BASIC)", "(PREMIUM)", "(VIP)", "(FAMILY)");
    for(i=0;i< vec->cantidad_actual; i++)
    {
        if(veccopia[i].estado == 'A')
        {
            printf("%-30s", veccopia[i].nombre);

            if(strcmp(veccopia[i].plan, "BASIC") == 0)
                printf("%-12ld", veccopia[i].dni);
            else
                printf("%-12d", 0);

            if(strcmp(veccopia[i].plan, "PREMIUM") == 0)
                printf("%-12ld", veccopia[i].dni);
            else
                printf("%-12d", 0);

            if(strcmp(veccopia[i].plan, "VIP") == 0)
                printf("%-12ld", veccopia[i].dni);
            else
                printf("%-12d", 0);

            if(strcmp(veccopia[i].plan, "FAMILY") == 0)
                printf("%-12ld", veccopia[i].dni);
            else
                printf("%-12d", 0);

            printf("\n");
        }
    }

    free(veccopia);
}

void persistir_miembros(const t_vector *vec, t_fecha fecha_proceso)
{
    char path[100];
    sprintf(path, "Archivos\\miembros_%04d%02d%02d.dat", fecha_proceso.anio, fecha_proceso.mes, fecha_proceso.dia);

    FILE *pf = fopen(path, "wb");
    if (!pf)
    {
        printf("Error al crear archivo de persistencia de miembros: %s\n", path);
        return;
    }

    fwrite(vec->vdatos, sizeof(t_miembro), vec->cantidad_actual, pf);
    fclose(pf);
    printf("Miembros persistidos en: %s\n", path);
}

int cargar_miembros_bin(const char *path, t_vector *vec_miembros, t_indice *indice)
{
    FILE *pf = fopen(path, "rb");
    if (!pf)
    {
        return ERROR;
    }

    t_miembro m;
    t_reg_indice_miembro reg_indice;
    unsigned nro_reg = 0;

    while (fread(&m, sizeof(t_miembro), 1, pf) == 1)
    {
        vector_agregar(vec_miembros, &m, sizeof(t_miembro));

        reg_indice.dni = m.dni;
        reg_indice.nro_reg = nro_reg;

        if (m.estado == 'A')
        {
            indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_miembro), cmp_DNI);
        }
        nro_reg++;
    }

    fclose(pf);
    return OK;
}

void mostrar_miembro(const void *elem)
{
    const t_miembro *m = (const t_miembro *)elem;
    printf("%-9ld | %-13.13s | %-60.60s | %02d/%02d/%04d | %c | %02d/%02d/%04d | %-10.10s | %02d/%02d/%04d | %c | %-10.10s | %-30.30s\n",
           m->dni,
           m->cuil,
           m->nombre,
           m->fecha_nacimiento.dia, m->fecha_nacimiento.mes, m->fecha_nacimiento.anio,
           m->sexo,
           m->fecha_afiliacion.dia, m->fecha_afiliacion.mes, m->fecha_afiliacion.anio,
           m->categoria,
           m->fecha_ultima_cuota.dia, m->fecha_ultima_cuota.mes, m->fecha_ultima_cuota.anio,
           m->estado,
           m->plan,
           (m->email_tutor[0] != '\0') ? m->email_tutor : "N/A");
}

void mostrar_miembro_por_dni(const t_indice *indice, const t_vector *vec)
{
    t_reg_indice_miembro clave;
    t_miembro miembro;
    int pos;
    long dni;

    printf("Ingrese DNI del miembro: ");
    if(scanf("%ld", &dni) != 1)
    {
        printf("DNI invalido.\n");
        return;
    }

    clave.dni     = dni;
    clave.nro_reg = 0;

    pos = indice_buscar(indice, &clave, indice->cantidad_elementos_actual, sizeof(t_reg_indice_miembro), cmp_DNI);
    if(pos == NO_EXISTE)
    {
        printf("ERROR: No existe un miembro con DNI %ld.\n", dni);
        return;
    }

    t_reg_indice *vectorIndice = (t_reg_indice*)indice->vindice;
    unsigned pos_vector_miembros = vectorIndice[pos].nro_reg;
    memcpy(&miembro, vec->vdatos + pos_vector_miembros * sizeof(t_miembro), sizeof(t_miembro));
    mostrar_encabezado_miembros();
    mostrar_miembro(&miembro);
}

void mostrar_reg_indice_miembro(const void *elem)
{
    const t_reg_indice_miembro *r = (const t_reg_indice_miembro *)elem;
    printf("nro_reg: %2u | dni: %ld\n", r->nro_reg, r->dni);
}

void mostrar_encabezado_miembros(void)
{
    printf("\n=========================================================================================================================================================================================\n");
    printf("%-9s | %-13s | %-60s | %-10s | %s | %-10s | %-10s | %-10s | %s | %-10s | %-30s\n",
           "DNI", "CUIL", "APELLIDO Y NOMBRE", "F. NAC", "S", "F. AFIL", "CATEGORIA", "U. PAGO", "E", "PLAN", "EMAIL TUTOR");
    printf("=========================================================================================================================================================================================\n");

}

int cmp_DNI(const void *valor1, const void *valor2)
{
    const t_reg_indice_miembro *v1 = (const t_reg_indice_miembro*) valor1;
    const t_reg_indice_miembro *v2 = (const t_reg_indice_miembro*) valor2;

    if(v1->dni < v2->dni)
    {
        return NO_EXISTE;
    }
    else if(v1->dni > v2->dni)
    {
        return OK;
    }
    else
    {
        return ERROR;
    }
}

int cmp_nombre(const void *a, const void *b)
{
    t_miembro *ma = (t_miembro*) a;
    t_miembro *mb = (t_miembro*) b;

    return strcmp(ma->nombre, mb->nombre);
}


///2.Listar miembros con estado B (dados de baja), ordenados por DNI

void listar_miembros_dados_baja_por_DNI(const t_vector *vec_miembros)
{
    int i,encontrados=0;
    t_miembro *veccopia = malloc(vec_miembros->cantidad_actual * sizeof(t_miembro));
    memcpy(veccopia,vec_miembros->vdatos,vec_miembros->cantidad_actual * sizeof(t_miembro));

    qsort(veccopia,vec_miembros->cantidad_actual,sizeof(t_miembro),cmp_miembro_dni);

    printf("\nMIEMBROS dados de baja(Estado = 'B') ORDENADO por DNI:");
    for(i=0;i<vec_miembros->cantidad_actual;i++)
    {
        if(veccopia[i].estado == 'B')
        {
            encontrados ++;
            printf("\n%ld\t%s",veccopia[i].dni,veccopia[i].nombre);
        }
    }
    if(encontrados==0)
    {
        printf("\nNo se encontro miembros dados de baja...");
    }
    free(veccopia);
}
    ///funcion cmp nueva para el 2. Se necesita porque la anterior comparaba por indice pero cuando estado == 'B' se elimina del indice aunque queda en el vector original.
int cmp_miembro_dni(const void *a,const void *b)
{
    const t_miembro *ma = (const t_miembro*)a;
    const t_miembro *mb = (const t_miembro*)b;
    if(ma->dni < mb->dni)
    {
        return NO_EXISTE;
    }
    else if(ma->dni > mb->dni)
    {
        return OK;
    }
    else
    {
        return ERROR;
    }
}
///------------------------------------------------------------------

///3.Mostrar cantidad de miembros por categoría (MENOR / ADULTO)

void mostrar_cantidad_miembros_por_categoria(const t_vector *vec_miembros)
{
    int i,cant_menores=0,cant_adultos=0;
    t_miembro *miembro = (t_miembro*)vec_miembros->vdatos;

    for(i=0;i<vec_miembros->cantidad_actual;i++)
    {
        if(strcmp(miembro[i].categoria,"MENOR") == 0)
        {
            cant_menores++;
        }
        else
        {
            if(strcmp(miembro[i].categoria,"ADULTO") == 0)
            {
                cant_adultos++;
            }
        }
    }
    printf("\nCATEGORIA-CANTIDAD DE MIEMBROS:");
    printf("\nMENORES:%d",cant_menores);
    printf("\nADULTOS:%d",cant_adultos);
}

///------------------------------------------------------------------

///5.Listar los miembros que tienen al menos un alquiler registrado, ordenados por apellido y nombre

void listar_miembros_con_almenos_un_alquiler_ordenadoxApellidoyNombre(const t_vector *vec_miembros,const t_vector *vec_alquileres)
{
    int i,pos;
    t_alquiler *alquileres = (t_alquiler*)vec_alquileres->vdatos;
    t_miembro *veccopia = malloc(vec_miembros->cantidad_actual * sizeof(t_miembro));

    memcpy(veccopia,vec_miembros->vdatos,vec_miembros->cantidad_actual*sizeof(t_miembro));
    qsort(veccopia,vec_miembros->cantidad_actual,sizeof(t_miembro),cmp_nombre);

    for(i=0;i<vec_miembros->cantidad_actual;i++)
    {
        pos = buscar_miembro_en_alquileres(vec_alquileres->cantidad_actual,veccopia[i].dni,alquileres);
        if(pos != -1)
        {
            printf("\nApellido y nombre:%s-Dni:%ld",veccopia[i].nombre,veccopia[i].dni);
        }
    }

    free(veccopia);
}


int buscar_miembro_en_alquileres(int ce,long dni,t_alquiler *alquileres)
{
    int i=0,pos=-1;

    while(pos==-1 && i<ce)
    {
        if(dni==alquileres[i].dni)
            pos=i;
        i++;
    }

    return pos;

}

///------------------------------------------------------------------

///7.Dado un plan ingresado por teclado, mostrar cuántos miembros activos tiene ese plan

void listar_miembros_activos_por_PlanIngresado(const t_vector *vec_miembros)
{
    int i,cant_miembros_PlanIngresado=0;
    char PlanIngresado[MAX_PLAN];
    const char *Planes[]={"BASIC","PREMIUM","VIP","FAMILY"};

    t_miembro *miembro = (t_miembro*)vec_miembros->vdatos;

    while(getchar() != '\n'); //fflush(stdin);

    do{
        printf("\nIngrese el plan por teclado(BASIC-PREMIUM-VIP-FAMILY):");
        fflush(stdin);
        fgets(PlanIngresado,MAX_PLAN,stdin);
        PlanIngresado[strcspn(PlanIngresado, "\n")] = '\0';
        str_to_upper(PlanIngresado);
        if(!validar_plan_genero(PlanIngresado,Planes,CANT_PLANES_GENEROS))
        {
            printf("\nERROR,volver a ingresar...(BASIC-PREMIUM-VIP-FAMILY):");

        }
    }while(!validar_plan_genero(PlanIngresado,Planes,CANT_PLANES_GENEROS));


    for(i=0;i<vec_miembros->cantidad_actual;i++)
    {
        if(strcmp(miembro[i].plan,PlanIngresado) == 0 && miembro[i].estado == 'A')
        {
            cant_miembros_PlanIngresado++;
        }
    }

    printf("\nLA CANTIDAD DE MIEMBROS ACTIVOS POR EL PLAN:%s,ES:%d",PlanIngresado,cant_miembros_PlanIngresado);

}

///------------------------------------------------------------------

///9. Dado un DNI, mostrar todos los titulos que alquilo con su cantidad de veces

void PorIngresoDNI_mostrar_titulos_alquilados(const t_indice *indice_miembros, const t_vector *vec_alquileres, const t_vector *vec_titulos)
{
    int i,pos;
    long int dni_ingresado;
    t_reg_indice_miembro *indice_miembro = (t_reg_indice_miembro*)indice_miembros->vindice;
    t_alquiler *alquiler = (t_alquiler*)vec_alquileres->vdatos;
    t_titulo *titulo = (t_titulo*)vec_titulos->vdatos;

    do{
        printf("\nIngrese el dni:");
        scanf("%ld",&dni_ingresado);
        pos = buscar_miembro_en_indice(indice_miembros->cantidad_elementos_actual,dni_ingresado,indice_miembro);
        if(pos == -1)
        {
            printf("\nDNI no encontrado, volver a ingresar...");
        }
    }while(pos == -1);

    for(i=0;i<vec_alquileres->cantidad_actual;i++)
    {
        if(alquiler[i].dni == dni_ingresado)
        {
            pos = buscar_id_pelicula_en_titulos(vec_titulos->cantidad_actual,alquiler[i].id_pelicula,titulo);
            if(pos != -1)
            {
                printf("\nTitulo de la pelicula:%s",titulo[pos].titulo);
                printf("\nCantidad de alquileres:%d",alquiler[i].cant_alquileres);
                printf("\n---------------------------");
            }
        }
    }
}

int buscar_miembro_en_indice(int ce,long int dni,t_reg_indice_miembro *indice_miembro)
{
    int i=0,pos = -1;
    while(i<ce && pos == -1)
    {
        if(indice_miembro[i].dni == dni)
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

int buscar_id_pelicula_en_titulos(int ce,int id_pelicula,t_titulo *titulo) /// en realidad deberia ir en titulos.h pero pongo todo aca por prolijidad.
{
    int i=0,pos = -1;
    while(i<ce && pos == -1)
    {
        if(titulo[i].id_pelicula == id_pelicula)
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

///------------------------------------------------------------------

///10.Listar los miembros activos cuya última cuota paga supera los 90 días desde la fecha de proceso, ordenados por fecha de última cuota ascendente.

void listar_miembros_con_ultima_cuota_paga_superior_a_90_desde_fecha_proceso(const t_vector *vec_miembros,t_fecha fecha_proceso)
{
    int i;
    t_miembro *veccopia = malloc(vec_miembros->cantidad_actual * sizeof(t_miembro));
    memcpy(veccopia,vec_miembros->vdatos,sizeof(t_miembro)*vec_miembros->cantidad_actual);

    ordenamiento_burbujeo(veccopia,vec_miembros->cantidad_actual,cmp_fecha_ultima_cuota);//ordeno por fecha de ultima cuota ascendente

    for(i=0;i<vec_miembros->cantidad_actual;i++)
    {
        if(veccopia[i].estado == 'A' && (dias_entre_fechas(fecha_proceso,veccopia[i].fecha_ultima_cuota) > 90)) //si el miembro esta activo, y la ultima cuota paga supera los 90 dias desde la fecha de proceso
        {
            mostrar_miembro(&veccopia[i]);
        }
    }

    free(veccopia);
}


int fecha_a_dias_exacto(t_fecha f)
{
    int dias = 0;
    int anio,mes;
    int dias_por_mes[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    t_fecha f_aux;
    f_aux.dia = 1;
    f_aux.mes = 1;

    for(anio = 1;anio < f.anio;anio++)
    {
        f_aux.anio = anio; //se actualiza con cada iteracion
        if(fecha_anio_bisiesto(f_aux))
            dias+= 366;
        else
            dias+= 365;
    }

    for(mes = 1;mes < f.mes;mes++)
    {
        if(mes == 2 && fecha_anio_bisiesto(f))
        {
            dias += 29;
        }
        else
        {
            dias += dias_por_mes[mes];
        }
    }

    dias += f.dia;

    return dias;
}

int dias_entre_fechas(t_fecha f1,t_fecha f2)
{
    return fecha_a_dias_exacto(f1) - fecha_a_dias_exacto(f2);
}


int cmp_fecha_ultima_cuota(const t_miembro *a,const t_miembro *b) //ascendente
{
    return cmp_fecha(&a->fecha_ultima_cuota,&b->fecha_ultima_cuota);
}

void ordenamiento_burbujeo(t_miembro *vec,int ce,int(*cmp)(const t_miembro*,const t_miembro*))
{
    int i,j;
    t_miembro aux;

    for(i=0;i<ce-1;i++)
    {
        for(j=0;j < ce-i-1;j++)
        {
            if(cmp(&vec[j],&vec[j+1])>0)
            {
                aux = vec[j];
                vec[j] = vec[j+1];
                vec[j+1] = aux;
            }
        }
    }
}


///------------------------------------------------------------------

///11.Dado un DNI ingresado por teclado, mostrar si existe o no en el índice y su posición

void PorIngresoDNI_mostrar_si_existe_en_indice_miembros(t_indice *indice_miembros)
{
    int pos;
    long int dni_ingresado;
    t_reg_indice_miembro clave;


    printf("\nIngrese el dni a verificar si existe o no en el indice_miembros:");
    scanf("%ld",&dni_ingresado);

    clave.dni = dni_ingresado;

    pos = indice_buscar(indice_miembros,&clave,indice_miembros->cantidad_elementos_actual,sizeof(t_reg_indice_miembro),cmp_DNI);
    if(pos != -1)
    {
        printf("\nEl dni %ld se encuentra en el indice, en la posicion %d",dni_ingresado,pos);
    }
    else
    {
        printf("\nEl dni %ld NO se encentra en el indice...",dni_ingresado);
    }

}

///12.Mostrar todos los DNIs del indice en orden

void mostrar_dni_del_indice_en_orden(t_indice *indice_miembros)
{
    int i;
    t_reg_indice_miembro *miembro_indice = (t_reg_indice_miembro*)indice_miembros->vindice;

    for(i=0;i<indice_miembros->cantidad_elementos_actual;i++)
    {
        printf("\nDNI:%ld,nro reg:%d",miembro_indice[i].dni,miembro_indice[i].nro_reg);
    }
}

///13.Dado un DNI, mostrar todos los datos completos del miembro usando el indice

int PorIngresoDNI_mostrar_datos_completos_miembro_con_indice(t_indice *indice_miembros,t_vector *vec_miembros)
{
    t_reg_indice_miembro *indice_de_miembro = (t_reg_indice_miembro*)indice_miembros->vindice;
    t_reg_indice_miembro clave; //necesito esto para poder buscar en el indice

    t_miembro *miembro =(t_miembro*)vec_miembros->vdatos;
    int pos,nro_reg;
    long int dni_ingresado;

    printf("\nIngrese el dni del miembro:");
    scanf("%ld",&dni_ingresado);
    clave.dni = dni_ingresado; //igualo la estructura clave en el campo dni al dni_ingresado y le mando la direccion.

    pos = indice_buscar(indice_miembros,&clave,indice_miembros->cantidad_elementos_actual,sizeof(t_reg_indice_miembro),cmp_DNI);
    if(pos == -1)
    {
        printf("\nDNI no encontrado en el indice de miembros...");
        return ERROR;
    }

    nro_reg = indice_de_miembro[pos].nro_reg;

    mostrar_miembro(&miembro[nro_reg]);

    return OK;

}

///------------------------------------------------------------------

///14.Contar cuantos miembros activos hay en el indice

void Contar_miembros_activos_con_indice(t_indice *indice_miembros)
{
    printf("\nLa cantidad de miembros activos que hay en el indice es:%d",indice_miembros->cantidad_elementos_actual);
}

///------------------------------------------------------------------

///15.Dado un rango de DNIs, listar todos los miembros dentro de ese rango

void listar_miembros_por_rango_dni_ingresado_con_indice(t_indice *indice_miembros,t_vector *vec_miembros)
{
    t_reg_indice_miembro *indice_de_miembro = (t_reg_indice_miembro*)indice_miembros->vindice;
    t_reg_indice_miembro clave;

    t_miembro *miembro = (t_miembro*)vec_miembros->vdatos;
    long int dni_inferior,dni_superior;
    int pos_inferior,pos_superior,i,nro_reg;

    printf("\nIngrese el rango de DNIs de los cuales quiere listar los miembros:");
    do{
        printf("\nDNI inferior:");
        scanf("%ld",&dni_inferior);
        clave.dni = dni_inferior;
        pos_inferior = indice_buscar(indice_miembros,&clave,indice_miembros->cantidad_elementos_actual,sizeof(t_reg_indice_miembro),cmp_DNI);
        if(pos_inferior == -1)
        {
            printf("\nDNI no encontrado...Volver a ingresar:");
        }
    }while(pos_inferior == -1);

    do{
        printf("\nDNI superior:");
        scanf("%ld",&dni_superior);
        clave.dni = dni_superior;
        pos_superior = indice_buscar(indice_miembros,&clave,indice_miembros->cantidad_elementos_actual,sizeof(t_reg_indice_miembro),cmp_DNI);
        if(pos_superior == -1)
        {
            printf("\nDNI no encontrado...Volver a ingresar:");
        }
    }while(pos_superior == -1);

    printf("\nLos miembros con DNI pertenecientes al rango: %ld,%ld son:",dni_inferior,dni_superior);
    printf("\n");

    i = pos_inferior;

    while(indice_de_miembro[i].dni <= dni_superior && i < indice_miembros->cantidad_elementos_actual)
    {
        nro_reg = indice_de_miembro[i].nro_reg;
        mostrar_miembro(&miembro[nro_reg]);
        i++;
    }
}


///------------------------------------------------------------------

///ORDENAR BURBUJEO

/*void burbujeo(t_miembro *vec, int ce, int (*cmp)(const t_miembro*, const t_miembro*))
{
    int i, j;
    t_miembro aux;

    for(i = 0; i < ce - 1; i++)
    {
        for(j = 0; j < ce - i - 1; j++)
        {
            if(cmp(&vec[j], &vec[j+1]) > 0)
            {
                aux = vec[j];
                vec[j] = vec[j+1];
                vec[j+1] = aux;
            }
        }
    }
}*/




