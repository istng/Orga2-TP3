/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"

// La esctructura tipo_zombie tiene como objetivo formar una lista circular con 
// los tres tipos de zombies para que resulte más sencillo cambiar el tipo de zombie
// seleccionado por el jugador
// A su vez, info_jugador va a tener entre sus miembros un puntero a esta estructura

typedef struct str_tipo_zombie {
	char ascii;
	struct tipo_zombie *siguiente_tipo_zombie;
} tipo_zombie;


typedef struct str_info_jugador {
	unsigned int	jugador:1;				//hay dos jugadores
	unsigned int 	pos:11;					//tamaño abritrario pero suficiente
	unsigned int 	puntos:11;				//tamaño abritrario pero suficiente
	unsigned int 	zombies_usados:4; 	    //hay 8 zombies posibles por jugador
	tipo_zombie *zombie_seleccionado;		//hay 3 tipos de zombies
	unsigned int 	ultimo_zombie:3;		//indice del ultimo zombie usado

} info_jugador;


typedef struct info_zombie {
	unsigned int	jugador:1;		//hay dos jugadores
	unsigned int 	i:10;			//filas, tamaño abritrario pero suficiente
	unsigned int 	j:11;			//columnas, tamaño abritrario pero suficiente
	unsigned int    tipo_zombie:2;  //hay 3 tipos de zombies, y 0 representa que no esta en juego
									//llegamos a 24!

} __attribute__((__packed__, aligned (4))) info_zombie;

tipo_zombie zombie_guerrero;
tipo_zombie zombie_mago;
tipo_zombie zombie_clerigo;
info_jugador A; 
info_jugador B;
info_zombie zombiesA[8];
info_zombie zombiesB[8];

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void game_jugador_mover(jugador jug, jugador_direccion dir);

void game_lanzar_zombi(unsigned int jugador);

void game_move_current_zombi(direccion dir);


#endif  /* !__GAME_H__ */
