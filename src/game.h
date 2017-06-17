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

// La esctructura nodo_zombie tiene como objetivo formar una lista circular doblemente enlazada
// con los tres tipos de zombies para que resulte más sencillo cambiar el tipo de zombie
// seleccionado por el jugador
// A su vez, info_jugador va a tener entre sus miembros un puntero a esta estructura

typedef struct str_nodo_zombie {
	zombie_tipo tipo;
	char ascii;
	struct str_nodo_zombie *siguiente_nodo_zombie;
	struct str_nodo_zombie *anterior_nodo_zombie;
} nodo_zombie;


typedef struct str_info_jugador {
	jugador			jug;					//hay dos jugadores
	unsigned short 	pos;					//tamaño abritrario pero suficiente, altura del mapa, <=SIZE_H
	unsigned short 	puntos;					//tamaño abritrario pero suficiente, el primer jugador que suma 10 puntos gana
	unsigned short 	zombies_usados; 	    //hay 8 zombies posibles por jugador
	nodo_zombie 	*zombie_seleccionado;	//hay 3 tipos de zombies
	unsigned short 	ultimo_zombie;			//indice del ultimo zombie usado				
} info_jugador;


typedef struct info_zombie {
	jugador			jug;	//hay dos jugadores
	zombie_tipo		tipo;	//hay 3 tipos de zombies
	zombie_estado	estado;	//se considera ACTIVO si esta en juego e INACTIVO en caso contrario
	unsigned short 	i;		//filas, tamaño abritrario pero suficiente
	unsigned short 	j;		//columnas, tamaño abritrario pero suficiente
} info_zombie;

nodo_zombie zombie_guerrero;
nodo_zombie zombie_mago;
nodo_zombie zombie_clerigo;
info_jugador A;
info_jugador B;
info_zombie zombiesA[CANT_ZOMBIS];
info_zombie zombiesB[CANT_ZOMBIS];

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void game_jugador_mover(jugador jug, jugador_direccion dir);

void game_jugador_cambiar_zombie(jugador jug, jugador_zombie_lista dir);

void game_lanzar_zombi(jugador jug);
/*
void game_move_current_zombi(direccion dir);
*/

#endif  /* !__GAME_H__ */
