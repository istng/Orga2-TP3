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


void mmu_mappear_pagina(unsigned int virtual, unsigned int dir_pd, unsigned int fisica, unsigned int user_lvl){

	unsigned int offset_directorio = virtual>>22;
	unsigned int offset_tabla = (virtual<<10)>>22;
	//unsigned int i;

	pde_entry* pd = (pde_entry*)dir_pd;
	pte_entry* pt;


	breakpoint();
	// (1) Nesesitamos la tabla de paginas:
	if(pd[offset_directorio].present){
		// Si estaba simplemente obtenemos direccion de la tabla de paginas en el directorio
		pt = (pte_entry*) (pd[offset_directorio].direccion << 12);
		breakpoint();
	}
	else{
		breakpoint();
		// Si no estaba agregamos una nueva entrada al directorio
		unsigned int pagina_libre = mmu_proxima_pagina_fisica_libre(); // Pedimos una pagina para alocar la tabla
		pd[offset_directorio].present = 1;
		pd[offset_directorio].read_write = 1;
		pd[offset_directorio].user_supervisor = user_lvl;
		pd[offset_directorio].plvl_writethr = 0;
		pd[offset_directorio].plvl_cachedisable = 0;
		pd[offset_directorio].accesed = 0;
		pd[offset_directorio].ignored = 0;
		pd[offset_directorio].pagesize = 0;
		pd[offset_directorio].global = 0;
		pd[offset_directorio].disponible = 0;
		pd[offset_directorio].direccion = pagina_libre >> 12; //20 bits altos de la direccion donde se encuentra a pde

		pt = (pte_entry*) pagina_libre;
		// Inicializamos la tabla
		//breakpoint();
		/*for (i = 0; i < 1024; ++i){
			pt[i].present = 0;
		}*/
	}

	print_hex((unsigned int)pt,8,10,10,20);
	// (2) Buscamos la entrada de esa pagina en la tabla
	if (pt[offset_tabla].present){
		// Si la entrada ya existia solo remapeamos la direccion pasada por parametro
		pt[offset_tabla].direccion = fisica>>12;
	}
	else{
		// Si no ...

		pt[offset_tabla].present = 1;
		pt[offset_tabla].read_write = 1;
		pt[offset_tabla].user_supervisor = user_lvl;
		pt[offset_tabla].plvl_writethr = 0;
		pt[offset_tabla].plvl_cachedisable = 0;
		pt[offset_tabla].accesed = 0;
		pt[offset_tabla].dirty_bit = 0;
		pt[offset_tabla].page_table_attribute_index = 0;
		pt[offset_tabla].global = 0;
		pt[offset_tabla].disponible = 0;
		pt[offset_tabla].direccion = fisica>>12;
	}

}


/*
void mmu_mappear_pagina(unsigned int virtual, unsigned int dir_pd, unsigned int fisica){

	unsigned int offset_directorio = virtual>>22;
	unsigned int offset_tabla = (virtual<<10)>>22;
	unsigned int i;

	pde_entry* pd = (pde_entry*)dir_pd;
	pte_entry* pt;


	// (1) Nesesitamos la tabla de paginas:
	if(pd[offset_directorio].present){
		// Si estaba simplemente obtenemos direccion de la tabla de paginas en el directorio
		pt = (pte_entry*) (pd[offset_directorio].direccion << 12);
	}
	else{
		// Si no estaba agregamos una nueva entrada al directorio
		unsigned int pagina_libre = mmu_proxima_pagina_fisica_libre(); // Pedimos una pagina para alocar la tabla
		pd[offset_directorio].present = 1;
		pd[offset_directorio].read_write = 1;
		pd[offset_directorio].user_supervisor = 0;
		pd[offset_directorio].plvl_writethr = 0;
		pd[offset_directorio].plvl_cachedisable = 0;
		pd[offset_directorio].accesed = 0;
		pd[offset_directorio].ignored = 0;
		pd[offset_directorio].pagesize = 0;
		pd[offset_directorio].global = 0;
		pd[offset_directorio].disponible = 0;
		pd[offset_directorio].direccion = pagina_libre >> 12; //20 bits altos de la direccion donde se encuentra a pde

		pt = (pte_entry*) pagina_libre;
		// Inicializamos la tabla
		for (i = 0; i < 1024; ++i){
			pt[i].present = 0;
		}


	}
	// (2) Buscamos la entrada de esa pagina en la tabla
	if (pt[offset_tabla].present){
		// Si la entrada ya existia solo remapeamos la direccion pasada por parametro
		pt[offset_tabla].direccion = fisica>>12;
	}
	else{
		// Si no ...
		pt[offset_tabla].present = 1;
		pt[offset_tabla].read_write = 1;
		// La gracia de esta funcion : PRIVILEGIOS
		// -> tienen privilegios de user pues son de los zombies
		pt[offset_tabla].user_supervisor = 0;
		pt[offset_tabla].plvl_writethr = 0;
		pt[offset_tabla].plvl_cachedisable = 0;
		pt[offset_tabla].accesed = 0;
		pt[offset_tabla].dirty_bit = 0;
		pt[offset_tabla].page_table_attribute_index = 0;
		pt[offset_tabla].global = 0;
		pt[offset_tabla].disponible = 0;
		pt[offset_tabla].direccion = fisica>>12;
	}

}*/

void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){

	unsigned int offset_directorio = virtual>>22;
	unsigned int offset_tabla = (virtual<<10)>>22;

	pde_entry* pd = (pde_entry*)cr3;
	pte_entry* pt = (pte_entry*) (unsigned int)(pd[offset_directorio].direccion);

	pt[offset_tabla].present = 0;
}


void mmu_inicializar_dir_kernel() {

	mmu_inicializar();

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








/******************************************************************************/







unsigned int pos_a_dirMapa(unsigned int i, unsigned int j){

	unsigned int posMapa = 0x400000;
	unsigned int tam_pagina = 0x1000;

	return posMapa + tam_pagina * VIDEO_COLS * i + tam_pagina * j;

}

void mappear_entorno_zombi(unsigned int i, unsigned int j, int jugador, unsigned int dir_pd ){

	// Precondicion: el zombi no se mueve hacia la columna donde esta el jugador contrario.

	unsigned int indices[9][2];
	indices[0][0]=i;
	indices[0][1]=j;

	if (jugador){
		// El movimiento lo realiza el jugador A



		//Pagina 2:
		indices[1][0] = i;
		indices[1][1] = j+1;
		//Pagina 3:
		indices[2][0] = i+1 == 1 ? SIZE_H-1 : i+1;
		indices[2][1] = j+1;
		//Pagina 4:
		indices[3][0] = i-1 == SIZE_H ? 1 : i-1;
		indices[3][1] = j+1;
		//Pagina 5:
		indices[4][0] = i+1 == 1 ? SIZE_H-1 : i+1;
		indices[4][1] = j;
		//Pagina 6:
		indices[5][0] = i-1 == SIZE_H ? 1 : i-1;
		indices[5][1] = j;
		//Pagina 7:
		indices[6][0] = i;
		indices[6][1] = j - 1;
		//Pagina 8:
		indices[7][0] = i-1 == SIZE_H ? 1 : i-1;
		indices[7][1] = j-1;
		//Pagina 9:
		indices[8][0] = i+1 == SIZE_H ? 1 : i+1;
		indices[8][1] = j-1;

	}
	else{
		// El movimiento lo realiza el jugador B

		//Pagina 2:
		indices[1][0] = i;
		indices[1][1] = j-1;
		//Pagina 3:
		indices[2][0] = i-1 == SIZE_H ? 1 : i-1;
		indices[2][1] = j-1;
		//Pagina 4:
		indices[3][0] = i+1 == SIZE_H ? 1 : i+1;
		indices[3][1] = j-1;
		//Pagina 5:
		indices[4][0] = i-1 == SIZE_H ? 1 : i-1;
		indices[4][1] = j;
		//Pagina 6:
		indices[5][0] = i+1 == 1 ? SIZE_H-1 : i+1;
		indices[5][1] = j;
		//Pagina 7:
		indices[6][0] = i;
		indices[6][1] = j + 1;
		//Pagina 8:
		indices[7][0] = i+1 == 1 ? SIZE_H-1 : i+1;
		indices[7][1] = j+1;
		//Pagina 9:
		indices[8][0] = i-1 == SIZE_H ? 1 : i-1;
		indices[8][1] = j+1;

	}


	int k;
	for (k = 0; k < 9;++k){
		unsigned int pag_virtual = DIR_TAREAS + k * PAGE_SIZE;
		unsigned int pag_fisica = pos_a_dirMapa(indices[k][0],indices[k][1]);
		print_int(k,1,1,20);
		mmu_mappear_pagina(pag_virtual,dir_pd,pag_fisica,1);

	}

}


// Esta funcion crea el directorio y las tablas asociadas a este. Luego devuelve la direccion del directorio.

unsigned int mmu_inicializar_dir_zombi(unsigned int i, unsigned int j, int jugador){
	// Inicializamos un directorio para la tarea
 	pde_entry* pd = (pde_entry*) mmu_proxima_pagina_fisica_libre();

	int k;
	for (k = 0; k < 1024; ++k){
		pd[k].present = 0;
	}


	// mapeamos el entorno del zombie
	mappear_entorno_zombi(i,j,jugador,(unsigned int)pd);

	return (unsigned int) pd;

}
