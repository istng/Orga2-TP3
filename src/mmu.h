/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"



/********************************/
/******** PDE entry *************/
/********************************/

typedef struct str_pde_entry {
	unsigned char	present:1;
	unsigned char	read_write:1;
	unsigned char	user_supervisor:1;
	unsigned char	plvl_writethr:1;
	unsigned char 	plvl_cachedisable:1;
	unsigned char	accesed:1;
	unsigned char	ignored:1;
	unsigned char   pagesize:1;
	unsigned char   global:1;
	unsigned char   disponible:4;
    unsigned int    direccion:20;

} __attribute__((__packed__, aligned (4))) pde_entry;



/********************************/
/******** PTE entry *************/
/********************************/

typedef struct str_pte_entry {
	unsigned char	present:1;
	unsigned char	read_write:1;
	unsigned char	user_supervisor:1;
	unsigned char	plvl_writethr:1;
	unsigned char 	plvl_cachedisable:1;
	unsigned char	accesed:1;
	unsigned char	dirty_bit:1;
	unsigned char   page_table_attribute_index:1;
	unsigned char   global:1;
	unsigned char   disponible:4;
    unsigned int    direccion:20;

} __attribute__((__packed__, aligned (4))) pte_entry;




void mmu_inicializar();
void mmu_inicializar_dir_kernel();

#endif	/* !__MMU_H__ */




