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

	tlbflush();

}




void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){

	unsigned int offset_directorio = virtual>>22;
	unsigned int offset_tabla = (virtual<<10)>>22;
	unsigned int i;
	unsigned int cant_paginas_presentes_en_pt = 0;

	pde_entry* pd = (pde_entry*)cr3;
	pte_entry* pt = (pte_entry*) (unsigned int)(pd[offset_directorio].direccion);

	pt[offset_tabla].present = 0;

	for(i = 0; i < 1024; i++){
		if(pt[i].present != 0){
			cant_paginas_presentes_en_pt++;
		}
	}

	if(cant_paginas_presentes_en_pt == 0){
		pd[offset_directorio].present = 0;
	}

	tlbflush();
}


void mmu_inicializar_dir_kernel() {

	mmu_inicializar();

	pde_entry* pd = (pde_entry*)0x27000;

	identity_mapping((unsigned int)pd,0);

}

void identity_mapping(unsigned int directorio,unsigned int user_lvl){

	pde_entry* pd = (pde_entry*)directorio;
	pte_entry* pt = (pte_entry*)mmu_proxima_pagina_fisica_libre();

	pd[0].present = 1;
	pd[0].read_write = 1;
	pd[0].user_supervisor = user_lvl; ///??????
	pd[0].plvl_writethr = 0;
	pd[0].plvl_cachedisable = 0;
	pd[0].accesed = 0;
	pd[0].ignored = 0;
	pd[0].pagesize = 0;
	pd[0].global = 0;
	pd[0].disponible = 0; //??????????????????????????????????????????????????????
	pd[0].direccion = (unsigned int) pt >> 12; //20 bits altos de la direccion donde se encuentra a pte


	int i;
	for (i = 1; i < 1024; ++i)
	{
		pd[i].present = 0;
	}

	for (i = 0; i < 1024; ++i)
	{
		pt[i].present = 1;
		pt[i].read_write = 1;
		pt[i].user_supervisor = user_lvl; ///??????
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

void mappear_entorno_zombi(unsigned int i, unsigned int j, jugador jug, unsigned int dir_pd ){

	// Precondicion: el zombi no se mueve hacia la columna donde esta el jugador contrario.

	unsigned int indices[9][2];
	indices[0][0]= mod_mapa(i);
	indices[0][1]= j;

	if (jug == JUGADOR_A){
		// El movimiento lo realiza el jugador A



		//Pagina 2:
		indices[1][0] = mod_mapa(i);
		indices[1][1] = j+1;
		//Pagina 3:
		indices[2][0] = mod_mapa(i+1);
		indices[2][1] = j+1;
		//Pagina 4:
		indices[3][0] = mod_mapa(i-1);
		indices[3][1] = j+1;
		//Pagina 5:
		indices[4][0] = mod_mapa(i+1);
		indices[4][1] = j;
		//Pagina 6:
		indices[5][0] = mod_mapa(i-1);
		indices[5][1] = j;
		//Pagina 7:
		indices[6][0] = mod_mapa(i);
		indices[6][1] = j-1;
		//Pagina 8:
		indices[7][0] = mod_mapa(i-1);
		indices[7][1] = j-1;
		//Pagina 9:
		indices[8][0] = mod_mapa(i+1);
		indices[8][1] = j-1;

	}
	else{
		// El movimiento lo realiza el jugador B

		//Pagina 2:
		indices[1][0] = mod_mapa(i);
		indices[1][1] = j-1;
		//Pagina 3:
		indices[2][0] = mod_mapa(i-1);
		indices[2][1] = j-1;
		//Pagina 4:
		indices[3][0] = mod_mapa(i+1);
		indices[3][1] = j-1;
		//Pagina 5:
		indices[4][0] = mod_mapa(i-1);
		indices[4][1] = j;
		//Pagina 6:
		indices[5][0] = mod_mapa(i+1);
		indices[5][1] = j;
		//Pagina 7:
		indices[6][0] = mod_mapa(i);
		indices[6][1] = j + 1;
		//Pagina 8:
		indices[7][0] = mod_mapa(i+1);
		indices[7][1] = j+1;
		//Pagina 9:
		indices[8][0] = mod_mapa(i-1);
		indices[8][1] = j+1;

	}



	int k;
	for (k = 0; k < 9;++k){
		unsigned int pag_virtual = DIR_TAREAS + k * PAGE_SIZE;
		unsigned int pag_fisica = pos_a_dirMapa(indices[k][0],indices[k][1]);
		mmu_mappear_pagina(pag_virtual,dir_pd,pag_fisica, 1);//???????????????'
	}

}



unsigned int mod_mapa(unsigned int i){

	unsigned int res_i;
	if ( i > SIZE_H ){
		res_i = i % SIZE_H;
	}
	else if (i == 0){
		res_i = SIZE_H;
	}
	else {
		res_i = i;
	}
	return res_i;
}



// Esta funcion crea el directorio y las tablas asociadas a este. Luego devuelve la direccion del directorio.

unsigned int mmu_inicializar_dir_zombi(jugador jug,char tipo, unsigned short indice){
	// Inicializamos un directorio para la tarea
 	pde_entry* pd = (pde_entry*) obtener_cr3(jug, indice);
	identity_mapping((unsigned int)pd,1); //?????????????????????



	info_jugador juga =	jug == JUGADOR_A ? A : B;
	unsigned int i = juga.pos;
	unsigned int j = jug == JUGADOR_A ? 1 : SIZE_W;

	// Posición en el mapa en el cual se va a copiar el código del zombie
	char *posMem = (char *) pos_a_dirMapa(mod_mapa(i), j);//0x8000000;
	char *posCodigo;



	//Movemos el codigo del zombie
	switch (tipo) {
		case 'G':
			posCodigo = (char*) (jug == 0 ? 0x10000 : 0x13000);
			break;
		case 'M':
			posCodigo = (char*) (jug == 0 ? 0x11000 : 0x14000);
			break;
		case 'C':
			posCodigo = (char*) (jug == 0 ? 0x12000 : 0x15000);
			break;
	}

	// mappearmos el entorno del zombie

	mappear_entorno_zombi(i,j,jug,(unsigned int) pd);

	// La inicializamos en el scheduler

	// La página de memoria del mapa donde se va acopiar el código se mapea
	// de no hacerlo se obtiene un Page Fault
	mmu_mappear_pagina((unsigned int)posMem, rcr3(), (unsigned int)posMem, 1);

	// Copiamos el codigo
	//unsigned int cr3 = rcr3();
	//lcr3((unsigned int)pd);
	unsigned int k;
	for(k = 0; k<PAGE_SIZE ; k++){
		posMem[k] = posCodigo[k];
	}
	//lcr3(cr3);
	// desmapeamos la pagina
	mmu_unmapear_pagina((unsigned int)posMem, rcr3());

	return (unsigned int) pd;
}


void desmapear_entorno_zombie(unsigned int i, unsigned int j, unsigned int dir_pd ){

	// Precondicion: el zombi no se mueve hacia la columna donde esta el jugador contrario.


	unsigned int dir_inicial = 0x8000000;
	int k;
	for (k = 0; k < 9;++k){
		unsigned int pagina = dir_inicial + k * PAGE_SIZE;
		mmu_unmapear_pagina(pagina,dir_pd);
	}

}




void copiar_codigo_zombi(unsigned int i , unsigned int j, jugador jug, zombie_tipo tipo){


	// Posición en el mapa en el cual se va a copiar el código del zombie
	char *posMem = (char *) pos_a_dirMapa(mod_mapa(i), j);//0x8000000;
	char *posCodigo;

	//Movemos el codigo del zombie
	switch (tipo) {
		case GUERRERO:
			posCodigo = (char*) (jug == 0 ? 0x10000 : 0x13000);
			break;
		case CLERIGO:
			posCodigo = (char*) (jug == 0 ? 0x11000 : 0x14000);
			break;
		case MAGO:
			posCodigo = (char*) (jug == 0 ? 0x12000 : 0x15000);
			break;
	}

	// La página de memoria del mapa donde se va acopiar el código se mapea
	// de no hacerlo se obtiene un Page Fault
	mmu_mappear_pagina((unsigned int)posMem, rcr3(), (unsigned int)posMem, 1);

	// Copiamos el codigo
	//unsigned int cr3 = rcr3();
	//lcr3((unsigned int)pd);
	unsigned int k;
	for(k = 0; k<PAGE_SIZE ; k++){
		posMem[k] = posCodigo[k];
	}
	//lcr3(cr3);
	// desmapeamos la pagina
	mmu_unmapear_pagina((unsigned int)posMem, rcr3());


}


void cerea_esta_pila(unsigned int dir){
	char* pagina = (char*) ((dir>>4)<<4);

	if((unsigned int)pagina != dir){
		mmu_mappear_pagina((unsigned int)pagina,rcr3(),dir,1);
		unsigned int k;
		for (k=0;k<PAGE_SIZE;k++){
			pagina[k] = 0;
		}
		mmu_unmapear_pagina((unsigned int)pagina,rcr3());
	}
	

}



