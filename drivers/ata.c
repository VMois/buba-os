#include "ata.h"
#include "../cpu/type.h"
#include "../cpu/ports.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"

// Documentation source: https://wiki.osdev.org/ATA_PIO_Mode

void ide_delay(int n)
{
	for (int i = 0; i < n; i++)
		port_byte_in(ATA_SECONDARY_DEVCTL);
}

// drive software reset
void drive_reset() {
    port_byte_out(ATA_PRIMARY_DEVCTL, 0x04);
    ide_delay(4);
    port_byte_out(ATA_PRIMARY_DEVCTL, 0x00);
}

// chs to LBA
u32 chs_to_lba(u32 cylinder, u32 head, u32 sector) {
    return (cylinder * 16 + head) * 63 + (sector - 1);
}

int detect_devtype(int slavebit) {
    u32 REG_CYL_LO=4, REG_CYL_HI=5, REG_DEVSEL=6;
    /* waits until master drive is ready again */
	// ata_soft_reset(ctrl->dev_ctl);		
	port_byte_out(ATA_PRIMARY_IO + REG_DEVSEL, 0xA0 | slavebit<<4);
    // 400ns seconds delay
	ide_delay(4);

	unsigned cl = port_byte_in(ATA_PRIMARY_IO + REG_CYL_LO);	/* get the "signature bytes" */
	unsigned ch = port_byte_in(ATA_PRIMARY_IO + REG_CYL_HI);
 
	/* differentiate ATA, ATAPI, SATA and SATAPI */
	if (cl==0x14 && ch==0xEB) return 0;
	if (cl==0x69 && ch==0x96) return 1;
	if (cl==0 && ch == 0) return 2;
	if (cl==0x3c && ch==0xc3) return 3;
	return -1;
}

int ata_poll() {
    ide_delay(4);

    while(port_byte_in(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_BSY);

    uint8_t status = port_byte_in(ATA_PRIMARY_IO + ATA_REG_STATUS);

    if (status & ATA_SR_ERR)
        return 2;
    
    if (status & ATA_SR_DF)
        return 1;
    
    if ((status & ATA_SR_DRQ) == 0)
        return 3;
    
    return 0;
}

uint16_t* ata_read(uint32_t lba, uint8_t sector_num) {
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xE0 | (uint8_t) (0x00 << 4) | (uint8_t) ((lba >> 24) & 0x0F));
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, (uint8_t) sector_num);
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t) lba);
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_COMMAND, (uint8_t) ATA_CMD_READ_PIO);

    if (ata_poll())
        return NULL;

    uint16_t* data = (uint16_t*) kmalloc(sizeof(uint16_t) * sector_num * 256);
    for (int i = 0; i < sector_num * 256; i++) {
        data[i] = port_word_in(ATA_PRIMARY_IO + ATA_REG_DATA);
    }
    return data;
}

uint8_t ata_write(uint32_t lba, uint8_t sector_num, uint16_t* data, uint8_t size) {
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xE0 | (uint8_t) (0x00 << 4) | (uint8_t) ((lba >> 24) & 0x0F));
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, (uint8_t) sector_num);
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t) lba);
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_COMMAND, (uint8_t) ATA_CMD_WRITE_PIO);

    if (ata_poll())
        return 1;

    for (int i = 0; i < sector_num * 256; i++) {
        ata_poll();
        if (i < size) {
            port_word_out(ATA_PRIMARY_IO + ATA_REG_DATA, data[i]);
            continue;
        }
        // fill with zeros the rest
        port_word_out(ATA_PRIMARY_IO + ATA_REG_DATA, (uint16_t) 0x00);
    }
    port_byte_out(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH); // cache flush
    ata_poll();
    return 0;
}

void ata_init() {
    drive_reset();
}
