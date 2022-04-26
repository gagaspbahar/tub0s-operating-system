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
        TODO : Implementasi ls
    */
    exit();
}

void ls(char *name, byte currDir){
// 1. ls dapat memperlihatkan isi pada current working directory 
// 2. ls dapat memperlihatkan isi folder yang berada pada current working directory
  int i = 0, found = 0, countDir = 0;
  struct node_filesystem node_fs_buffer;
  byte parentIdx, sectorIdx;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  // Kasus 2, ingin melihat isi folder
  if (!strlen(name) == 0){
    cd(currDir, name, &currDir);
  }

  // Print isi
  for (i = 0;i < 64; i++){
    parentIdx = node_fs_buffer.nodes[i].parent_node_index;
    sectorIdx = node_fs_buffer.nodes[i].sector_entry_index;
    found = strlen(node_fs_buffer.nodes[i].name != 0) && parentIdx == currDir && sectorIdx;

    if (found){
      printString(node_fs_buffer.nodes[i].name);
      printString("\r\n");
      countDir++;
    }
  }

  // Kasus kosong
  if (countDir == 0){
    printString("Empty directory\r\n");
  }
}