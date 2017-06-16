/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

info_jugador A; 
info_jugador B;
info_zombie zombiesA[8];
info_zombie zombiesB[8];


void inicializar_variables_juego(){
	A.jugador = 0;
	A.pos = 20;
	A.puntos = 0;
	A.zombies_usados = 0;
	A.zombie_seleccionado = 0;
	A.ultimo_zombie = 0;
	
	
	B.jugador = 1;
	B.pos = 20;
	B.puntos = 0;
	B.zombies_usados = 0;
	B.zombie_seleccionado = 0;
	B.ultimo_zombie = 0;


	for (int i = 0; i < 8; ++i){
		zombiesA[i].tipo_zombie = 0; //es decir, no hay zombie
		zombiesB[i].tipo_zombie = 0;
	}

}








void game_jugador_mover(unsigned int jugador, unsigned int value) {
}
void game_lanzar_zombi(unsigned int jugador) {
}
void game_move_current_zombi(direccion dir) {
}

