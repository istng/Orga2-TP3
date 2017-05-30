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

	str_pde_entry* pd = (pde*)0x27000;
	str_pte_entry* pt = (pte*)0x28000;

	pd[0] = completar una entrada de pde que apunte a la pagetable en 0x28000
	pd[0].p = 1;
	pd[0].read_write = 1;
	pd[0].user_supervisor = 1;
	pd[0].plvl_writethr = ;
	pd[0].plvl_cachedisable = ;
	pd[0].accesed = ;
	pd[0].ignored = ;
	pd[0].pagesize = ;
	pd[0].global = ;
	pd[0].disponible = ;
	pd[0].direccion = 0x28000;

	for (int i = 1; i < 1024; ++i)
	{
		pd[i].p = 0;
	}

	for (int i = 0; i < 1024; ++i)
	{
		pt[i] = i;
	}	

}






