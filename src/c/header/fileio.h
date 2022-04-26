#ifndef FILEIO_H
#define FILEIO_H

#include "std_type.h"

void readSector(byte *buffer, int sector_number);

void writeSector(byte *buffer, int sector_number);

void read(struct file_metadata *metadata, enum fs_retcode *return_code);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);

#endif