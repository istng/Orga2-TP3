/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"

char reloj_ascii[4] = {(char)'|', (char)'/', (char)'-', (char)'\\'};

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}


//hardcodeamos los valores que nunca cambian de la pantalla
void print_screen(){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia || arreglo de filas
    int i,j;

    //pintamos todo de verde color esperanza
    for (i = 0; i < VIDEO_FILS;++i){
        for (j = 0; j < VIDEO_COLS;++j){
            p[i][j].c = 219;
            p[i][j].a = C_FG_GREEN;
        }
    }

    //pinto las partes correspondientes de negro
    for (i = 0; i < VIDEO_COLS; ++i){
    	p[0][i].c = 219;
    	p[0][i].a = C_FG_BLACK;
    	p[45][i].c = 219;
		p[45][i].a = C_FG_BLACK;
		p[46][i].c = 219;
		p[46][i].a = C_FG_BLACK;
		p[47][i].c = 219;
    	p[47][i].a = C_FG_BLACK;
    	p[48][i].c = 219;
    	p[48][i].a = C_FG_BLACK;
    	p[49][i].c = 219;
    	p[49][i].a = C_FG_BLACK;
    }

    //pintamos las barras roja y azul
    for (i = 1; i < VIDEO_FILS - 5; ++i){
    	p[i][0].c = 219;
    	p[i][0].a = C_FG_RED;
    	p[i][79].c = 219;
    	p[i][79].a = C_FG_BLUE;
    }

	//agregamos los cuadros de puntos
	for (i = 34; i < 39; ++i){
    	p[45][i].c = 219;
		p[45][i].a = C_FG_RED;
		p[46][i].c = 219;
		p[46][i].a = C_FG_RED;
		p[47][i].c = 219;
    	p[47][i].a = C_FG_RED;
    	p[48][i].c = 219;
    	p[48][i].a = C_FG_RED;
    	p[49][i].c = 219;
    	p[49][i].a = C_FG_RED;

    	p[45][i+5].c = 219;
		p[45][i+5].a = C_FG_BLUE;
		p[46][i+5].c = 219;
		p[46][i+5].a = C_FG_BLUE;
		p[47][i+5].c = 219;
    	p[47][i+5].a = C_FG_BLUE;
    	p[48][i+5].c = 219;
    	p[48][i+5].a = C_FG_BLUE;
    	p[49][i+5].c = 219;
    	p[49][i+5].a = C_FG_BLUE;
	}

	//agregamos los numeros
	p[46][4].c = 49;
	p[46][4].a = C_FG_WHITE;
  p[48][4].c = 88;
  p[48][4].a = C_FG_RED;
	p[46][6].c = 50;
	p[46][6].a = C_FG_WHITE;
  p[48][6].c = 88;
  p[48][6].a = C_FG_RED;
	p[46][8].c = 51;
	p[46][8].a = C_FG_WHITE;
  p[48][8].c = 88;
  p[48][8].a = C_FG_RED;
	p[46][10].c = 52;
	p[46][10].a = C_FG_WHITE;
  p[48][10].c = 88;
  p[48][10].a = C_FG_RED;
	p[46][12].c = 53;
	p[46][12].a = C_FG_WHITE;
  p[48][12].c = 88;
  p[48][12].a = C_FG_RED;
	p[46][14].c = 54;
	p[46][14].a = C_FG_WHITE;
  p[48][14].c = 88;
  p[48][14].a = C_FG_RED;
	p[46][16].c = 55;
	p[46][16].a = C_FG_WHITE;
  p[48][16].c = 88;
  p[48][16].a = C_FG_RED;
	p[46][18].c = 56;
	p[46][18].a = C_FG_WHITE;
  p[48][18].c = 88;
  p[48][18].a = C_FG_RED;

	p[46][74].c = 56;
	p[46][74].a = C_FG_WHITE;
  p[48][74].c = 88;
  p[48][74].a = C_FG_RED;
	p[46][72].c = 55;
	p[46][72].a = C_FG_WHITE;
  p[48][72].c = 88;
  p[48][72].a = C_FG_RED;
	p[46][70].c = 54;
	p[46][70].a = C_FG_WHITE;
  p[48][70].c = 88;
  p[48][70].a = C_FG_RED;
	p[46][68].c = 53;
	p[46][68].a = C_FG_WHITE;
  p[48][68].c = 88;
  p[48][68].a = C_FG_RED;
	p[46][66].c = 52;
	p[46][66].a = C_FG_WHITE;
  p[48][66].c = 88;
  p[48][66].a = C_FG_RED;
	p[46][64].c = 51;
	p[46][64].a = C_FG_WHITE;
  p[48][64].c = 88;
  p[48][64].a = C_FG_RED;
	p[46][62].c = 50;
	p[46][62].a = C_FG_WHITE;
  p[48][62].c = 88;
  p[48][62].a = C_FG_RED;
	p[46][60].c = 49;
	p[46][60].a = C_FG_WHITE;
  p[48][60].c = 88;
  p[48][60].a = C_FG_RED;

  //puntos
  print_int(0,36,47,0x4F);
  print_int(0,41,47,0x1F);

	// Agregamos los jugadores
	print_jugador(JUGADOR_A);
	print_jugador(JUGADOR_B);
}


void print_jugador(jugador jug){
	switch(jug){
		case JUGADOR_A:
			print(&(A.zombie_seleccionado->ascii), 0, A.pos, C_FG_LIGHT_RED);
			break;
		case JUGADOR_B:
			print(&(B.zombie_seleccionado->ascii), VIDEO_COLS-1, B.pos, C_FG_LIGHT_BLUE);
			break;
	}
}


void print_limpiar_pos_jugador(jugador jug){
	char char_vacio[2] = {219, 0};
	switch(jug){
		case JUGADOR_A:
			print(char_vacio, 0, A.pos, C_FG_RED);
			break;
		case JUGADOR_B:
			print(char_vacio, VIDEO_COLS-1, B.pos, C_FG_BLUE);
			break;
	}
}

void print_zombi(jugador jug, unsigned int i, unsigned int j){
  switch(jug){
		case JUGADOR_A:
			print(&(A.zombie_seleccionado->ascii), j, i, C_FG_LIGHT_RED);
			break;
		case JUGADOR_B:
			print(&(B.zombie_seleccionado->ascii), j, i, C_FG_LIGHT_BLUE);
			break;
	}
}

void print_limpiar_pos_zombi(unsigned int i,unsigned int j){
    print("X",j,i,0x27);
}


void screen_anotarPuntos(jugador jug){

  unsigned int p = puntos(jug);
  switch (jug) {
    case JUGADOR_A: print_int(p,36,47,0x4F); break;
    case JUGADOR_B: print_int(p,41,47,0x1F); break;
  }

}


void screen_zombie_cadaver (unsigned int i , unsigned int j){
    print("X",j,i,0x24);
}


void print_reloj_zombie(jugador jug, unsigned int indice){
	info_zombie* zombie = obtener_zombie_actual();
	unsigned short fila_offset = jug == JUGADOR_A ? 4 : 60;
	char reloj[2];
	reloj[0] = reloj_ascii[zombie->contador_reloj % 4];
	reloj[1] = 0; // caracter de terminacion
	print(reloj, fila_offset + 2 * indice, 48, C_FG_WHITE);
}

void unprint_reloj_zombie(jugador jug, unsigned int indice){
    unsigned short fila_offset = jug == JUGADOR_A ? 4 : 60;
    print("X", fila_offset + 2 * indice, 48, C_FG_RED);
}

void print_debug_screen(unsigned int eip,unsigned int edi,unsigned int esi,unsigned int ebp,unsigned int esp,unsigned int ebx,unsigned int edx,unsigned int ecx,unsigned int eax){
    print_hex(eax,8,10,30,C_FG_WHITE);
    print_hex(ebx,8,12,30,C_FG_WHITE);
    print_hex(ecx,8,14,30,C_FG_WHITE);
    print_hex(edx,8,16,30,C_FG_WHITE);
    print_hex(esi,8,18,30,C_FG_WHITE);
    print_hex(edi,8,20,30,C_FG_WHITE);
    print_hex(ebp,8,22,30,C_FG_WHITE);
    print_hex(esp,8,24,30,C_FG_WHITE);
    print_hex(eip,8,26,30,C_FG_WHITE);
}
