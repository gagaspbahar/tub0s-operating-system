#include "../header/program.h"

void exec(struct file_metadata *metadata, int segment)
{
  interrupt(0x21, 0x6, metadata, segment, 0x0);
}

void exit()
{
  struct file_metadata meta;
  meta.node_name = "shell";
  meta.parent_index = 0x00;

  exec(&meta, 0x2000);
}