#!/bin/bash
IMAGE_NAME=$1
rm -f $IMAGE_NAME.hdd
dd if=/dev/zero bs=1M count=0 seek=64 of=$IMAGE_NAME.hdd
sgdisk $IMAGE_NAME.hdd -n 1:2048 -t 1:ef00
./limine/limine bios-install $IMAGE_NAME.hdd
mformat -i $IMAGE_NAME.hdd@@1M
mmd -i $IMAGE_NAME.hdd@@1M ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine
mcopy -i $IMAGE_NAME.hdd@@1M ./src/ros.elf ::/boot
mcopy -i $IMAGE_NAME.hdd@@1M limine.conf limine/limine-bios.sys ::/boot/limine
mcopy -i $IMAGE_NAME.hdd@@1M limine/BOOTX64.EFI ::/EFI/BOOT
mcopy -i $IMAGE_NAME.hdd@@1M limine/BOOTIA32.EFI ::/EFI/BOOT
