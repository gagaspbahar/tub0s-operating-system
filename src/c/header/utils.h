#ifndef UTILS_H
#define UTILS_H

#include "std_type.h"
#include "filesystem.h"

#define FS_MESSAGE_SECTOR_NUMBER 0x104

extern int getCurrentSegment();

void get_message(Message *msg);

void set_message(Message *msg);

#endif