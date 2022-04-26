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
        TODO : Implementasi cat
    */
    exit();
}

void cat(char *param, byte currDir){
//cat dapat menampilkan isi dari file sebagai text file
  int i = 0;
  struct file_metadata data;
  struct node_filesystem node_fs_buffer;
  enum fs_retcode return_code;
  int idx_sector = -1;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  // Cari indexnya
  for (i = 0; i < 64; i++){
    if (node_fs_buffer.nodes[i].parent_node_index == currDir && strcmp(node_fs_buffer.nodes[i].name, param)){
        idx_sector = i;
        break;
    }
  }

  // Kasus file tidak dapat ditemukan
  if (idx_sector == -1){
      return_code = FS_R_NODE_NOT_FOUND;
      printString("File not found\n");
      return;
  } 

  clear(data.node_name, 14);
  strcpy(data.node_name, node_fs_buffer.nodes[idx_sector].name);

  data.parent_index = node_fs_buffer.nodes[idx_sector].parent_node_index;
  read(&data, &return_code);

  if (return_code == FS_R_TYPE_IS_FOLDER){
    printString("Type is folder\r\n");
  } else if (return_code == FS_R_NODE_NOT_FOUND){
    printString("Node not found\r\n");
  } else {
    printString(data.buffer);
    printString("\r\n");
  }

}
