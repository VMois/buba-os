[org 0x7c00]

mov bp, 0x8000 ; bottom address of the stack
mov sp, bp ; set bottom = top address of the stack

mov bx, 0x9000
mov dh, 2
mov dl, 0x80 ; choose hard disk 0
call disk_load

mov dx, [bx] ; retrieve the first loaded word, 0xdada
call print_hex
call print_nl

mov dx, [bx + 512] ; first word from second loaded sector, 0xface
call print_hex

jmp $; jump to current address = infinite loop

%include "print.asm"
%include "print_hex.asm"
%include "boot_sector_disk.asm"


times 510-($-$$) db 0
dw 0xaa55 

times 256 dw 0xdada ; sector 2 = 512 bytes
times 256 dw 0xface 
