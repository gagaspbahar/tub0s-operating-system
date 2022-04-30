#ifndef FILEIO_H
#define FILEIO_H

#include "std_type.h"
#include "filesystem.h"

void readSector(byte *buffer, int sector_number);

void writeSector(byte *buffer, int sector_number);

void read(FileMetadata *metadata, enum fs_retcode *return_code);

void write(FileMetadata *metadata, enum fs_retcode *return_code);

#endif