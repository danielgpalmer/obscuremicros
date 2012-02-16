#include <diskio.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static uint8_t* flash = (uint16_t*) 0x80000000;

DSTATUS disk_initialize(BYTE disk) {
	printf("disk_initialize(%d)\n", disk);
	return 0;
}
DSTATUS disk_status(BYTE disk) {
	printf("disk_status(%d)\n", disk);
	return STA_PROTECT;
}
DRESULT disk_read(BYTE disk, BYTE* buffer, DWORD sector, BYTE count) {
	printf("disk_read(%d,0x%08x,%d,%d)\n", disk, buffer, sector, count);
	unsigned offset = sector * 512, bytes = count * 512;

	printf("reading %d bytes, offset %d\n", bytes, offset);

	memcpy(buffer, flash + offset, bytes);
	return RES_OK;
}
DRESULT disk_write(BYTE s0, const BYTE* s1, DWORD s2, BYTE s3) {
	return RES_OK;
}

DRESULT disk_ioctl(BYTE disk, BYTE command, void* buffer) {
	printf("disk_ioctl(%d,%d,0x08x)\n", disk, command, buffer);
	return RES_OK;
}

DWORD get_fattime(void) {
	return 1;
}
