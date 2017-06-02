/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/












#include "mmu.h"


unsigned int proxima_pagina_libre;

void mmu_inicializar() {
	proxima_pagina_libre = INICIO_PAGINAS_LIBRES;
}

unsigned int mmu_proxima_pagina_fisica_libre() {
	unsigned int pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return pagina_libre;
}

void mmu_mappear_pagina(unsigned int virtual,unsigned int dir_pd,unsigned int fisica){
	
	unsigned int offset_directorio = virtual>>22;
	unsigned int offset_tabla = (virtual<<10)>>22;
	unsigned int pagina_libre = mmu_proxima_pagina_fisica_libre(); //??????????????????

	pde_entry* pd = (pde_entry*)dir_pd;
	pte_entry* pt = (pte_entry*)pagina_libre;


	pd[offset_directorio].present = 1;
	pd[offset_directorio].read_write = 1;
	pd[offset_directorio].user_supervisor = 0; 
	pd[offset_directorio].plvl_writethr = 0;
	pd[offset_directorio].plvl_cachedisable = 0;
	pd[offset_directorio].accesed = 0;
	pd[offset_directorio].ignored = 0;
	pd[offset_directorio].pagesize = 0;
	pd[offset_directorio].global = 0; 
	pd[offset_directorio].disponible = 1;
	pd[offset_directorio].direccion = pagina_libre >> 12; //20 bits altos de la direccion donde se encuentra a pde


	// Todas las entradas de la tabla de paginas no estan presentes 
	// excepto la entrada correspondiente a la pagina fisica de la 
	// direccion que se pasa como parametro.
	for (i = 0; i < 1024; ++i)
	{
		pt[i].present = 0;
	}

	pt[offset_tabla].present = 1;
	pt[offset_tabla].read_write = 1;
	pt[offset_tabla].user_supervisor = 0; ///??????
	pt[offset_tabla].plvl_writethr = 0;
	pt[offset_tabla].plvl_cachedisable = 0;
	pt[offset_tabla].accesed = 0;
	pt[offset_tabla].dirty_bit = 0;
	pt[offset_tabla].page_table_attribute_index = 0;
	pt[offset_tabla].global = 0; 
	pt[offset_tabla].disponible = 0; //??????????????????????????????????????????????????????
	pt[offset_tabla].direccion = fisica>>12;
}


void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){

	unsigned int offset_directorio = virtual>>22;
	unsigned int offset_tabla = (virtual<<10)>>22;

	pde_entry* pd = (pde_entry*)cr3;
	pte_entry* pt = (pte_entry*)pd[offset_directorio];

	pt[offset_tabla].present = 0;
}




void mmu_inicializar_dir_kernel() {

	pde_entry* pd = (pde_entry*)0x27000;
	pte_entry* pt = (pte_entry*)0x28000;

	pd[0].present = 1;
	pd[0].read_write = 1;
	pd[0].user_supervisor = 0; ///??????
	pd[0].plvl_writethr = 0;
	pd[0].plvl_cachedisable = 0;
	pd[0].accesed = 0;
	pd[0].ignored = 0;
	pd[0].pagesize = 0;
	pd[0].global = 0; 
	pd[0].disponible = 0; //??????????????????????????????????????????????????????
	pd[0].direccion = 0x28000 >> 12; //20 bits altos de la direccion donde se encuentra a pde

	

	int i;for (i = 1; i < 1024; ++i)
	{
		pd[i].present = 0;
	}

	for (i = 0; i < 1024; ++i)
	{
		pt[i].present = 1;
		pt[i].read_write = 1;
		pt[i].user_supervisor = 0; ///??????
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




