/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "game.h"

unsigned int siguiente_jugador;

unsigned short sched_proximo_indice() {

	unsigned short res;

	switch(siguiente_jugador){
		case 0:
			res = A.zombies_usados == 0 ? 0 : TSS_A + A.ultimo_zombie;
			if(A.zombies_usados > 0){
				A.ultimo_zombie = (A.ultimo_zombie + 1) % A.zombies_usados;
			}
			siguiente_jugador = 1;
			break;
		case 1:
			res = B.zombies_usados == 0 ? 0 : TSS_B + B.ultimo_zombie;
			if(B.zombies_usados > 0){
				B.ultimo_zombie = (B.ultimo_zombie + 1) % B.zombies_usados;
			}
			siguiente_jugador = 0;
			break;
	}



	return res;

}

