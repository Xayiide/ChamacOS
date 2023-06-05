if grub-file --is-x86-multiboot chamacos.bin; then
    echo chamacos.bin: Multiboot confirmado
else
    echo chamacos.bin no es Multiboot
fi

if grub-file --is-x86-multiboot chamacos.elf; then
    echo chamacos.elf: Multiboot confirmado
else
    echo chamacos.elf no es Multiboot
fi
