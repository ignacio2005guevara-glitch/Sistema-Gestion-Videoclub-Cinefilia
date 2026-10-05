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

#define TAM_NOMBRE_COMPLETO 60

#define MI_TOUPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 32) : (c))
#define MI_TOLOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define ES_LETRA(c)   (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') ? 1 : 0)
#define ES_DIGITO(c) ((c) >= '0' && (c) <= '9')

int cmp_fecha(const void *a, const void *b);
void normalizar(const char *nombre_original, char *nombre_normalizado, int con_coma);;
int validar_email(const char* email);
bool fecha_anio_bisiesto(t_fecha fecha);
int es_fecha_valida(t_fecha fecha);
int validar_DNI(long *dni);
int validar_CUIL(long* dni, char* sexo, const char* cuil_archivo);
int calcular_CUIL(long* dni, char* sexo, char* cuil_destino);
int validar_caracter(char dato,const char opciones[],int ce);
int validar_plan_genero(const char *dato,const char *opciones[], int ce);
int validar_stock(int dato,int li,int excepcion);
t_fecha obtener_fecha_actual();
int validar_categoria(const char *categoria,const t_miembro *miembro, const t_fecha *hoy);
int validar_id(int *id, t_indice *indice);
t_fecha obtener_fecha_proceso(void);
void str_to_upper(char *cadena);
int validar_nombre(char *nombre);
int archivo_existe(const char* path, const char* modo);

#endif /* VALIDACIONES_H_INCLUDED */
