mov ah, 0x0e ; tty mode
mov al, 'B'
int 0x10
mov al, 'u'
int 0x10
mov al, 'b'
int 0x10
mov al, 'a'
int 0x10
mov al, ' '
int 0x10
mov al, 'O'
int 0x10
mov al, 'S'
int 0x10

jmp $; jump to current address = infinite loop

times 510-($-$$) db 0
dw 0xaa55 
