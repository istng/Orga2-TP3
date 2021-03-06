/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "tss.h"
#include "sched.h"
#include "idt.h"
#include "isr.h"
#include "defines.h"

info_jugador A;
info_jugador B;
info_zombie zombiesA[CANT_ZOMBIS];
info_zombie zombiesB[CANT_ZOMBIS];
modo_debug estado_debug;
unsigned int hay_interrupcion_en_pantalla;

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
	A.zombies_lanzados = 0;
	A.zombie_seleccionado = &zombie_guerrero;

	B.jug = JUGADOR_B;
	B.pos = 20;
	B.puntos = 0;
	B.zombies_lanzados = 0;
	B.zombie_seleccionado = &zombie_mago;

	// seteamos modo debug
	estado_debug = DESACTIVADO;
	hay_interrupcion_en_pantalla = FALSE;


	int i;
	for (i = 0; i < 8; ++i){
		zombiesA[i].estado = INACTIVO; //es decir, no hay zombie
		zombiesA[i].contador_reloj = 0; //se inicializa el contador
		zombiesB[i].estado = INACTIVO;
		zombiesB[i].contador_reloj = 0;
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
	info_zombie* zombie =  jug == JUGADOR_A ? &zombiesA[indiceZombie] : &zombiesB[indiceZombie];
	char tipoZombie = info_jug->zombie_seleccionado->ascii;

	// inicializamos el directorio de zombie y copiamos su codigo
	mmu_inicializar_dir_zombi(jug, tipoZombie, indiceZombie);


	tss_inicializar_zombi(jug, indiceZombie);

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
	zombie->contador_reloj = 0;

	// Lo pintamos en el mapa
	print_zombi(jug, indiceZombie,zombie->i,zombie->j);

	info_jug->zombies_lanzados += 1;

	unsigned short columna_offset = jug == JUGADOR_A ? 31 : 49;//47
	unsigned short color = jug == JUGADOR_A ? 0x4F : 0x1F;
	print_int(20 - info_jug->zombies_lanzados,columna_offset,47,color);
	if(20 - info_jug->zombies_lanzados < 10) {
		print_int(0,columna_offset-1,47,color);
	}

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

	// Se imprime el reloj girado una posicion
	print_reloj_zombie(jugadorAct, indice);
	zombie->contador_reloj++;

	// los movimientos de los jugadores son opuestos
	int orientacion = jugadorAct == JUGADOR_A ? 1 : -1;
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
 /*
	print_int(pagina_destino_offset,20,18,30);
	print_int(orientacion==1,20,19,30);
	print_int(zombie->i,20,20,30);
	print_int(zombie->j,20,21,30);
	print_int(i,20,22,30);
	print_int(j,20,23,30);
*/

	//Movemos el codigo del zombie
	char *pagina_original = (char*) DIR_TAREAS;
	char *pagina_destino = (char*) (DIR_TAREAS + pagina_destino_offset * PAGE_SIZE);
	//print_hex((unsigned int)pagina_original,8,20,16,30);
	//print_hex((unsigned int)pagina_destino,8,20,17,30);
	unsigned int k;
	for(k = 0; k<PAGE_SIZE ; k++){
		pagina_destino[k] = pagina_original[k];
	}


	desmapear_entorno_zombie(zombie->i, zombie->j,rcr3());
	mappear_entorno_zombi(i,j,jugadorAct,(unsigned int) rcr3());


	// chequeamos si se comio otra tarea

	if(hay_otra_tarea(i,j)){

		jugador el_de_la_otra_tarea = el_jugador_que_tiene_la_otra_tarea(i,j);
		unsigned int tarea = la_otra_tarea(i,j);
		desalojar_tarea(tarea, el_de_la_otra_tarea);
	}

	// Printemo la pantalla (: # )
	print_limpiar_pos_zombi(zombie->i, zombie->j);

	// chequeamos si llego al final
	if (llego_al_final(jugadorAct,zombie) || llego_al_principio(jugadorAct,zombie)){

		desalojar_tarea_actual();
		if(llego_al_final(jugadorAct,zombie)) { jug->puntos += 1; screen_anotarPuntos(jugadorAct); }
		screen_zombie_cadaver(zombie->i, zombie->j);
		unprint_reloj_zombie(jugadorAct, indice);

	}
	// si no llego al final:
	else{
		print_zombi(jugadorAct, indice,i,j);
		// Actualizos posicion zombi
		zombie->i = mod_mapa(i);
		zombie->j = j;
	}

}


unsigned int puntos(jugador jug){
	unsigned int res = 0;
	switch (jug) {
		case JUGADOR_A: res =  A.puntos;break;
		case JUGADOR_B: res =  B.puntos;break;
	}
	return res;
}

unsigned int llego_al_final(jugador jug, info_zombie * zombie){
	unsigned int res = 0;
	switch (zombie->jug) {
		case JUGADOR_A: res = (zombie->j == SIZE_W); break;
		case JUGADOR_B: res = (zombie->j == 1); break;
	}
	return res;
}
unsigned int llego_al_principio(jugador jug, info_zombie * zombie){
	unsigned int res = 0;
	switch (zombie->jug) {
		case JUGADOR_A: res = (zombie->j == 1 && zombie->contador_reloj > 1); break;
		case JUGADOR_B: res = (zombie->j == SIZE_W && zombie->contador_reloj > 1); break;
	}
	return res;
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


info_zombie* obtener_arreglo_zombies(jugador jug){
	info_zombie* zombies = jug == JUGADOR_A ? (info_zombie*) &zombiesA : (info_zombie*) &zombiesB;
	return zombies;
}


unsigned int hay_zombies_activos(jugador jug){
	unsigned int res = FALSE;
	info_zombie *zombies = obtener_arreglo_zombies(jug);
	unsigned short i;

	for(i = 0; i < CANT_ZOMBIS; i++){
		if(zombies[i].estado == ACTIVO){
			res = TRUE;
		}
	}

	return res;
}


unsigned short indice_siguiente_zombie_activo(jugador jug, unsigned short indice){
	// Esta función devuelve el índice en el arreglo del siguiente zoombie activo del
	// jugador jug.
	// En caso de alcanzar el límite del arreglo, empieza a buscar desde el principio.
	// En caso de que haya un solo zoombie activo, devuelve el mismo índice.
	// Esta función asume que hay_zombies_activos devuelve TRUE

	unsigned short indice_sig_zoombie_act = indice;
	info_zombie *zombies = obtener_arreglo_zombies(jug);
	unsigned short i;
	int indice_sig_zoombie_act_encontrado = FALSE;

	for(i = indice+1; i < CANT_ZOMBIS && !indice_sig_zoombie_act_encontrado; i++){
		if(zombies[i].estado == ACTIVO){
			indice_sig_zoombie_act = i;
			indice_sig_zoombie_act_encontrado = TRUE;
		}
	}

	if(indice_sig_zoombie_act_encontrado == FALSE){
		for(i = 0; i < indice && !indice_sig_zoombie_act_encontrado; i++){
			if(zombies[i].estado == ACTIVO){
				indice_sig_zoombie_act = i;
				indice_sig_zoombie_act_encontrado = TRUE;
			}
		}
	}

	return indice_sig_zoombie_act;
}


unsigned int hay_otra_tarea(unsigned int i , unsigned int j){
	unsigned int res = 0;
	unsigned int k;
	for (k = 0; k < 8; k++){
		if (zombiesA[k].i == i && zombiesA[k].j == j && zombiesA[k].estado == ACTIVO){res = 1;}
		if (zombiesB[k].i == i && zombiesB[k].j == j && zombiesB[k].estado == ACTIVO){res = 1;}
	}
	return res;
}

jugador el_jugador_que_tiene_la_otra_tarea(unsigned int i , unsigned int j){
	jugador res;
	unsigned int k;
	for (k = 0; k < 8; k++){
		if (zombiesA[k].i == i && zombiesA[k].j == j){res = JUGADOR_A; break;}
		if (zombiesB[k].i == i && zombiesB[k].j == j){res = JUGADOR_B; break;}
	}
	return res;
}

unsigned int la_otra_tarea(unsigned int i , unsigned int j){
	unsigned int res = 0;
	unsigned int k;
	for (k = 0; k < 8; k++){
		if (zombiesA[k].i == i && zombiesA[k].j == j){res = k; break;}
		if (zombiesB[k].i == i && zombiesB[k].j == j){res = k; break;}
	}
	return res;
}

void desalojar_tarea_actual(){
    // El zombie que estaba corriendo se marca como INACTIVO
    // con lo cual el scheduler no lo va a poner a correr nunca más
    info_zombie* zombie_actual = obtener_zombie_actual();
    zombie_actual->estado = INACTIVO;
}

void desalojar_tarea(unsigned int indice, jugador jug){
    // El zombie que estaba corriendo se marca como INACTIVO
    // con lo cual el scheduler no lo va a poner a correr nunca más
    info_zombie* zombies = obtener_arreglo_zombies(jug);
    zombies[indice].estado = INACTIVO;
}



info_zombie* obtener_zombie_actual(){
	info_zombie* zombie =  jugadorActual() == JUGADOR_A ? &zombiesA[tarea_actual_A()] : &zombiesB[tarea_actual_B()];
    return zombie;
}



modo_debug estado_modo_debug(){
	return estado_debug;
}

unsigned int hay_interrupcion(){
	return hay_interrupcion_en_pantalla;
}

void swicth_modo_debug(){
	switch (estado_debug) {
		case DESACTIVADO:
			estado_debug = ACTIVADO;
			//print("activado",0,1,30); 
			break;
		case ACTIVADO: 
			estado_debug = DESACTIVADO;
			//print("desactivado",0,1,30);
			break;
	}
}

void switch_hay_interrupcion(){
	switch (hay_interrupcion_en_pantalla) {
		case TRUE:
			hay_interrupcion_en_pantalla = FALSE;
			//print("hay una interrupcion",1,1,30);  
			break;
		case FALSE: 
			hay_interrupcion_en_pantalla = TRUE;
			//print("no hay una interrupcion",1,1,30);
			break;
	}
}




unsigned int termino(){
	unsigned int res = 0;
	
	//si se llego al maximo de zombies (20 para cada jugador)
	if(A.zombies_lanzados == 21 || B.zombies_lanzados == 21){
		if(A.puntos > B.puntos) {
			print("Se llego a 20 zombies. Gano A",25,25,C_FG_LIGHT_RED);
		} else if(A.puntos < B.puntos) {
			print("Se llego a 20 zombies. Gano B",25,25,C_FG_LIGHT_BLUE);
		} else {
			print("Se llego a 20 zombies. Empate.",24,25,C_FG_WHITE);
		}
		res = 1;
	}

	//si se llego al maximo de puntos (10), y gano un jugador
	if(A.puntos == 10) {
		print("Gano A",37,25,C_FG_LIGHT_RED);
		res = 1;
	} else if(B.puntos == 10) {
		print("Gano B",37,25,C_FG_LIGHT_BLUE);
		res = 1;
	}


	//si las 16 tareas están activas y llegan a 2000 o mas
	unsigned int i;
	unsigned int cuenta = 0;
	for (i = 0 ; i<8 ;i++){
		cuenta = zombiesA[i].contador_reloj > 256 ? cuenta+1 : cuenta;
		cuenta = zombiesB[i].contador_reloj > 256 ? cuenta+1 : cuenta;
	}
	if(cuenta == 16){
		if(A.puntos > B.puntos) {
			print("Se termino el tiempo. Gano A",25,25,C_FG_LIGHT_RED);
		} else if(A.puntos < B.puntos) {
			print("Se termino el tiempo. Gano B",25,25,C_FG_LIGHT_BLUE);
		} else {
			print("Se termino el tiempo. Empate.",24,25,C_FG_WHITE);
		}
		res = 1;
	}

	return res;

}
