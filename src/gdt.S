.global reloadSegments
reloadSegments:
    push $0x08
    lea .reload_CS, %rax
    push %rax
    lretq
.reload_CS:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret
