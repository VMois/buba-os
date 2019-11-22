#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"

void main() {
    isr_install();
    irq_install();
    clear_screen();

    kprint("Ready for input\n"
           "Type DIE to halt the CPU \n"
           "> "
    );
}

void user_input(char *input) {
    if (strcmp(input, "DIE") == 0) {
        kprint("Stopping the CPU\n");
        asm volatile("hlt");
    }
    kprint("\n > ");
}
