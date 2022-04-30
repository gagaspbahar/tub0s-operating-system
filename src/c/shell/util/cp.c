#include "../../header/std_type.h"
#include "../../header/textio.h"
#include "../../header/fileio.h"
#include "../../header/program.h"
#include "../../header/utils.h"
#include "../../header/filesystem.h"

int main() {
    struct message msg;
    get_message(&msg);
    /*
        TODO : Implementasi cp
    */
    exit();
}

void cp(char *param, byte currDir, char *target){
// cp dapat melakukan copy file dari current working directory ke current working directory
// Relative pathing dan peng-copyan rekursif folder tidak diwajibkan
  struct node_filesystem node_fs_buffer;
  struct file_metadata data;
  enum fs_retcode return_code;
  byte tempBuffer[8192];
  int i;
  for(i = 0; i < 8192; i++){
    tempBuffer[i] = 0x0;
  }

  // Inisialisasi pemindahan file
  data.buffer = tempBuffer;
  if (return_code == FS_R_NODE_NOT_FOUND){
    printString("There is no such file\r\n");
    return;
  } else if (return_code == FS_R_TYPE_IS_FOLDER){
    printString("Cannot copy directory\r\n");
    return;
  }
  data.parent_index = currDir;
  data.node_name = param;

  // Read & Write
  read(&data, &return_code);
  data.node_name = target;
  write(&data, &return_code);

  // Error
  if (return_code == FS_W_FILE_ALREADY_EXIST){
    printString("File already exist\r\n");
  } else if (return_code == FS_W_NOT_ENOUGH_STORAGE){
    printString("Not enough space\r\n");
  } else if (return_code == FS_W_MAXIMUM_NODE_ENTRY){
    printString("Node reached maximum capacity\r\n");
  } else if (return_code == FS_W_MAXIMUM_SECTOR_ENTRY){
    printString("Sector reached maximum capacity\r\n");
  } else if (return_code == FS_W_INVALID_FOLDER){
    printString("Invalid folder\r\n");
  }

}
