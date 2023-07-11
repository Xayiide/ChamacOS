# Requisitos:
- [Compilador Cruzado](https://wiki.osdev.org/GCC_Cross-Compiler)
- [QEMU](https://www.qemu.org/)
- make



# Intro

Pulsar la tecla <kbd>o</kbd> incrementa el divisor de frecuencia del PIT.  
Pulsar la tecla <kbd>p</kbd> decrementa el divisor de frecuencia del PIT.  
Pulsar la tecla <kbd>^</kbd> (dcha de la <kbd>p</kbd> en teclado español) muestra el número de ticks del PIT.

# Inspiraciones
La ayuda principal la estoy sacando de la [wiki](https://wiki.osdev.org/).  
Tambien de proyectos como
- [Skylight](https://github.com/austanss/skylight/),
- [TwilightOS](https://github.com/Zandr0id/TwilightOS/),
- [KoizOS](https://github.com/drakeor/koiz-os/),
- [CuriOS](https://github.com/h5n1xp/CuriOS/)
- [ShawnOS](https://github.com/shawnanastasio/ShawnOS/)

# Para compilar:
- `make qemu` + `gdb`: Compilar depurando con GDB
- `make fqemu`: Compilar y ejecutar rápido con qemu
- `make cdqemu`: Compilar y ejecutar rápido con qemu usando GRUB
