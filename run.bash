#!/bin/bash

COMMAND=$1

if [[ $COMMAND = "iso" ]]; then
    qemu-system-x86_64 -M q35 -cdrom ros.iso -boot d -serial stdio

elif [[ $COMMAND = "iso-uefi" ]]; then
    qemu-system-x86_64 -M q35 \
        -drive if=pflash,unit=0,format=raw,file=ovmf-code-x86_64.fd,readonly=on\
        -drive if=pflash,unit=1,format=raw,file=ovmf-vars-x86_64.fd\
        -cdrom ros.iso \
        -boot d -serial stdio
fi
if [[ $COMMAND = "iso-d" ]]; then
    qemu-system-x86_64 -M q35 -cdrom ros.iso -boot d -serial stdio -s -S

elif [[ $COMMAND = "iso-uefi-d" ]]; then
    qemu-system-x86_64 -M q35 \
        -drive if=pflash,unit=0,format=raw,file=ovmf-code-x86_64.fd,readonly=on\
        -drive if=pflash,unit=1,format=raw,file=ovmf-vars-x86_64.fd\
        -cdrom ros.iso \
        -boot d -serial stdio -s -S
fi
