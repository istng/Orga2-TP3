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
	tss_idle.cs     = GDT_CODIGO_0<<3;
	tss_idle.ss     = GDT_DATA_0<<3;
	tss_idle.ds     = GDT_DATA_0<<3;
	tss_idle.es     = GDT_DATA_0<<3;
	tss_idle.gs     = GDT_DATA_0<<3;
	tss_idle.fs     = GDT_VIDEO<<3;
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
	
	// Se define una entra de gdt generica para se uitilizada por las tss
	gdt_entry gdt_entry_tss = {
        (unsigned short)    0x0067,         /* limit[0:15s]  */ 
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */ 
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl privilegios*/
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    };

	for (i = 0; i < 8; ++i)
	{
		tss_zombisA[i].eip    = 0x8000000;
		tss_zombisA[i].esp    = 0x8001000;
		tss_zombisA[i].ebp    = 0x8001000;
		tss_zombisA[i].cs     = GDT_CODIGO_3<<3;
		tss_zombisA[i].ss     = GDT_DATA_3<<3;
		tss_zombisA[i].ss0    = GDT_DATA_0<<3;
		tss_zombisA[i].ds     = GDT_DATA_3<<3;
		tss_zombisA[i].es     = GDT_DATA_3<<3;
		tss_zombisA[i].gs     = GDT_DATA_3<<3;
		tss_zombisA[i].fs     = GDT_VIDEO<<3;
		tss_zombisA[i].cr3    = 0x09000000; // lo inicializamos con fruta
		tss_zombisA[i].eflags = 0x00000202;
		tss_zombisA[i].esp0	  = mmu_proxima_pagina_fisica_libre();

		gdt[TSS_A+i] = gdt_entry_tss;
	    gdt[TSS_A+i].base_0_15  = (((int)&tss_zombisA[i])<<16)>>16;
	    gdt[TSS_A+i].base_23_16 = (((int)&tss_zombisA[i])<<8)>>24;
    	gdt[TSS_A+i].base_31_24 = ((int)&tss_zombisA[i])>>24;


    	tss_zombisB[i].eip    = 0x8000000;
		tss_zombisB[i].esp    = 0x8001000;
		tss_zombisB[i].ebp    = 0x8001000;
		tss_zombisB[i].cs     = GDT_CODIGO_3<<3;
		tss_zombisB[i].ss     = GDT_DATA_3<<3;
		tss_zombisB[i].ss0    = GDT_DATA_0<<3;
		tss_zombisB[i].ds     = GDT_DATA_3<<3;
		tss_zombisB[i].es     = GDT_DATA_3<<3;
		tss_zombisB[i].gs     = GDT_DATA_3<<3;
		tss_zombisB[i].fs     = GDT_VIDEO<<3;
		tss_zombisB[i].cr3    = 0x09000000; // lo inicializamos con fruta
		tss_zombisB[i].eflags = 0x00000202;
		tss_zombisB[i].esp0   = mmu_proxima_pagina_fisica_libre();

		gdt[TSS_B+i] = gdt_entry_tss;
	    gdt[TSS_B+i].base_0_15  = (((int)&tss_zombisB[i])<<16)>>16;
	    gdt[TSS_B+i].base_23_16 = (((int)&tss_zombisB[i])<<8)>>24;
    	gdt[TSS_B+i].base_31_24 = ((int)&tss_zombisB[i])>>24;
	}
}

void tss_inicializar_zombi(jugador jug,unsigned int indice, unsigned int cr3){

	switch (jug) {
		case JUGADOR_A:
			tss_zombisA[indice].cr3 = cr3;
			break;
		case JUGADOR_B:
			tss_zombisB[indice].cr3 = cr3;
			break;
	}
}
