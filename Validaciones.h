#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "Estructuras.h"
#include "Indice.h"
#include "Titulos.h"

#define CANT_PLANES_GENEROS  4
#define CANT_SEXOS   3
#define CANT_ESTADOS 2
#define OK 1
#define ERROR 0
#define TAM_NOMBRE_COMPLETO 60
#define CANT_PLANES_GENEROS  4
#define CANT_SEXOS   3
#define CANT_ESTADOS 2

typedef int (*Cmp)(const void *, const void *);


int cmp_dni  (const void *a, const void *b);
int cmp_fecha(const void *a, const void *b);
void normalizar(const char *nombre_original, char *nombre_normalizado, int con_coma);;
int validar_email(const char* email); //Retorna 1 si es valido o 0 si no lo es.
bool fecha_anio_bisiesto(t_fecha fecha);
int es_fecha_valida(t_fecha fecha);
t_fecha parsear_fechas(const char* cadena);
int validar_DNI(long *dni);
int validar_CUIL(long* dni, char* sexo, const char* cuil_archivo);
int calcular_CUIL(long* dni, char* sexo, char* cuil_destino);
int validar_caracter(char dato,const char opciones[],int ce);
int validar_plan_genero(const char *dato,const char *opciones[], int ce);
int validar_stock(int dato,int li,int excepcion);
int validar_ID_archivo(int id,const int *pv,int ce);
t_fecha obtener_fecha_actual();
int validar_categoria(const char *categoria,const t_miembro *miembro, const t_fecha *hoy);
int validar_id(int *id, t_indice *indice);
t_fecha obtener_fecha_proceso(void);




#endif /* VALIDACIONES_H_INCLUDED */
