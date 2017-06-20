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

unsigned short sched_proximo_indice() {

	//cero significa no hago nada, no hace falta tss_idle
	unsigned short res = 0;


	switch(sched.siguiente_jugador){
		case JUGADOR_A:
			if(A.zombies_usados > 0){
				unsigned short i;
				for (i=1;i<=8;i++){
					unsigned short indice = (A.ultimo_zombie + i) % 7;
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
					unsigned short indice = (B.ultimo_zombie + i) % 7;
					if(zombiesA[indice].estado == ACTIVO){
						res = TSS_B + indice;
						sched.tareaActual_B = res;
						break;
					}
				}
			}

			sched.siguiente_jugador = JUGADOR_A;
			break;
	}

	print_int(sched.siguiente_jugador == JUGADOR_A ? 0 : 1,10,8,30);
	print_int(sched.tareaActual_B,10,9,30);
	print_int(sched.tareaActual_A,10,10,30);

	


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
