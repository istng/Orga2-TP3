/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "tss.h"
#include "sched.h"

info_jugador A;
info_jugador B;
info_zombie zombiesA[CANT_ZOMBIS];
info_zombie zombiesB[CANT_ZOMBIS];

nodo_zombie zombie_guerrero = { (zombie_tipo) GUERRERO,
								(char) 'G',
								(struct str_nodo_zombie*) &zombie_mago,
								(struct str_nodo_zombie*) &zombie_clerigo};
nodo_zombie zombie_mago 	= { (zombie_tipo) MAGO,
								(char) 'M',
								(struct str_nodo_zombie*) &zombie_clerigo,
								(struct str_nodo_zombie*) &zombie_guerrero};
nodo_zombie zombie_clerigo 	= { (zombie_tipo) CLERIGO,
								(char) 'C',
								(struct str_nodo_zombie*) &zombie_guerrero,
								(struct str_nodo_zombie*) &zombie_mago};


void inicializar_variables_juego(){
	A.jug = JUGADOR_A;
	A.pos = 20;
	A.puntos = 0;
	A.zombies_usados = 0;
	A.zombie_seleccionado = &zombie_guerrero;
	A.ultimo_zombie = 0;


	B.jug = JUGADOR_B;
	B.pos = 20;
	B.puntos = 0;
	B.zombies_usados = 0;
	B.zombie_seleccionado = &zombie_mago;
	B.ultimo_zombie = 0;

	int i;
	for (i = 0; i < 8; ++i){
		zombiesA[i].estado = INACTIVO; //es decir, no hay zombie
		zombiesB[i].estado = INACTIVO;
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

void game_jugador_cambiar_zombie(jugador jug, jugador_zombie_lista dir){
	switch(jug){
		case JUGADOR_A:
			switch(dir){
				case ANTERIOR:
					A.zombie_seleccionado = (A.zombie_seleccionado)->anterior_nodo_zombie;
					break;
				case SIGUIENTE:
					A.zombie_seleccionado = A.zombie_seleccionado->siguiente_nodo_zombie;
					break;
			}
			break;
		case JUGADOR_B:
			switch(dir){
				case ANTERIOR:
					B.zombie_seleccionado = B.zombie_seleccionado->anterior_nodo_zombie;
					break;
				case SIGUIENTE:
					B.zombie_seleccionado = B.zombie_seleccionado->siguiente_nodo_zombie;
					break;
			}
			break;
	}
}

// La función game_lanzar_zombi se va encargar de:
// *Colocar (copiar) el zombie (el código del progrma) en el mapa
// *Mapear a la dirección del eip (y siguientes) de la TSS las posiciones en el mapa donde se
//  encuentra el zombie (y su entorno)
// *Marcar el zombie como ACTIVO para que el scheduler le de tiempo de ejecución

// Sera llamada del scheduler y devolvera una


void game_lanzar_zombi(jugador jug) {


	info_jugador *info_jug = jug == JUGADOR_A ? &A : &B;
	unsigned short indiceZombie = info_jug->zombies_usados;
	// Esta linae de abajo no la entendí
	//unsigned short indiceZombie = info_jug->zombies_usados == 0 ? 0 : info_jug->zombies_usados + 1;
	info_zombie* zombie =  jug == JUGADOR_A ? &zombiesA[indiceZombie] : &zombiesB[indiceZombie];
	char tipoZombie = info_jug->zombie_seleccionado->ascii;

	// inicializamos el directorio de zombie y copiamos su codigo
	unsigned int dir_pd  = mmu_inicializar_dir_zombi(jug,tipoZombie);


	tss_inicializar_zombi(jug, indiceZombie, dir_pd);

	// inicializamos el zombie y lo marcamos como activo
	switch (tipoZombie) {
		case 'G':
			zombie->tipo = GUERRERO;
			break;
		case 'C':
			zombie->tipo = CLERIGO;
			break;
		case 'M':
			zombie->tipo = MAGO;
			break;
	}

	zombie->jug = info_jug->jug;
	zombie->estado = ACTIVO;
	zombie->i = info_jug->pos;
	zombie->j = jug == JUGADOR_A ? 1 : SIZE_W;

	// Lo pintamos en el mapa
	print_zombi(jug,zombie->i,zombie->j);

	// Por último modificamos la info del jugador
	info_jug->zombies_usados = (info_jug->zombies_usados) + 1;
	info_jug->ultimo_zombie = indiceZombie;
	
}


void game_move_current_zombi(direccion dir) {

	/*jugador jug = jugadorActual();



	desmapear_entorno_zombie(unsigned int i, unsigned int j, jugador jug, unsigned int dir_pd )
	switch(dir){
		case 0x83D:

	}


	
	*/
}

