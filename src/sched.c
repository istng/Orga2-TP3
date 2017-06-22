/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "game.h"

scheduler sched;

void sched_inicializar(){
	sched.siguiente_jugador = JUGADOR_A;
	sched.tareaActual_A = 0;
	sched.tareaActual_B = 0;
}


unsigned short sched_proximo_indice(){
	//cero significa no hago nada, no hace falta tss_idle
	unsigned short res = 0;
	unsigned short sig_zoombie = 0;
	unsigned short TSS_actual;
	unsigned short *tarea_actual;

	if(sched.siguiente_jugador == JUGADOR_A){
		TSS_actual = TSS_A;
		tarea_actual = (unsigned short*) &sched.tareaActual_A;
	} else {
		TSS_actual = TSS_B;
		tarea_actual = (unsigned short*) &sched.tareaActual_B;
	}

	if(hay_zombies_activos(sched.siguiente_jugador)){
		sig_zoombie = indice_siguiente_zombie_activo(sched.siguiente_jugador, *tarea_actual);
		*tarea_actual = sig_zoombie;
		res = sig_zoombie + TSS_actual;
	}
	cambiar_siguiente_jugador();
	unsigned int i = 0 + (unsigned int) res;
	print_int(1000,5,5,30);
	print_int(i,5,5,30);

	return res;
}




jugador jugadorActual(){
	if (sched.siguiente_jugador == JUGADOR_A)
	{
		return JUGADOR_B;
	}
	else{
		return JUGADOR_A;
	}
}

unsigned short tarea_actual_A(){
	return sched.tareaActual_A;
}


unsigned short tarea_actual_B(){
	return sched.tareaActual_B;
}


void cambiar_siguiente_jugador(){
	if (sched.siguiente_jugador == JUGADOR_A)
	{
		sched.siguiente_jugador = JUGADOR_B;
	}
	else{
		sched.siguiente_jugador = JUGADOR_A;
	}
}
