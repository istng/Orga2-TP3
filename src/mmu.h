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
	unsigned char   disponible:3;
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
	unsigned char   disponible:3;
    unsigned int    direccion:20;

} __attribute__((__packed__, aligned (4))) pte_entry;


void mmu_inicializar();
unsigned int mmu_proxima_pagina_fisica_libre();
void mmu_mappear_pagina(unsigned int virtual, unsigned int dir_pd, unsigned int fisica, unsigned int user_lvl);
void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3);
void mmu_inicializar_dir_kernel();
void identity_mapping(unsigned int directorio);

unsigned int pos_a_dirMapa(unsigned int i, unsigned int j);
void mappear_entorno_zombi(unsigned int i, unsigned int j, int jugador, unsigned int dir_pd );
unsigned int mmu_inicializar_dir_zombi();

#endif	/* !__MMU_H__ */
