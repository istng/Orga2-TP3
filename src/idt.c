/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "game.h"

#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x40;                                                                  \
    idt[numero].attr = (unsigned short) 0x8E00;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);



void idt_inicializar() {
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(15);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);

    // Entrada interrupcion reloj
    IDT_ENTRY(32);
    // Entrada interrupcion teclado
    IDT_ENTRY(33);

    // Entrada interrupcion de software
    IDT_ENTRY(102);


}


//HACER UN .C QUE SEA DE AUXILIARES!!!!!!! (mover también del .h)
char convert_tecla(char scan_code) {
    char ascii = 0;
    switch(scan_code) {
        case 0x11:
            ascii = 'W';
            break;
        case 0x1E:
            ascii = 'A';
            break;
        case 0x1F:
            ascii = 'S';
            break;
        case 0x20:
            ascii = 'D';
            break;
        case 0x2A:
            ascii = '<';   //left shift
            break;
        case 0x17:
            ascii = 'I';
            break;
        case 0x24:
            ascii = 'J';
            break;
        case 0x25:
            ascii = 'K';
            break;
        case 0x26:
            ascii = 'L';
            break;
        case 0x36:
            ascii = '>';   //right shift
            break;
    }
    return ascii;
}


void accion_tecla(char scan_code){
    char tecla = convert_tecla(scan_code);
    switch(tecla){
        case 'W':
            print_limpiar_pos_jugador(JUGADOR_A);
            game_jugador_mover(JUGADOR_A, ARRIBA);
            print_jugador(JUGADOR_A);
            break;
        case 'A':
            game_jugador_cambiar_zombie(JUGADOR_A, ANTERIOR);
            print_jugador(JUGADOR_A);
            break;
        case 'S':
            print_limpiar_pos_jugador(JUGADOR_A);
            game_jugador_mover(JUGADOR_A, ABAJO);
            print_jugador(JUGADOR_A);
            break;
        case 'D':
            game_jugador_cambiar_zombie(JUGADOR_A, SIGUIENTE);
            print_jugador(JUGADOR_A);
            break;
        case '<':
            game_lanzar_zombi(JUGADOR_A);
            print("termino",20,20,0x22);
            break;
        case 'I':
            print_limpiar_pos_jugador(JUGADOR_B);
            game_jugador_mover(JUGADOR_B, ARRIBA);
            print_jugador(JUGADOR_B);
            break;
        case 'J':
            game_jugador_cambiar_zombie(JUGADOR_B, ANTERIOR);
            print_jugador(JUGADOR_B);
            break;
        case 'K':
            print_limpiar_pos_jugador(JUGADOR_B);
            game_jugador_mover(JUGADOR_B, ABAJO);
            print_jugador(JUGADOR_B);
            break;
        case 'L':
            game_jugador_cambiar_zombie(JUGADOR_B, SIGUIENTE);
            print_jugador(JUGADOR_B);
            break;
        case '>':
            game_lanzar_zombi(JUGADOR_B);
            break;
    }
}
