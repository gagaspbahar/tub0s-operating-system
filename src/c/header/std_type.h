#ifndef _STD_TYPE_H
#define _STD_TYPE_H

typedef char bool;
#define true 1
#define false 0

typedef unsigned char byte;

struct message
{
  byte current_directory;
  char arg1[64];
  char arg2[64];
  char arg3[64];
  int next_program_segment;
  byte other[317];
};

typedef struct message Message;

#endif
