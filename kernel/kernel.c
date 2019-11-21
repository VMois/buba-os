#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"
#include "../libc/string.h"

void main() {
    isr_install();
    irq_install();
    clear_screen();

    kprint("Kernel input\n"
           "Type DIE to halt the CPU \n >");
}

void user_input(char *input) {
    if (strcmp(input, "DIE") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}
