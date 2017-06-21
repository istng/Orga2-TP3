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
	
	if(hay_zoombies_activos(sched.siguiente_jugador)){
		sig_zoombie = indice_siguiente_zoombie_activo(sched.siguiente_jugador, *tarea_actual);
		*tarea_actual = sig_zoombie;
		res = sig_zoombie + TSS_actual;
	} 
	cambiar_siguiente_jugador();

	return res;
}

/*
unsigned short sched_proximo_indice() {

	//cero significa no hago nada, no hace falta tss_idle
	unsigned short res = 0;



	switch(sched.siguiente_jugador){
		case JUGADOR_A:
			if(A.zombies_usados > 0){
				unsigned short i;
				for (i=1;i<=8;i++){
					unsigned short indice = (sched.tareaActual_A + i) % 8;
					if(zombiesA[indice].estado == ACTIVO){
						res = TSS_A + indice;
						sched.tareaActual_A = res;
						break;
					}
				}
			}



			sched.siguiente_jugador = JUGADOR_B;
			break;
		case JUGADOR_B:
			if(B.zombies_usados > 0){
				unsigned short i;
				for (i=1;i<=8;i++){
					unsigned short indice = (sched.tareaActual_B + i) % 8;
					if(zombiesB[indice].estado == ACTIVO){
						res = TSS_B + indice;
						sched.tareaActual_B = res;
						break;
					}
				}
			}

			sched.siguiente_jugador = JUGADOR_A;
			break;

			breakpoint();
	}

	print_int(sched.siguiente_jugador == JUGADOR_A ? 0 : 1,10,8,30);
	print_int(sched.tareaActual_B,10,9,30);
	print_int(sched.tareaActual_A,10,10,30);

	//breakpoint();



	return res;

}
*/


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
