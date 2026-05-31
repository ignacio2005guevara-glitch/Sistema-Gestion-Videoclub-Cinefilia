#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Miembros.h"

void cargar_miembros(const char *path, void* vec, t_indice *indice, t_error *auditoria, t_fecha fecha_proceso)
{
    const char *Planes[]  = {"BASIC", "PREMIUM", "VIP", "FAMILY"};
    //const char *Generos[] = {"Drama", "Comedia", "Terror", "Accion"};
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
            registrar_error(auditoria, "DNI", &registro.dni);
            continue;
        }

        // Validar CUIL
        if(!validar_CUIL(&registro.dni, &registro.sexo, registro.cuil))
        {
            registrar_error(auditoria, "CUIL", &registro.dni);
            continue;
        }

        // Validar Fecha de nacimiento
        t_fecha fecha_limite; // fecha del registro - 10 años
        fecha_limite.dia = fecha_proceso.dia;
        fecha_limite.mes = fecha_proceso.mes;
        fecha_limite.anio = fecha_proceso.anio - 10;

        if(!es_fecha_valida(registro.fecha_nacimiento) || cmp_fecha(&registro.fecha_nacimiento, &fecha_limite) >= 0)
        {
            registrar_error(auditoria, "FEC NACIMIENTO", &registro.dni);
            continue;
        }

        // Validar Sexo
        if(!validar_caracter(registro.sexo, Sexos, CANT_SEXOS))
        {
            registrar_error(auditoria, "SEXO", &registro.dni);
            continue;
        }

        // Validar Fecha de afiliacion
        if(!es_fecha_valida(registro.fecha_afiliacion) || cmp_fecha(&registro.fecha_afiliacion, &registro.fecha_nacimiento) < 0 || cmp_fecha(&registro.fecha_afiliacion, &fecha_proceso) > 0 )
        {
            registrar_error(auditoria, "FEC AFILIACION", &registro.dni);
            continue;
        }

        // Validar Categoria
        if(!validar_categoria(registro.categoria, &registro, &fecha_proceso))
        {
            registrar_error(auditoria, "CATEGORIA", &registro.dni);
            continue;
        }

        // Validar Fecha ultima cuota paga
        if(!es_fecha_valida(registro.fecha_afiliacion) || cmp_fecha(&registro.fecha_ultima_cuota, &registro.fecha_afiliacion) < 0 || cmp_fecha(&registro.fecha_ultima_cuota, &fecha_proceso) > 0)
        {
            registrar_error(auditoria, "FEC ULT CUOTA PAGA", &registro.dni);
            continue;
        }

        // Validar Estado
        if(!validar_caracter(registro.estado, Estados, CANT_ESTADOS))
        {
            registrar_error(auditoria, "ESTADO", &registro.dni);
            continue;
        }

        // Validar Plan
        if(!validar_plan_genero(registro.plan, Planes, CANT_PLANES_GENEROS))
        {
            registrar_error(auditoria, "PLAN", &registro.dni);
            continue;
        }

        // Validar Email Tutor
        /// PENDIENTE. ¿QUE FUNCION HAY QUE USAR?

        // Normalizar nombre
        char nombre_normalizado[MAX_NOMBRE];
        normalizar(registro.nombre, nombre_normalizado, 1);
        strcpy(registro.nombre, nombre_normalizado);


        // Revisar si la clave no existe en el indice. Armar reg_indice con la clave a buscar
        reg_indice.dni     = registro.dni;
        reg_indice.nro_reg = 0;   /* nro_reg no importa para la búsqueda */

        if(indice_buscar(indice, &reg_indice, indice->cantidad_elementos_actual, sizeof(t_reg_indice_miembro), cmp_DNI) == NO_EXISTE)
        {
            // Nueva clave -> escribir en archivo e insertar en indice
            vector_agregar(vec, &registro, sizeof(t_miembro));
            reg_indice.nro_reg = nro_reg;
            indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_miembro), cmp_DNI);
            nro_reg++;
        }
        else
        {
            // Registro duplicado
            continue; /// DEFINIR QUE HACER EN ESTE CASO
        }
    }

    fclose(pfArchivo);
}

int  alta_miembro(t_indice *indice, t_vector *vec, t_fecha fecha_proceso)
{
    t_miembro nuevo_miembro;
    t_reg_indice_miembro reg_indice;
    //unsigned nro_reg = 0;
    char buffer[MAX_CARACTERES_MIEMBRO_CSV];
    const char *Planes[] = {"BASIC", "PREMIUM", "VIP", "FAMILY"};
    const char  Sexos[]  = {'F', 'M', 'O'};

    /* ── Pedir todos los datos ── */
    printf("Ingrese DNI: ");
    scanf("%ld", &nuevo_miembro.dni);
    getchar();
    fflush(stdin);

    printf("Ingrese CUIL: ");
    fgets(nuevo_miembro.cuil, MAX_CUIL, stdin);
    nuevo_miembro.cuil[strcspn(nuevo_miembro.cuil, "\n")] = '\0';
    fflush(stdin);

    printf("Ingrese Apellido y Nombre (Apellido, Nombre): ");
    fgets(nuevo_miembro.nombre, MAX_NOMBRE, stdin);
    nuevo_miembro.nombre[strcspn(nuevo_miembro.nombre, "\n")] = '\0';
    fflush(stdin);

    printf("Ingrese Fecha de Nacimiento (dd/mm/aaaa): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d/%d/%d", &nuevo_miembro.fecha_nacimiento.dia,
                               &nuevo_miembro.fecha_nacimiento.mes,
                               &nuevo_miembro.fecha_nacimiento.anio);
    fflush(stdin);

    printf("Ingrese Sexo (F/M/O): ");
    fgets(buffer, sizeof(buffer), stdin);
    nuevo_miembro.sexo = buffer[0];
    fflush(stdin);

    printf("Ingrese Fecha de Afiliacion (dd/mm/aaaa): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d/%d/%d", &nuevo_miembro.fecha_afiliacion.dia,
                               &nuevo_miembro.fecha_afiliacion.mes,
                               &nuevo_miembro.fecha_afiliacion.anio);
    fflush(stdin);

    printf("Seleccione Categoria:\n");
    printf("  1. MENOR\n  2. ADULTO\n\n");
    printf("Opcion: ");
    fgets(nuevo_miembro.categoria, MAX_CATEGORIA, stdin);
    nuevo_miembro.categoria[strcspn(nuevo_miembro.categoria, "\n")] = '\0';
    fflush(stdin);

    printf("Ingrese Fecha Ultima Cuota Paga (dd/mm/aaaa): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d/%d/%d", &nuevo_miembro.fecha_ultima_cuota.dia,
                               &nuevo_miembro.fecha_ultima_cuota.mes,
                               &nuevo_miembro.fecha_ultima_cuota.anio);
    fflush(stdin);

    printf("Seleccione Plan:\n");
    printf("  1. BASIC\n  2. PREMIUM\n  3. VIP\n  4. FAMILY\n");
    printf("Opcion: ");
    fgets(nuevo_miembro.plan, MAX_PLAN, stdin);
    nuevo_miembro.plan[strcspn(nuevo_miembro.plan, "\n")] = '\0';
    fflush(stdin);

    // Validar DNI
    if(!validar_DNI(&nuevo_miembro.dni))
    {
        //registrar_error(auditoria, "DNI", &nuevo_miembro.dni);
        return ERROR;
    }


    // Validar DNI duplicado en el indice
    t_reg_indice_miembro reg_buscar;
    reg_buscar.dni = nuevo_miembro.dni;
    if(indice_buscar(indice, &reg_buscar,
                     indice->cantidad_elementos_actual,
                     sizeof(t_reg_indice_miembro), cmp_DNI) != NO_EXISTE)
    {
        printf("El DNI ya existe en el sistema.\n");
        return ERROR;
    }

    // Validar CUIL
    if(!validar_CUIL(&nuevo_miembro.dni, &nuevo_miembro.sexo, nuevo_miembro.cuil))
    {
        //registrar_error(auditoria, "CUIL", &nuevo_miembro.dni);
        return ERROR;
    }

    // Validar Fecha de nacimiento
    t_fecha fecha_limite; // fecha del registro - 10 años
    fecha_limite.dia = fecha_proceso.dia;
    fecha_limite.mes = fecha_proceso.mes;
    fecha_limite.anio = fecha_proceso.anio - 10;

    if(!es_fecha_valida(nuevo_miembro.fecha_nacimiento) || cmp_fecha(&nuevo_miembro.fecha_nacimiento, &fecha_limite) >= 0)
    {
        //registrar_error(auditoria, "FEC NACIMIENTO", &nuevo_miembro.dni);
        return ERROR;
    }

    // Validar Sexo
    if(!validar_caracter(nuevo_miembro.sexo, Sexos, CANT_SEXOS))
    {
        //registrar_error(auditoria, "SEXO", &nuevo_miembro.dni);
        return ERROR;
    }

    // Validar Fecha de afiliacion
    if(!es_fecha_valida(nuevo_miembro.fecha_afiliacion) || cmp_fecha(&nuevo_miembro.fecha_afiliacion, &nuevo_miembro.fecha_nacimiento) < 0 || cmp_fecha(&nuevo_miembro.fecha_afiliacion, &fecha_proceso) > 0 )
    {
        //registrar_error(auditoria, "FEC AFILIACION", &nuevo_miembro.dni);
        return ERROR;
    }

    // Validar Categoria
    if(!validar_categoria(nuevo_miembro.categoria, &nuevo_miembro, &fecha_proceso))
    {
        //registrar_error(auditoria, "CATEGORIA", &nuevo_miembro.dni);
        return ERROR;
    }

    // Validar Fecha ultima cuota paga
    if(!es_fecha_valida(nuevo_miembro.fecha_afiliacion) || cmp_fecha(&nuevo_miembro.fecha_ultima_cuota, &nuevo_miembro.fecha_afiliacion) < 0 || cmp_fecha(&nuevo_miembro.fecha_ultima_cuota, &fecha_proceso) > 0)
    {
        //registrar_error(auditoria, "FEC ULT CUOTA PAGA", &nuevo_miembro.dni);
        return ERROR;
    }

    // Validar Plan
    if(!validar_plan_genero(nuevo_miembro.plan, Planes, CANT_PLANES_GENEROS))
    {
        //registrar_error(auditoria, "PLAN", &nuevo_miembro.dni);
        return ERROR;
    }

    // Validar Email Tutor
    /// PENDIENTE. ¿QUE FUNCION HAY QUE USAR?

    // Normalizar Nombre
    char nombre_normalizado[MAX_NOMBRE];
    normalizar(nuevo_miembro.nombre, nombre_normalizado, 1);
    strcpy(nuevo_miembro.nombre, nombre_normalizado);

    // Estado = 'A'
    nuevo_miembro.estado = 'A';

    // Revisar si la clave no existe en el indice. Armar reg_indice con la clave a buscar
    reg_indice.dni     = nuevo_miembro.dni;
    reg_indice.nro_reg = vec->cantidad_actual;
    //reg_indice.nro_reg = 0;   /* nro_reg no importa para la búsqueda */

    if(indice_buscar(indice, &reg_indice, indice->cantidad_elementos_actual, sizeof(t_reg_indice_miembro), cmp_DNI) == NO_EXISTE)
    {
        // Nueva clave -> escribir en archivo e insertar en indice
        vector_agregar(vec, &nuevo_miembro, sizeof(t_miembro));
        indice_insertar(indice, &reg_indice, sizeof(t_reg_indice_miembro), cmp_DNI);
    }
    else
    {
        // Registro duplicado
        printf("\nERROR: DNI duplicado. Ya existe un miembro con el DNI ingresado.\n");
    }

    ///PRUEBA mostrar vector
    printf("\n=========================================================================================================================================================================================\n");
    printf("%-9s | %-13s | %-60s | %-10s | %s | %-10s | %-10s | %-10s | %s | %-10s | %-30s\n",
           "DNI", "CUIL", "APELLIDO Y NOMBRE", "F. NAC", "S", "F. AFIL", "CATEGORIA", "U. PAGO", "E", "PLAN", "EMAIL TUTOR");
    printf("=========================================================================================================================================================================================\n");
    vector_mostrar(vec, sizeof(t_miembro), mostrar_miembro);
    //archivo_bin_mostrar(pf_miembros, sizeof(t_miembro), mostrar_miembro);

    /// PRUEBA mostrar indice
    printf("\nIndice Miembros:\n");
    indice_mostrar(indice, sizeof(t_reg_indice_miembro), mostrar_reg_indice_miembro);

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
        printf("\nDESEA CONFIRMAR LA BAJA DEL USUARIO %s (Y/N):",miembro.nombre);
        scanf(" %c",&validacion);
        if(validacion == 'y' || validacion == 'Y')
        {

            miembro.estado='B';
            memcpy(pos,&miembro,sizeof(t_miembro));
            indice_eliminar(indice,&registro_a_buscar,sizeof(t_reg_indice),cmp_DNI);
        }
        else
            printf("\nBAJA CANCELADA\n");
    }

        /// PRUEBA mostrar vec miembros
    printf("\n=========================================================================================================================================================================================\n");
    printf("%-9s | %-13s | %-60s | %-10s | %s | %-10s | %-10s | %-10s | %s | %-10s | %-30s\n",
           "DNI", "CUIL", "APELLIDO Y NOMBRE", "F. NAC", "S", "F. AFIL", "CATEGORIA", "U. PAGO", "E", "PLAN", "EMAIL TUTOR");
    printf("=========================================================================================================================================================================================\n");
    vector_mostrar(vec, sizeof(t_miembro), mostrar_miembro);

    /// PRUEBA mostrar indice miembros
    printf("\nIndice Miembros:\n");
    indice_mostrar(indice, sizeof(t_reg_indice_miembro), mostrar_reg_indice_miembro);

    return 1;
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
    printf("5. Fecha de Afiliación\n");
    printf("6. Plan\n");
    printf("7. Email del Tutor\n");
    printf("8. Fecha de Última Cuota Paga\n");
    printf("Elija el número del campo que desea modificar: ");
    scanf("%d", &opcion);

    switch(opcion) {
        case 1: { //NO ANDA EL CUIL
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
                printf("Error: El DNI nuevo ya está registrado.\n");
                return ERROR;
                }
            printf("%s\n",miembro->nombre);
            printf("%ld\n",miembro->dni);
            printf("%c\n",miembro->sexo);

            miembro->dni = dni_nuevo;
            cambio_dni = 1; // Bandera para actualizar el índice al final

            calcular_CUIL(&miembro->dni, &miembro->sexo, miembro->cuil); //como cambio el dni tambien cambia el cuil
            //printf("%c\n",miembro->sexo);
            printf("CUIL recalculado automáticamente: %s\n", miembro->cuil);
            break;
        }

        case 2: { // Modificar Nombre y Apellido ANDA BIEN
            char nuevo_nombre[60];
            printf("Ingrese el nuevo Apellido y Nombre: ");
            fflush(stdin);
            gets(nuevo_nombre);

            // normalizamos
            char nombre_normalizado[60];
            normalizar(nuevo_nombre, nombre_normalizado,1);
            strcpy(miembro->nombre, nombre_normalizado);
            break;
        }

        case 3: { // Modificar Fecha de Nacimiento ANDA BIEN
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
            scanf(" %c", &nuevo_sexo);

           if(!validar_caracter(nuevo_sexo, Sexos, CANT_SEXOS))
              {
                printf("Error: Sexo no válido.\n");
                return ERROR;
               }

            miembro->sexo = nuevo_sexo;

            calcular_CUIL(&miembro->dni, &miembro->sexo, miembro->cuil); //como cambio el sexo, cambia el cuil
            printf("CUIL recalculado por cambio de sexo: %s\n", miembro->cuil);
            break;
        }

        case 5: { // Modificar Fecha de Afiliación
            t_fecha nueva_afiliacion;
            printf("Ingrese nueva Fecha de Afiliación (DD MM AAAA): ");
            scanf("%d %d %d", &nueva_afiliacion.dia, &nueva_afiliacion.mes, &nueva_afiliacion.anio);

             if(!es_fecha_valida(nueva_afiliacion) || cmp_fecha(&nueva_afiliacion, &miembro->fecha_nacimiento) < 0 || cmp_fecha(&nueva_afiliacion, &fechaProceso) > 0 )
            miembro->fecha_afiliacion = nueva_afiliacion;
            break;
        }

        case 6: { // Modificar Plan ANDA BIEN
            char plan_nuevo[8];
            printf("Escriba el nuevo Plan:\n- BASIC\n- PREMIUM\n- VIP\n- FAMILY\nPlan: ");
            fflush(stdin);
            gets(plan_nuevo);

            if(!validar_plan_genero(plan_nuevo, Planes, CANT_PLANES_GENEROS))
                {
                printf("Opción invalida.\n");
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
            gets(nuevo_email);

           // if (validarEmail(nuevo_email) == ERROR)
                {
                printf("Error: Formato de Email inválido.\n");
                return ERROR;
               }

            if (strcmp(miembro->categoria, "MENOR") == 0 && strlen(nuevo_email) == 0)
                {
                printf("Error: Al ser MENOR, el mail del tutor no puede estar vacío.\n");
                return ERROR;
            }

            strcpy(miembro->email_tutor, nuevo_email);
            break;
        }

        case 8: { // Modificar fecha de ultima cuota paga
            t_fecha nueva_cuota;
            printf("Ingrese Fecha de Última Cuota (DD MM AAAA): ");
            scanf("%d %d %d", &nueva_cuota.dia, &nueva_cuota.mes, &nueva_cuota.anio);

            if(!es_fecha_valida(nueva_cuota) || cmp_fecha(&nueva_cuota,& miembro->fecha_afiliacion) < 0 || cmp_fecha(&nueva_cuota, &fechaProceso) > 0)
            miembro->fecha_ultima_cuota = nueva_cuota;
            break;
        }


        default:
            printf("Opción inválida. Volviendo al menú.\n");
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

    printf("Miembro e índice modificados con éxito\n");

        /// PRUEBA mostrar vec miembros
    printf("\n=========================================================================================================================================================================================\n");
    printf("%-9s | %-13s | %-60s | %-10s | %s | %-10s | %-10s | %-10s | %s | %-10s | %-30s\n",
           "DNI", "CUIL", "APELLIDO Y NOMBRE", "F. NAC", "S", "F. AFIL", "CATEGORIA", "U. PAGO", "E", "PLAN", "EMAIL TUTOR");
    printf("=========================================================================================================================================================================================\n");
    vector_mostrar(vec, sizeof(t_miembro), mostrar_miembro);

    /// PRUEBA mostrar indice miembros
    printf("\nIndice Miembros:\n");
    indice_mostrar(indice, sizeof(t_reg_indice_miembro), mostrar_reg_indice_miembro);


    return 1;
}

void listar_miembros_por_DNI (const t_indice *indice,const t_vector *vec)
{
    int i;
    t_reg_indice_miembro *reg;
    t_miembro *miembro;

    mostrar_encabezado_miembros();

    for(i=0;i< indice->cantidad_elementos_actual;i++)
    {
        reg = (t_reg_indice_miembro*)indice->vindice + i; // 1. Obtengo el registro del indice

        miembro = (t_miembro*)vec->vdatos + reg->nro_reg; // 2. Con nro_reg accedo al miembro completo en el vector
        if(miembro->estado == 'A') // 3. Solo muestro los activos
        {
            mostrar_miembro(miembro);
        }
    }
}

void listar_miembros_por_plan(const t_indice *indice,const t_vector *vec)
{
    int i;
    t_miembro *veccopia = malloc(vec->cantidad_actual * sizeof(t_miembro)); // 1 y 2. Crear copia
    memcpy(veccopia, vec->vdatos, vec->cantidad_actual * sizeof(t_miembro));

    qsort(veccopia, vec->cantidad_actual, sizeof(t_miembro), cmp_nombre);// 3. Ordenar por nombre

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

// --- Función para imprimir un único miembro respetando los límites máximos ---
void miembro_mostrar_linea(const t_miembro* m)
{
    // %-9ld     -> DNI (long, hasta 9 dígitos para 100.000.000)
    // %-13.13s  -> CUIL (char[14], mide 13 caracteres de texto visible: XX-XXXXXXXX-X)
    // %-60.60s  -> Apellido y Nombre (char[61], máximo 60 caracteres de texto)
    // %-10.10s  -> Categoría (char[11], máximo 10 caracteres: 'ADULTO' o 'MENOR')
    // %-10.10s  -> Plan (char[11], máximo 10 caracteres: 'PREMIUM', 'BASIC', etc.)
    // %-30.30s  -> Email Tutor (char[31], máximo 30 caracteres)

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

// --- Función de lectura del archivo binario ---
int mostrar_archivo_miembros(const char* nombre_binario)
{
    FILE* arch_bin = fopen(nombre_binario, "rb");
    if (arch_bin == NULL)
    {
        printf("Error: No se pudo abrir el archivo binario %s\n", nombre_binario);
        return 0;
    }

    t_miembro socio;

    // Encabezado con los anchos de columna idénticos a los datos del printf inferior
    printf("\n=========================================================================================================================================================================================\n");
    printf("%-9s | %-13s | %-60s | %-10s | %s | %-10s | %-10s | %-10s | %s | %-10s | %-30s\n",
           "DNI", "CUIL", "APELLIDO Y NOMBRE", "F. NAC", "S", "F. AFIL", "CATEGORIA", "U. PAGO", "E", "PLAN", "EMAIL TUTOR");
    printf("=========================================================================================================================================================================================\n");

    while (fread(&socio, sizeof(t_miembro), 1, arch_bin) == 1)
    {
        miembro_mostrar_linea(&socio);
    }

    printf("=========================================================================================================================================================================================\n\n");

    fclose(arch_bin);
    return 1;
}

void persistir_miembros(const t_indice *indice, t_fecha fechaProceso)
{

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
    //t_reg_indice_miembro reg;
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
        return -1;
    }
    else if(v1->dni > v2->dni)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int cmp_nombre(const void *a, const void *b)
{
    t_miembro *ma = (t_miembro*) a;
    t_miembro *mb = (t_miembro*) b;

    return strcmp(ma->nombre, mb->nombre);
}
