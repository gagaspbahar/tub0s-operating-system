#include "../../header/cp.h"

int main()
{
  struct message msg;
  get_message(&msg);
  cp(msg.arg2, msg.current_directory, msg.arg3);
  exit();
}

void cp(char *param, byte currDir, char *target)
{
  // cp dapat melakukan copy file dari current working directory ke current working directory
  // Relative pathing dan peng-copyan rekursif folder tidak diwajibkan
  struct node_filesystem node_fs_buffer;
  struct file_metadata data;
  enum fs_retcode return_code;
  byte tempBuffer[8192];
  int i;
  for (i = 0; i < 8192; i++)
  {
    tempBuffer[i] = 0x0;
  }

  // Inisialisasi pemindahan file
  data.buffer = tempBuffer;
  if (return_code == FS_R_NODE_NOT_FOUND)
  {
    puts("There is no such file\r\n");
    return;
  }
  else if (return_code == FS_R_TYPE_IS_FOLDER)
  {
    puts("Cannot copy directory\r\n");
    return;
  }
  data.parent_index = currDir;
  data.node_name = param;

  // Read & Write
  read(&data, &return_code);
  data.node_name = target;
  write(&data, &return_code);

  // Error
  if (return_code == FS_W_FILE_ALREADY_EXIST)
  {
    puts("File already exist\r\n");
  }
  else if (return_code == FS_W_NOT_ENOUGH_STORAGE)
  {
    puts("Not enough space\r\n");
  }
  else if (return_code == FS_W_MAXIMUM_NODE_ENTRY)
  {
    puts("Node reached maximum capacity\r\n");
  }
  else if (return_code == FS_W_MAXIMUM_SECTOR_ENTRY)
  {
    puts("Sector reached maximum capacity\r\n");
  }
  else if (return_code == FS_W_INVALID_FOLDER)
  {
    puts("Invalid folder\r\n");
  }
}
