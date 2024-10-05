IMAGE_NAME=$1
rm -rf -v iso_root
mkdir -p -v iso_root/boot
cp -v ./src/ros.elf iso_root/boot
mkdir -p -v iso_root/boot/limine
cp -v limine.conf limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/boot/limine
mkdir -p -v iso_root/EFI/BOOT
cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT
cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT
xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    --efi-boot boot/limine/limine-uefi-cd.bin \
    -efi-boot-part --efi-boot-image --protective-msdos-label \
    iso_root -o $IMAGE_NAME.iso
./limine/limine bios-install $IMAGE_NAME.iso

rm -rf -v iso_root
