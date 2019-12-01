#include "filesystem.h"

void fs_init() {

    kprint("Example usage of ATA driver \n");
    uint32_t t = chs_to_lba(0, 0, 30);
    uint16_t data[4] = {1, 2, 3, 4};
    if(ata_write(t, 1, data, 4)) {
        kprint("Write error");
    }
    uint16_t *tmp = ata_read(t, 1);
    for (int i = 0; i < 16; i++) {
        char* str = (char*) kmalloc(sizeof(char) * 6);
        hex_to_ascii(tmp[i], str);
        kprint(str);
        kprint("\n");
        free(str);
    }
}
