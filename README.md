# Intro

Pulsar la tecla <kbd>o</kbd> incrementa el divisor de frecuencia del PIT.  
Pulsar la tecla <kbd>p</kbd> decrementa el divisor de frecuencia del PIT.  
Pulsar la tecla <kbd>^</kbd> (dcha de la <kbd>p</kbd> en teclado español) muestra el número de ticks del PIT.

# Makefile
La intención es tenerlo todo organizado en directorios decentes. Algo parecido  
a cómo está en la versión `0.01` del kernel de Linux:
- boot
    - boot.asm
- inc
- mm
    - pmm.c
    - pfa.c ¿?
    - heap.c ¿?
- kernel
    - kernel.c
    - idt.c
    - gdt.c
    - isr.c
    - irq.c
    - sys.c
    - vga.c
    - io.c
    - kb.c
    - pic.c
    - pit.c
