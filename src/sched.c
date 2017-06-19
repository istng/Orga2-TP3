/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "game.h"

jugador siguiente_jugador = JUGADOR_A;

unsigned short sched_proximo_indice() {

	unsigned short res = 0;
	breakpoint();

	switch(siguiente_jugador){
		case JUGADOR_A:
			//res = A.zombies_usados == 0 ? 0 : TSS_A;// + A.ultimo_zombie - 1;
			//if(A.zombies_usados > 0){
			//	A.ultimo_zombie = (A.ultimo_zombie + 1) % A.zombies_usados;
			//}
			if(A.zombies_usados > 0){
				res = TSS_A + A.ultimo_zombie - 1;
				breakpoint();
			}
			siguiente_jugador = JUGADOR_B;
			break;
		case JUGADOR_B:
			//res = B.zombies_usados == 0 ? 0 : TSS_B;// + B.ultimo_zombie - 1;
			//if(B.zombies_usados > 0){
			//	B.ultimo_zombie = (B.ultimo_zombie + 1) % B.zombies_usados;
			//}
			if(B.zombies_usados > 0){
				res = TSS_B + B.ultimo_zombie - 1;
				breakpoint();
			}
			siguiente_jugador = JUGADOR_A;
			break;
	}



	return res;

}
