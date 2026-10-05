#ifndef MIEMBROS_H_INCLUDED
#define MIEMBROS_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Auditoria.h"
#include "VectorDinamico.h"
#include "Alquileres.h" ///5.

#define MAX_EMAIL  30
#define MAX_CUIL   15
#define MAX_CARACTERES_MIEMBRO_CSV 300
#define CANT_CAMPOS_MIEMBRO 16

void cargar_miembros(const char *path, void *vec_miembros, t_indice *indice, void *vec_auditoria, t_fecha fecha_proceso);
int  alta_miembro(t_indice *indice, t_vector *vec, t_fecha fecha_proceso);
int  baja_miembro(t_indice *indice,t_vector *vec);
int  modificar_miembro(t_indice *indice, t_vector *vec, t_fecha fechaProceso);
void mostrar_miembro(const void *elem);
void mostrar_encabezado_miembros(void);
void mostrar_miembro_por_dni(const t_indice *indice, const t_vector *vec);
void listar_miembros_por_DNI(const t_indice *indice, const t_vector *vec);
void listar_miembros_por_plan(const t_indice *indice, const t_vector *vec);
void listar_miembros_vector(const t_vector *vec);
void mostrar_reg_indice_miembro(const void *elem);


//Persistir y cargar a binario
void persistir_miembros(const t_vector *vec, t_fecha fecha_proceso);
int cargar_miembros_bin(const char *path, t_vector *vec_miembros, t_indice *indice);

int cmp_miembro(const void *a, const void *b);
int cmp_DNI(const void *valor, const void *valor2);
int cmp_nombre(const void *a, const void *b);

///2.Listar miembros con estado B (dados de baja), ordenados por DNI

void listar_miembros_dados_baja_por_DNI(const t_vector *vec_miembros);
int cmp_miembro_dni(const void *a,const void *b);

///3.Mostrar cantidad de miembros por categoría (MENOR / ADULTO)

void mostrar_cantidad_miembros_por_categoria(const t_vector *vec_miembros);

///5. Listar los miembros que tienen al menos un alquiler registrado, ordenados por apellido y nombre

void listar_miembros_con_almenos_un_alquiler_ordenadoxApellidoyNombre(const t_vector *vec_miembros,const t_vector *vec_alquileres);
int buscar_miembro_en_alquileres(int ce,long dni,t_alquiler *alquileres);

///7. Dado un plan ingresado por teclado, mostrar cuántos miembros activos tiene ese plan

void listar_miembros_activos_por_PlanIngresado(const t_vector *vec_miembros);

///9. Dado un DNI, mostrar todos los titulos que alquilo con su cantidad de veces

void PorIngresoDNI_mostrar_titulos_alquilados(const t_indice *indice_miembros, const t_vector *vec_alquileres, const t_vector *vec_titulos);
int buscar_miembro_en_indice(int ce,long int dni,t_reg_indice_miembro *indice_miembro);
int buscar_id_pelicula_en_titulos(int ce,int id_pelicula,t_titulo *titulo);//en realidad debe ir en titulos.h pero por prolijidad lo pongo aca

///10.Listar los miembros activos cuya última cuota paga supera los 90 días desde la fecha de proceso, ordenados por fecha de última cuota ascendente.

void listar_miembros_con_ultima_cuota_paga_superior_a_90_desde_fecha_proceso(const t_vector *vec_miembros,t_fecha fecha_proceso);
void ordenamiento_burbujeo(t_miembro *vec,int ce,int(*cmp)(const t_miembro*,const t_miembro*));
int cmp_fecha_ultima_cuota(const t_miembro *a,const t_miembro *b);
int fecha_a_dias_exacto(t_fecha f);
int dias_entre_fechas(t_fecha f1,t_fecha f2);

///11.Dado un DNI ingresado por teclado, mostrar si existe o no en el índice y su posición

void PorIngresoDNI_mostrar_si_existe_en_indice_miembros(t_indice *indice_miembros);

///12.Mostrar todos los DNIs del indice en orden

void mostrar_dni_del_indice_en_orden(t_indice *indice_miembros);

///13.Dado un DNI, mostrar todos los datos completos del miembro usando el indice

int PorIngresoDNI_mostrar_datos_completos_miembro_con_indice(t_indice *indice_miembros,t_vector *vec_miembros);

///14.Contar cuantos miembros activos hay en el indice

void Contar_miembros_activos_con_indice(t_indice *indice_miembros);

///15.Dado un rango de DNIs, listar todos los miembros dentro de ese rango

void listar_miembros_por_rango_dni_ingresado_con_indice(t_indice *indice_miembros,t_vector *vec_miembros);


#endif /* MIEMBROS_H_INCLUDED */
