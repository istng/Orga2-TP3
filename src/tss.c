/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

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

void tss_AG(unsigned int indice){




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


    gdt[TSS_Idle].base_0_15 = (((int)&tss_idle)<<16)>>16;
    gdt[TSS_Idle].base_23_16 = (((int)&tss_idle)<<8)>>24;
    gdt[TSS_Idle].base_31_24 = ((int)&tss_idle)>>24;


}
