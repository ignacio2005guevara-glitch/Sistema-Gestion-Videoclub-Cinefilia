#ifndef TITULOS_H_INCLUDED
#define TITULOS_H_INCLUDED

#include "Validaciones.h"
#include "Indice.h"
#include "Auditoria.h"
#include "Miembros.h"
#include "VectorDinamico.h"
#include "Alquileres.h"///8.

#define MAX_CARACTERES_TITULO_CSV 300
#define CANT_CAMPOS_TITULO  5


void cargar_titulos(const char *path, void *vec_titulos, t_indice *indice, void *vec_auditoria_titulos, t_fecha fecha_proceso);
int  alta_titulo(t_indice *indice, t_vector *vec);
int  baja_titulo(t_indice *indice, t_vector *vec);
int  modificar_titulo(t_indice *indice,t_vector *vec);
void persistir_titulos(const t_vector *vec, t_fecha fecha_proceso);
int cargar_titulos_bin(const char *path, t_vector *vec_titulos, t_indice *indice);
int cmp_id_pelicula(const void *valor1, const void *valor2);
int cmp_titulo(const void *valor1, const void *valor2);
void mostrar_reg_indice_titulo(const void *elem);
void mostrar_titulo(const void *elem);
void mostrar_encabezado_titulos(void);
void mostrar_titulo_por_id(const t_indice *indice, const t_vector *vec);
void listar_titulos_vector(const t_vector *vec);
int generar_id_titulo(const t_indice *indice);

///1.Listar todos los títulos con stock 0, ordenados por título alfabéticamente

void listar_titulos_stock_cero(const t_vector *vec_titulos);

///4.Dado un género ingresado por teclado, listar los títulos de ese género ordenados por título

void listar_titulos_del_genero_ingresado_ordenadoxtitulo(const t_vector *vec_titulos);

///6.Listar títulos ordenados por stock descendente

void listar_titulos_ordenados_por_stock_descendente(const t_vector *vec_titulos);
int cmp_stock_descendente(const void *valor1,const void *valor2);

///8.Listar los títulos que nunca fueron alquilados

void listar_titulos_nunca_alquilados(const t_vector *vec_titulos,const t_vector *vec_alquileres);
int buscar_titulo_en_alquileres(int ce,int id_pelicula,t_alquiler *alquileres);

#endif /* TITULOS_H_INCLUDED */
