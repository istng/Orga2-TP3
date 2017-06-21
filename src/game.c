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


	B.jug = JUGADOR_B;
	B.pos = 20;
	B.puntos = 0;
	B.zombies_usados = 0;
	B.zombie_seleccionado = &zombie_mago;

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
	unsigned short indiceZombie = slot_libre(jug);
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
	// Con el objetivo de mover al zombie en el mapa,esta función realiza los siguientes pasos:
	// 1) Copia al zombie desde la posición (página) actual a la posición destino
	//	Tanto la página actual como la destino ya se encuentran mapeadas en el directorio de
	//	páginas del zombie (es una página del entorno del zombie). La posición de memoria del
	//	mapa donde se encuentra el zombie está mapeada a la dirección DIR_TAREAS, mientras que
	//	su entorno se encuentran mapeado a las direcciones DIR_TAREAS + i (i = 1..8), por lo
	//	tanto, hay que encontrar cual es la direccion DIR_TAREAS + i a la que se mueve.
	//	Tanto los movimientos como los números 'i' de las páginas de memoria del jugador B se
	//	encuentran espejados respecto al jugador A. Definimos conveniente una variable a la
	//	que llamamos 'orientación' para simplificar el código.

	jugador jugadorAct = jugadorActual();
	info_jugador* jug = jugadorAct == JUGADOR_A ? &A : &B;
	unsigned int indice = jugadorAct == JUGADOR_A ? tarea_actual_A() : tarea_actual_B();
	info_zombie* zombie =  jugadorAct == JUGADOR_A ? &zombiesA[indice] : &zombiesB[indice];

	// los movimientos de los jugadores son opuestos
	int orientacion = jug == JUGADOR_A ? -1 : 1;
	unsigned int pagina_destino_offset;
	unsigned int i,j;

	switch(dir){
		case ADE:
			i = zombie->i;
			j = zombie->j + orientacion;
			pagina_destino_offset = 1;
			break;
		case IZQ:
			i = mod_mapa(zombie->i - orientacion);
			j = zombie->j;
			pagina_destino_offset = 5;
			break;
		case DER:
			i = mod_mapa(zombie->i + orientacion);
			j = zombie->j;
			pagina_destino_offset = 4;
			break;
		case ATR:
			i = zombie->i;
			j = zombie->j - orientacion;
			pagina_destino_offset = 6;
			break;
	}

	//Movemos el codigo del zombie
	char *pagina_original = (char*) DIR_TAREAS;
	char *pagina_destino = (char*) (DIR_TAREAS + pagina_destino_offset * PAGE_SIZE);
	unsigned int k;
	for(k = 0; k<PAGE_SIZE ; k++){
		pagina_destino[k] = pagina_original[k];
	}

	breakpoint();


	desmapear_entorno_zombie(zombie->i, zombie->j,rcr3());
	mappear_entorno_zombi(i,j,jugadorAct,(unsigned int) rcr3());

	// Printemo la pantalla (: # )

	print_limpiar_pos_zombi(zombie->i, zombie->j);
	print_zombi(jugadorAct,i,j);

	// chequeamos si llego al final

	breakpoint();



	// Actualizos posicion zombi
	zombie->i = mod_mapa(i);
	zombie->j = j;


}



unsigned int hay_slot_libre(jugador jug){
	// devuelve 1 si el jugador tiene lugar para insertar un zombie, 0 si no.
	unsigned int i = 0;
	unsigned int res = 0;
	info_zombie* zombies =  jug == JUGADOR_A ? (info_zombie*) &zombiesA: ( info_zombie*) &zombiesB;
	for (i = 0; i<8; i++){
		if (zombies[i].estado == INACTIVO){
			return 1;
		}
	}
	return res;
}



unsigned short slot_libre(jugador jug){
	// Esta funcion nos da la proxima posicion libre el arreglo de zombies del jugador
	// donde podemos insertar un nuevo zombi

	unsigned int i;
	info_zombie* zombies =  jug == JUGADOR_A ? (info_zombie*) &zombiesA: (info_zombie*) &zombiesB;
	for (i = 0;i < 8 ;i++){
		if(zombies[i].estado == INACTIVO){
			return i;
		}
	}
	return 0;

}


info_zombie* obtener_arreglo_zoombies(jugador jug){
	info_zombie* zombies = jug == JUGADOR_A ? (info_zombie*) &zombiesA : (info_zombie*) &zombiesB;
	return zombies;
}


unsigned int hay_zoombies_activos(jugador jug){
	unsigned int res = FALSE;
	info_zombie *zombies = obtener_arreglo_zoombies(jug);
	unsigned short i;

	for(i = 0; i < CANT_ZOMBIS; i++){
		if(zombies[i].estado == ACTIVO){
			res = TRUE;
		}
	}

	return res;
}


unsigned short indice_siguiente_zoombie_activo(jugador jug, unsigned short indice){
	// Esta función devuelve el índice en el arreglo del siguiente zoombie activo del
	// jugador jug. 
	// En caso de alcanzar el límite del arreglo, empieza a buscar desde el principio.
	// En caso de que haya un solo zoombie activo, devuelve el mismo índice.
	// Esta función asume que hay_zoombies_activos devuelve TRUE

	unsigned short indice_sig_zoombie_act = indice;
	info_zombie *zombies = obtener_arreglo_zoombies(jug);
	unsigned short i;

	for(i = indice+1; i < CANT_ZOMBIS; i++){
		if(zombies[i].estado == ACTIVO){
			indice_sig_zoombie_act = i;
			break;
		}
	}

	if(indice_sig_zoombie_act == indice){
		for(i = 0; i < indice; i++){
			if(zombies[i].estado == ACTIVO){
				indice_sig_zoombie_act = i;
				break;
			}
		}
	}

	return indice_sig_zoombie_act;
}