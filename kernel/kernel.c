#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"

void main() {
    isr_install();
    irq_install();
    clear_screen();

    kprint("Kernel input\n"
           "Type DIE to halt the CPU \n > ");
}

void user_input(char *input) {
    if (strcmp(input, "DIE") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "AL") == 0) {
        char *test = (char*) kmalloc(6*sizeof(char));
        if (test == NULL) {
            kprint("bad");
        }
        char h[] = "hello\n";
        strcopy(test, h);
        kprint(test);
        free(test);
        test = NULL;
        test = (char*) kmalloc(5*sizeof(char));
        char t[] = "test";
        strcopy(test, t);
        kprint(test);
        free(test);
        test = NULL;
        test = (char*) kmalloc(5*sizeof(char));
        char c[] = "capt";
        strcopy(test, c);
        kprint(test);
        free(test);
    }
    kprint("\n > ");
}
