#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#define MAX_NOMBRE 60
#define MAX_PLAN   10
#define MAX_EMAIL  30
#define MAX_CUIL   15
#define MAX_CATEGORIA 10

#define MAX_TITULO 60
#define MAX_GENERO 20

typedef struct
{
    int dia;
    int mes;
    int anio;
}t_fecha;

typedef struct
{
    long    dni;
    char    cuil[MAX_CUIL];
    char    nombre[MAX_NOMBRE];
    t_fecha fecha_nacimiento;
    char    sexo;
    t_fecha fecha_afiliacion;
    char    categoria[MAX_CATEGORIA];      /* "MENOR" o "ADULTO" */
    t_fecha fecha_ultima_cuota;
    char    estado;             /* 'A' o 'B' */
    char    plan[MAX_PLAN];
    char    email_tutor[MAX_EMAIL];
}t_miembro;

typedef struct
{
    int  id_pelicula;
    char titulo[MAX_TITULO];
    char genero[MAX_GENERO];
    int  stock;
    char    estado;
} t_titulo;

typedef struct
{
    unsigned nro_reg;
    long     dni;
} t_reg_indice_miembro;

typedef struct
{
    unsigned nro_reg;
    int     id;
} t_reg_indice_titulo;

#endif // ESTRUCTURAS_H_INCLUDED
