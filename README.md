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

# Dependencias
Además del compilador cruzado que se explica [aquí](https://wiki.osdev.org/GCC_Cross-Compiler), es necesario instalar varias cosas para poder hacer una imagen con GRUB, tal y como se explica bien en [la Biblia](https://wiki.osdev.org/Bare_Bones#Booting_the_Kernel). Estas cosas son:  
- xorriso
- mtools  

Y sobra decir que también GRUB con todas sus utilidades para poder hacer uso de `grub-mkrescue`.
