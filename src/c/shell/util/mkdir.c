#include "../../header/mkdir.h"

int main()
{
  struct message msg;
  get_message(&msg);
  mkdir(msg.arg2, msg.current_directory);
  exit();
}

void mkdir(char *param, byte idxParent)
{
  // mkdir dapat membuat folder baru pada current working directory
  struct file_metadata *data;
  enum fs_retcode return_code;

  // set data
  data->node_name = param;
  data->parent_index = idxParent;
  data->filesize = 0; // Folder size = 0
  write(data, &return_code);

  if (return_code == 0)
  {
    puts("Successfully created directory\r\n");
  }
  else
  {
    puts("Build failed\r\n");
  }
}