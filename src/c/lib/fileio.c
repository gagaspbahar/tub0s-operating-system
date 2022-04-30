#include "../header/fileio.h"

void readSector(byte *buffer, int sector_number){
    interrupt(0x21, 0x2, buffer, sector_number, 0x0);
}

void writeSector(byte *buffer, int sector_number){
    interrupt(0x21, 0x3, buffer, sector_number, 0x0);
}

void clear(byte *ptr, unsigned int n) {
    // Mengosongkan byte array yang memiliki panjang n
    int i;
	for (i = 0; i < n; i++){
		ptr[i] = 0x00;
	}
}

void read(FileMetadata *metadata, enum fs_retcode *return_code){
    interrupt(0x21, 0x4, metadata, return_code, 0x0);
}

void write(FileMetadata *metadata, enum fs_retcode *return_code){
    interrupt(0x21, 0x5, metadata, return_code, 0x0);
}