#ifndef UTILS_H
#define UTILS_H

#include "std_type.h"
#include "filesystem.h"

#define FS_MESSAGE_SECTOR_NUMBER 0x104

extern int getCurrentSegment();

struct message {
  byte current_directory;
  char arg1[64];
  char arg2[64];
  char arg3[64];
  int next_program_segment; // Dapat digunakan untuk bagian 3.5
  byte other[317];
};

typedef struct message Message;

void get_message(Message *msg);

void set_message(Message *msg);

#endif