#include "../drivers/screen.h"

void main() {
    clear_screen();
    kprint("  BubaOS is here... to do nothing... yet");
    for (int i = 0; i < 25; i++) {
        char str[255];
        int_to_ascii(i, str);
        kprint_at(str, 0, i);
    }
}
