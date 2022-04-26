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
        TODO : Implementasi mkdir
    */
    exit();
}

void mkdir(char *param, byte idxParent){
  // mkdir dapat membuat folder baru pada current working directory
  struct file_metadata *data;
  enum fs_retcode return_code;

  // set data
  data->node_name = param;
  data->parent_index = idxParent;
  data->filesize = 0;// Folder size = 0 
  write(data, &return_code);

  if (return_code == 0){
    printString("Successfully created directory\r\n");
  } else {
    printString("Build failed\r\n");
  }

}