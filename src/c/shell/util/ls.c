#include "../../header/std_type.h"
#include "../../header/textio.h"
#include "../../header/fileio.h"
#include "../../header/program.h"
#include "../../header/utils.h"
#include "../../header/filesystem.h"

int main() {
    struct message msg;
    get_message(&msg);
    ls(msg.arg2, msg.current_directory);
    exit();
}

void ls(char *name, byte currDir){
// 1. ls dapat memperlihatkan isi pada current working directory 
// 2. ls dapat memperlihatkan isi folder yang berada pada current working directory
  int i = 0, found = 0, countDir = 0;
  struct node_filesystem node_fs_buffer;
  enum fs_retcode retcode;
  byte parentIdx, sectorIdx;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  // Kasus 2, ingin melihat isi folder
  if (!strcmp(name, "")){
    retcode = cd(currDir, name, &currDir);
    if (retcode != FS_SUCCESS)
    {
        return;
    }
  }

  // Print isi
  for (i = 0;i < 64; i++){
    parentIdx = node_fs_buffer.nodes[i].parent_node_index;
    sectorIdx = node_fs_buffer.nodes[i].sector_entry_index;
    found = !strcmp(node_fs_buffer.nodes[i].name, "") && parentIdx == currDir && sectorIdx;

    if (found){
      puts(node_fs_buffer.nodes[i].name);
      puts("\r\n");
      countDir++;
    }
  }

  // Kasus kosong
  if (countDir == 0){
    puts("Empty directory\r\n");
  }
}