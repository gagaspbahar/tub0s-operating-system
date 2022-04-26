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
  byte tempBuffer[512*16];

  // Inisialisasi pemindahan file
  data.buffer = tempBuffer;
  data.parent_index = currDir;
  data.node_name = param;

  // Read & Write
  read(&data, &return_code);
  data.node_name = target;
  write(&data, &return_code);

  // Error
  if (return_code == FS_W_FILE_ALREADY_EXIST){
    printString("File already exist\r\n");
  }

}
