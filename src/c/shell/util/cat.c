#include "../../header/cat.h"

int main()
{
  struct message msg;
  get_message(&msg);
  cat(msg.arg2, msg.current_directory);
  exit();
}

void cat(char *param, byte currDir)
{
  // cat dapat menampilkan isi dari file sebagai text file
  byte buf[8192];
  struct node_filesystem node_fs;
  struct node_entry node;
  struct file_metadata data;
  enum fs_retcode return_code;
  int i;

  for (i = 0; i < 8192; i++)
  {
    buf[i] = 0x0;
  }

  data.parent_index = currDir;
  data.buffer = buf;
  data.node_name = param;

  read(&data, &return_code);

  if (return_code == FS_R_TYPE_IS_FOLDER)
  {
    puts("Type is folder\r\n");
  }
  else if (return_code == FS_R_NODE_NOT_FOUND)
  {
    puts("Node not found\r\n");
  }
  else
  {
    puts(data.buffer);
    puts("\r\n");
  }
}
