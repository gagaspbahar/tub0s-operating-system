#include "../../header/std_type.h"
#include "../../header/textio.h"
#include "../../header/fileio.h"
#include "../../header/program.h"
#include "../../header/utils.h"
#include "../../header/filesystem.h"

int main() {
    struct message msg;
    get_message(&msg);
    cd(msg.current_directory, msg.arg2, &msg.current_directory);
    exit();
}

void cd(byte idxParent, char *param, byte *idxDir){
// Untuk relative pathing tidak diwajibkan
  int i = 0, found = 0;
  struct node_filesystem node_fs_buffer;
  enum fs_retcode return_code;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  
  // cd dapat langsung kembali ke root dengan argumen “/”
  if (strcmp(param, "/")){
    *idxDir = FS_NODE_P_IDX_ROOT;
    return_code = FS_SUCCESS;
    return;
  }

  // cd dapat naik satu tingkat dengan argumen “..”
  if (strcmp(param, "..")){
    // Kasus sudah di root
    if (idxParent == FS_NODE_P_IDX_ROOT){
      puts("Doesn't have parent directory\r\n");
      return_code = FS_SUCCESS;
    }
    else {
      *idxDir = node_fs_buffer.nodes[idxParent].parent_node_index;
    }
    return;
  }

  // cd dapat memindah current working directory ke folder tujuan
  for (i = 0; i<64; i++){
    if (node_fs_buffer.nodes[i].parent_node_index == idxParent && strcmp(param, node_fs_buffer.nodes[i].name))
    {
        *idxDir = i;
        return_code = FS_SUCCESS;
        found = 1;
        break;
    }
  }
  
  // Folder tidak dapat ditemukan
  if (!found){
    puts("The system cannot find the path specified.\r\n");
    return_code = FS_R_NODE_NOT_FOUND;
  }
}