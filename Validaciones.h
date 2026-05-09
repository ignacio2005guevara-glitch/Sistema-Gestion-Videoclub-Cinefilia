#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define EXITO 1
#define ERROR 0
#define TAM_NOMBRE_COMPLETO 60


typedef struct
{
    int dia;
    int mes;
    int anio;
} t_fecha;


//int validar_generico(const void *dato, const void *contexto, t_fnValidar fn);


int validarDNI(long dni);
void calcularCUIL(long dni, char sexo, char cuil[15]);
int normalizar_nombre(const char* nombre_original, char* nombre_normalizado);
int validarEmail(const char *email);
int validarPlan(const char *plan);
int validarGenero(const char *genero);

#endif /* VALIDACIONES_H_INCLUDED */
