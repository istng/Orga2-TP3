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


;; Sched
extern sched_proximo_indice

;; Auxiliares
extern convert_tecla



;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

mensaje_intr db     'Interrupcion X'
mensaje_intr_len equ    $ - mensaje_intr

%macro imprimir_interrupcion 1
    ; %1->mensaje, %2->filas, %3->columnas
    mov al, %1
    mov ebx, mensaje_intr
    add ebx, mensaje_intr_len
    dec ebx
    add al, 48
    mov [ebx], al
    imprimir_texto_mp mensaje_intr, mensaje_intr_len, 0x07, 24, 34
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

    imprimir_interrupcion %1
    jmp $

    popad
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
global _isr32
_isr32:
    pushad

    ;imprimir_interrupcion 32 ;borrar

    call fin_intr_pic1
    call proximo_reloj
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
    imprimir_tecla al, 0, 79
    call fin_intr_pic1

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
        
        
