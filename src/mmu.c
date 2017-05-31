/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/












#include "mmu.h"


void mmu_inicializar() {

}


void mmu_inicializar_dir_kernel() {

	pde_entry* pd = (pde_entry*)0x27000;
	pte_entry* pt = (pte_entry*)0x28000;

	pd[0].present = 1;
	pd[0].read_write = 1;
	pd[0].user_supervisor = 1; ///??????
	pd[0].plvl_writethr = 0;
	pd[0].plvl_cachedisable = 0;
	pd[0].accesed = 0;
	pd[0].ignored = 0;
	pd[0].pagesize = 0;
	pd[0].global = 0; 
	pd[0].disponible = 0; //??????????????????????????????????????????????????????
	pd[0].direccion = 0x28000;

	

	int i;for (i = 1; i < 1024; ++i)
	{
		pd[i].present = 0;
	}

	for (i = 0; i < 1024; ++i)
	{
		pt[i].present = 1;
		pt[i].read_write = 1;
		pt[i].user_supervisor = 1; ///??????
		pt[i].plvl_writethr = 0;
		pt[i].plvl_cachedisable = 0;
		pt[i].accesed = 0;
		pt[i].dirty_bit = 0;
		pt[i].page_table_attribute_index = 0;
		pt[i].global = 0; 
		pt[i].disponible = 0; //??????????????????????????????????????????????????????
		pt[i].direccion = i;

	}	

}






