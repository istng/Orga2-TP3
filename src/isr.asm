; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00


;; PIC
extern fin_intr_pic1
extern deshabilitar_pic

;; Sched
extern sched_proximo_indice

;; Auxiliares
extern convert_tecla
extern accion_tecla
extern desalojar_tarea_actual

extern rcr0
extern rcr1
extern rcr2
extern rcr3


;; Game
extern game_move_current_zombi

extern estado_modo_debug
extern hay_interrupcion
extern swicth_modo_debug
extern switch_hay_interrupcion
extern volver_al_juego
extern print_debug_screen

extern termino

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

mensaje_intr db     'Interrupcion XYZ'
mensaje_intr_len equ    $ - mensaje_intr

%macro imprimir_interrupcion 1
    ; %1->mensaje, %2->filas, %3->columnas
    mov al, %1
    mov ebx, mensaje_intr
    add ebx, mensaje_intr_len
    dec ebx ; se reemplazara Z por la unidad del numero de interrupcion
    mov cl, al
    xor ax, ax
    mov al, cl
    mov dl, 10 ; divisor
    div dl ; se divide por 10 para obtener la unidad
    add ah, 48 ; 48 es el valor ASCII de 0, se le suma al resto de la division para obtener la unidad en ASCII
    mov [ebx], ah
    dec ebx ; se reemplazara Y por la decena del numero de interrupcion
    mov cl, al
    xor ax, ax
    mov al, cl
    div dl ; se vuelve a dividir por 10 para obtener la decena y la centena
    add ah, 48
    mov [ebx], ah ; se reemplaza Y por la decena del numero de interrupcion
    dec ebx
    add al, 48
    mov [ebx], al ; se reemplaza X por la centena del numero de interrupcion
    imprimir_texto_mp mensaje_intr, mensaje_intr_len, 0x07, 0, 34
%endmacro


mensaje_tecla db     ' '
mensaje_tecla_len equ    $ - mensaje_tecla

%macro imprimir_tecla 3
    ; %1->mensaje, %2->filas, %3->columnas
    ;xor eax, eax
    mov al, %1
    mov ebx, mensaje_tecla
    add ebx, mensaje_tecla_len
    dec ebx
    push eax
    call convert_tecla
    add esp,4
    mov [ebx], al
    imprimir_texto_mp mensaje_tecla, mensaje_tecla_len, 0x07, %2, %3
%endmacro

%macro ISR 1
global _isr%1
_isr%1:
    pushad
    ;pushad

    imprimir_interrupcion %1

    ;call estado_modo_debug
    ;cmp eax,0
    ;jz .seguir

    ; aca hacemos cosas chachas para conseguir lo que queremos
    ;call .get_eip
    ;.get_eip:
    ;pop eax
    ;push eax; pusheamos el eip
    ;call print_debug_screen
    ;pop eax; popeamos el eip

    ;jmp .fin

    .seguir:
    call desalojar_tarea_actual

    .fin:
    call fin_intr_pic1
    jmp 14<<3:0 ; Tarea idle
    ;popad
    popad
    iret
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19





;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32:
_isr32:
    pushad

    call proximo_reloj
    call termino
    cmp ax,1
    je .termino

    call hay_interrupcion
    cmp ax,1;
    je .nojump


    call sched_proximo_indice
    cmp ax, 0
    je  .nojump

    shl ax, 3
    mov [sched_tarea_selector], ax
    call fin_intr_pic1
    ;xchg bx,bx
    jmp far [sched_tarea_offset]


    jmp .end

    .nojump:
        call fin_intr_pic1
        jmp .end

    .termino:
    call deshabilitar_pic
    .loopear:
    jmp .loopear


    .end:
    popad
    iret


;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
    pushad
    xor eax, eax
    in al, 0x60
    push eax
    call accion_tecla
    pop eax
    imprimir_tecla al, 0, 79
    ;call accion_tecla 
    call fin_intr_pic1

    popad
    iret

;;
;; Rutina de atención del Software
;; -------------------------------------------------------------------------- ;;
global _isr0x66
_isr0x66:

    pushad

    push eax
    call game_move_current_zombi
    call fin_intr_pic1
    pop eax
    jmp 14<<3:0

    popad
    iret


;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret
