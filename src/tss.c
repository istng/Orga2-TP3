/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"

tss tss_inicial;
tss tss_idle;

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];




void tss_inicializar() {
	tss_idle.eip    = 0x16000;
	tss_idle.esp    = 0x27000;
	tss_idle.ebp    = 0x27000;
	tss_idle.cs     =  8<<3;
	tss_idle.ss     = 10<<3;
	tss_idle.ds     = 10<<3;
	tss_idle.es     = 10<<3;
	tss_idle.gs     = 10<<3;
	tss_idle.fs     = 12<<3;
	tss_idle.cr3    = 0x27000;
	tss_idle.eflags = 0x00000202;



    gdt[TSS_Inicial].base_0_15 = (((int)&tss_inicial)<<16)>>16;
    gdt[TSS_Inicial].base_23_16 = (((int)&tss_inicial)<<8)>>24;
    gdt[TSS_Inicial].base_31_24 = ((int)&tss_inicial)>>24;

    gdt[TSS_Idle].base_0_15 = (((int)&tss_idle)<<16)>>16;
    gdt[TSS_Idle].base_23_16 = (((int)&tss_idle)<<8)>>24;
    gdt[TSS_Idle].base_31_24 = ((int)&tss_idle)>>24;

}

void tss_zombies(){
	unsigned int i;

	for (i = 0; i < 8; ++i)
	{


		tss_zombisA[i].eip    = 0x8000000;
		tss_zombisA[i].esp    = 0x8001000;
		tss_zombisA[i].ebp    = 0x8001000;
		tss_zombisA[i].cs     =  9<<3;
		tss_zombisA[i].ss     = 11<<3;
		tss_zombisA[i].ss0    = 10<<3;
		tss_zombisA[i].ds     = 11<<3;
		tss_zombisA[i].es     = 11<<3;
		tss_zombisA[i].gs     = 11<<3;
		tss_zombisA[i].fs     = 12<<3;
		tss_zombisA[i].cr3    = 0x09000000; // lo inicializamos con fruta
		tss_zombisA[i].eflags = 0x00000202;
		tss_zombisA[i].esp0 = mmu_proxima_pagina_fisica_libre();

	    gdt[TSS_A+i].base_0_15 = (((int)&tss_zombisA)<<16)>>16;
	    gdt[TSS_A+i].base_23_16 = (((int)&tss_zombisA)<<8)>>24;
    	gdt[TSS_A+i].base_31_24 = ((int)&tss_zombisA)>>24;


    tss_zombisB[i].eip    = 0x8000000;
		tss_zombisB[i].esp    = 0x8001000;
		tss_zombisB[i].ebp    = 0x8001000;
		tss_zombisB[i].cs     =  9<<3;
		tss_zombisB[i].ss     = 11<<3;
		tss_zombisB[i].ss0    = 10<<3;
		tss_zombisB[i].ds     = 11<<3;
		tss_zombisB[i].es     = 11<<3;
		tss_zombisB[i].gs     = 11<<3;
		tss_zombisB[i].fs     = 12<<3;
		tss_zombisB[i].cr3    = 0x09000000; // lo inicializamos con fruta
		tss_zombisB[i].eflags = 0x00000202;
		tss_zombisB[i].esp0 = mmu_proxima_pagina_fisica_libre();

	    gdt[TSS_B+i].base_0_15 = (((int)&tss_zombisB)<<16)>>16;
	    gdt[TSS_B+i].base_23_16 = (((int)&tss_zombisB)<<8)>>24;
    	gdt[TSS_B+i].base_31_24 = ((int)&tss_zombisB)>>24;
	}
}

void tss_inicializar_zombi(jugador jug,unsigned int indice, unsigned int cr3){

	switch (jug) {
		case JUGADOR_A:
			tss_zombisA[indice].cr3 = cr3;
		case JUGADOR_B:
			tss_zombisB[indice].cr3 = cr3;
	}

}
