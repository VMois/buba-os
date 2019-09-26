mov ah, 0x0e ; tty mode

; Not working
; Try to put address value to al, but need the CONTENT of address
mov al, the_secret
int 0x10

; Not working
; Trying to put content, but it doesn't include memore offset
mov al, [the_secret]
int 0x10

; Working.
; We are adding the_secret address and default memory offset. 
; Now it correctly accessing a memory
mov bx, the_secret
add bx, 0x7c00
mov al, [bx]
int 0x10

; Working
; We have manually calculated where the_secret is stored and add the default memory offset (0x7c00)
; Not flexible
mov al, [0x7c09]
int 0x10

jmp $

the_secret :
db "X"
; Padding and magic BIOS number.
times 510 -($-$$) db 0
dw 0xaa55