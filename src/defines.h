/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_ZOMBIS             8

#define SIZE_W                  78
#define SIZE_H                  44


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 13

#define GDT_IDX_NULL_DESC           0

#define GDT_CODIGO_0           8
#define GDT_CODIGO_3           9
#define GDT_DATA_0           10
#define GDT_DATA_3           11
#define GDT_VIDEO            12

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */

/* MMU */
/* -------------------------------------------------------------------------- */
#define INICIO_PAGINAS_LIBRES   0x28000 /* Tablas de paginas Kernel */
#define PAGE_SIZE   0x1000 /* 4 KB */
#define DIR_TAREAS   0x08000000 /* Inicio de paginas de tareas */


#endif  /* !__DEFINES_H__ */
