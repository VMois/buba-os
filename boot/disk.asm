disk_load:
    pusha
    push dx

    mov ah, 0x02 ; ah <- int 0x13 function, 0x02 = 'read'
    mov al, dh   ; number of sectors to read
    mov cl, 0x02 ; sector num
    mov ch, 0x00 ; cylinder num
    mov dh, 0x00 ; head num

    int 0x13
    jc disk_error
    pop dx
    cmp al, dh
    jne sectors_error
    popa
    ret

disk_error:
    mov bx, DISK_ERROR
    call print
    call print_nl
    mov dh, ah
    call print_hex
    jmp disk_loop

sectors_error:
    mov bx, SECTORS_ERROR
    call print

disk_loop:
    jmp $

DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0
