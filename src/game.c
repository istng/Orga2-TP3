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

tipo_zombie zombie_guerrero = { (char) 'G', (struct tipo_zombie*) &zombie_mago};
tipo_zombie zombie_mago 	= { (char) 'M', (struct tipo_zombie*) &zombie_clerigo};
tipo_zombie zombie_clerigo 	= { (char) 'C', (struct tipo_zombie*) &zombie_guerrero};


void inicializar_variables_juego(){
	A.jugador = 0;
	A.pos = 20;
	A.puntos = 0;
	A.zombies_usados = 0;
	A.zombie_seleccionado = &zombie_guerrero;
	A.ultimo_zombie = 0;
	
	
	B.jugador = 1;
	B.pos = 20;
	B.puntos = 0;
	B.zombies_usados = 0;
	B.zombie_seleccionado = &zombie_mago;
	B.ultimo_zombie = 0;


	for (int i = 0; i < 8; ++i){
		zombiesA[i].tipo_zombie = 0; //es decir, no hay zombie
		zombiesB[i].tipo_zombie = 0;
	}

}



void game_jugador_mover(jugador jug, jugador_direccion dir) {
	switch(jug){
		case JUGADOR_A:
			switch(dir){
				case ARRIBA:
					A.pos = A.pos > 1 ? A.pos-1 : SIZE_H;
					break;
				case ABAJO:
					A.pos = A.pos < SIZE_H ? A.pos+1 : 1;
					break;
			}
			break;
		case JUGADOR_B:
			switch(dir){
				case ARRIBA:
					B.pos = B.pos > 1 ? B.pos-1 : SIZE_H;
					break;
				case ABAJO:
					B.pos = B.pos < SIZE_H ? B.pos+1 : 1;
					break;
			}
			break;
	}
}
void game_lanzar_zombi(unsigned int jugador) {
}
void game_move_current_zombi(direccion dir) {
}
