#include "Validaciones.h"
#define MI_TOUPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 32) : (c))
#define MI_TOLOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define ES_LETRA(c)   (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') ? 1 : 0)


int cmp_dni(const void *a, const void *b)
{
    long dni_a = *(const long *)a;
    long dni_b = *(const long *)b;

    if (dni_a < dni_b) return -1;
    if (dni_a > dni_b) return  1;
    return 0;
}

int cmp_fecha(const void *a, const void *b)
{
    const t_fecha *fa = (const t_fecha *)a;
    const t_fecha *fb = (const t_fecha *)b;

    if (fa->anio != fb->anio) return (fa->anio < fb->anio) ? -1 : 1;
    if (fa->mes  != fb->mes ) return (fa->mes  < fb->mes ) ? -1 : 1;
    if (fa->dia  != fb->dia ) return (fa->dia  < fb->dia ) ? -1 : 1;
    return 0;
}

void normalizar(const char *nombre_original, char *nombre_normalizado, int con_coma)
{
    char* lec = (char*)nombre_original;
    char* esc = nombre_normalizado;
    int palabrasEscritas = 0;


    while(*lec) // Mientras haya cadena para leer
    {
        while(*lec && !ES_LETRA(*lec)) // Busco la primera/proxima palabra, ignoro espacios y ','
        {
            lec++;
        }

        if(*lec) // Proceso Palabra
        {
            if(palabrasEscritas == 1 && con_coma) // Escribir separador ANTES de esta palabra
            {
                // Segunda palabra => viene el nombre: escribir ", "
                *esc = ',';
                esc++;
                *esc = ' ';
                esc++;
            }
            else if(palabrasEscritas >= 1)
            {
                // Tercera palabra en adelante => espacio entre nombres
                *esc = ' ';
                esc++;
            }

            *esc = MI_TOUPPER(*lec); // Primer letra Mayuscula
            lec++;
            esc++;
            while(*lec && ES_LETRA(*lec)) // Resto de letras Minsucula
            {
                *esc = MI_TOLOWER(*lec);
                lec++;
                esc++;
            }

            palabrasEscritas++;
        }
    }
    *esc = '\0';
}

int validar_email(const char* email) //Retorna 1 si es valido o 0 si no lo es.
{
    const char* p_email = email;
    const char* p_arroba = NULL; //puntero que va a apuntar al @
    const char* p_dominio = NULL;
    int contArrobas = 0,hayPunto = 0;

    if(*p_email == '\0')
    {
        return 0; //esta vacio
    }

    while(*(p_email) != '\0')
    {
        if(*(p_email) == ' ')
        {
            return 0; //se encontro espacio
        }
        else
        {
            if(*(p_email) == '@')
            {
                contArrobas++;
                p_arroba = p_email; //guardo la posicion del @
            }
        }

        p_email++;
    }

    if(contArrobas != 1 || p_arroba == email || *(p_arroba+1) == '\0') //Verifico que haya exactamente 1 @,y que @ no sea ni el primer ni ultimo caracter.
    {
        return 0;
    }

    p_dominio = p_arroba+2; //Dominio : El proveedor del servicio
    while(*(p_dominio) != '\0')
    {
        if(*(p_dominio) == '.')
        {
            hayPunto++;
        }
        p_dominio++;
    }

    if(hayPunto == 0 || hayPunto > 2) //porq si es 0 no hay punto, pero si hay 2 o mas quiere decir que es por ejemplo juan.cruz@gmail...com y no es correcto.Ademas verifico el [opcional] como .ar
    {
         return 0;
    }

    if(strlen(email) > 30)
    {
        return 0; //es mayor a 30 y no se puede
    }

    return 1; //si pasa todas las validaciones el email es VALIDO
}

bool fecha_anio_bisiesto(t_fecha fecha)
{
    int resultado=0;
    if(((fecha.anio%4 == 0) && (fecha.anio%100 != 0)) || (fecha.anio%400 == 0))
        resultado=true;
    return resultado;
}

int es_fecha_valida(t_fecha fecha)
{
    int DiasPorMes[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

    if(fecha.anio <=1799)
        return ERROR;

    if(fecha.mes<1||fecha.mes>12)
        return ERROR;

    if(fecha.mes==2&&fecha_anio_bisiesto(fecha))
    {
        if(fecha.dia<1||fecha.dia>29)
            return ERROR;
    }
    else
    {
        if(fecha.dia<1||fecha.dia>DiasPorMes[fecha.mes])
            return ERROR;
    }
    return OK;
}

t_fecha parsear_fechas(const char* cadena)
{
    int dia, mes,anio, leidos;
    t_fecha fecha={0};

    if(cadena == NULL)
        return fecha;

    if(strlen(cadena) != 10)
        return fecha;

    if(*(cadena+2)!='/'||*(cadena+5)!='/')
        return fecha;

    leidos = scanf(cadena,"%d/%d/%d",&dia,&mes,&anio);

    if(leidos!=3)
        return fecha;

    fecha.dia=dia;
    fecha.mes=mes;
    fecha.anio=anio;

    return fecha;
}

int validar_DNI(long *dni)
{
    return (*dni > 1000000 && *dni < 100000000);
}

int validar_CUIL(long* dni, char* sexo, const char* cuil_archivo)
{
    char cuil_calculado[14];

    if(calcular_CUIL(dni, sexo, cuil_calculado) == ERROR)
    {
        return ERROR;
    }

    if (strcmp(cuil_archivo, cuil_calculado) == 0) {
        return OK;
    } else {
        return ERROR;
    }
}

int calcular_CUIL(long* dni, char* sexo, char* cuil_destino)
{
    // Coeficientes fijos para la multiplicación
    int num[] = {5, 4, 3, 2, 7, 6, 5, 4, 3, 2};
    int acumulado = 0;
    int sex_aux;
    int resto;
    int z;

    //determinamos prefijo inicial según el sexo
    if (*sexo == 'M' || *sexo == 'm') {
        sex_aux = 20;
    } else if (*sexo == 'F' || *sexo == 'f') {
        sex_aux = 27;
    } else if (*sexo == 'o' || *sexo == 'O'){
        sex_aux = 30; // Otro / Sociedad
    }
    else{
        return ERROR;
    }

    //sumamos el aporte del prefijo (posiciones 0 y 1 de los coeficientes)
    acumulado += (sex_aux / 10) * num[0];
    acumulado += (sex_aux % 10) * num[1];

    // descomponemos el DNI numérico de atrás hacia adelante (posiciones 9 a 2)
    long dni_aux = *dni;
    for (int i = 9; i >= 2; i--) {
        int digito = dni_aux % 10;
        acumulado += digito * num[i];
        dni_aux /= 10;
    }


    resto = acumulado % 11;


    if (resto == 0) {
        z = 0;
    }
    else if (resto == 1) {
        sex_aux = 23; // prefijjo a 23
        if (*sexo == 'M' || *sexo == 'm') {
            z = 9;
        } else {
            z = 4;
        }
    }
    else {
        z = 11 - resto;
    }

    sprintf(cuil_destino, "%02d-%08ld-%d", sex_aux, *dni, z);

    return OK; // OK
}

int validar_caracter(char dato,const char opciones[],int ce)
{
    int i;
    for(i=0;i<ce;i++)
    {
        if(dato == opciones[i])
            return OK; // OK = 1
    }
    return ERROR; // ERROR = 0
}

int validar_plan_genero(const char *dato,const char *opciones[], int ce)
{
    char dato_normalizado[20], opcion_normalizado[20];
    int i;

    normalizar(dato, dato_normalizado, 0);
    for(i=0;i<ce;i++)
    {
        normalizar(opciones[i], opcion_normalizado, 0);
        if(strcmp(dato_normalizado,opcion_normalizado)==0)
            return OK; //valido
    }

    return ERROR; // invalido
}

int validar_stock(int dato,int li,int excepcion)
{
    if(dato<li && dato!=excepcion)
        return ERROR; //invalido
    return OK; //valido
}

int validar_ID_archivo(int id,const int *pv,int ce)
{
    int i;
    if(ce==0)
        return 0;
    for(i = 0; i < ce; i++)
    {
        if(id == *(pv+i))
            return 1;  // repetido
    }
    if(id == *(pv+ce-1) + 1)
        return 0; //chequea que sea incremental
    return 1;
}

t_fecha obtener_fecha_actual()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    t_fecha f;
    f.dia = tm->tm_mday;
    f.mes = tm->tm_mon + 1;
    f.anio = tm->tm_year + 1900;

    return f;
}

int validar_categoria(const char *categoria,const t_miembro *miembro, const t_fecha *hoy)
{
    int edad;

    if(strcmp(categoria, "MENOR") != 0 && strcmp(categoria, "ADULTO") != 0)
        return ERROR;

    edad = hoy->anio - miembro->fecha_nacimiento.anio;

    if(hoy->mes < miembro->fecha_nacimiento.mes || (hoy->mes == miembro->fecha_nacimiento.mes && hoy->dia < miembro->fecha_nacimiento.dia))
        edad--;

    if(edad < 18)
    {
        if(strcmp(categoria, "ADULTO") == 0 || strcmp(miembro->email_tutor, "") == 0)
            return ERROR;
    }
    else if(strcmp(categoria, "MENOR") == 0)
            return ERROR;

    return OK;
}

int validar_id(int *id, t_indice *indice)
{
    t_reg_indice_titulo reg_buscar;

    // Validar que sea mayor a 0
    if(*id <= 0)
        return ERROR;

    // Validar que no exista en el indice (debe ser único)
    reg_buscar.id      = *id;
    reg_buscar.nro_reg = 0;

    if(indice_buscar(indice, &reg_buscar, indice->cantidad_elementos_actual, sizeof(t_reg_indice_titulo), cmp_id_pelicula) != NO_EXISTE)
        return ERROR;   /* ya existe → inválido */

    return OK;
}

t_fecha obtener_fecha_proceso(void)
{
    t_fecha fecha;
    char input[20];

    printf("Ingrese la fecha de proceso (DD/MM/AAAA) o Enter para usar la actual: ");
    fgets(input, sizeof(input), stdin);

    /* Si solo presionó Enter, usar la fecha actual */
    if(input[0] == '\n')
    {
        return obtener_fecha_actual();
    }

    /* Intentar parsear la fecha ingresada */
    if(sscanf(input, "%d/%d/%d", &fecha.dia, &fecha.mes, &fecha.anio) != 3)
    {
        printf("Formato invalido. Se usara la fecha actual.\n");
        return obtener_fecha_actual();
    }

    /* Validar que sea una fecha real */
    if(!es_fecha_valida(fecha))
    {
        printf("Fecha invalida. Se usara la fecha actual.\n");
        return obtener_fecha_actual();
    }

    return fecha;
}




