#include "../../header/std_type.h"
#include "../../header/textio.h"
#include "../../header/fileio.h"
#include "../../header/program.h"
#include "../../header/utils.h"
#include "../../header/filesystem.h"

int main() {
    struct message msg;
    get_message(&msg);
    mv(msg.current_directory, msg.arg2, msg.arg3);
    exit();
}

void mv(byte currDir, char *name, char *target) {
// mv dapat memindahkan file dan folder ke root dengan “/<nama tujuan>”
// mv dapat memindahkan file dan folder ke dalam parent folder current working directory dengan “../<nama tujuan>”
// mv dapat memasukkan file dan folder ke folder yang berada pada current working directory. 
  char buffer[1024];
  char *parent_dest, *temp;
  struct node_filesystem node_fs_buffer;
  int i, slash = -1;
  byte name_idx, target_idx, parent_idx;
  int name_exist = 0, target_exist = 0, parent_exist = 0, found_parent = 0;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  // Cari indexnya
  for (i = 0; i < 64; i++){
    if (node_fs_buffer.nodes[i].parent_node_index == currDir){
      if (strcmp(node_fs_buffer.nodes[i].name, name) && !name_exist){
        name_idx = i;
        name_exist = 1;
       }
      if (strcmp(node_fs_buffer.nodes[i].name, target) && !target_exist){
        target_idx = i;
        target_exist = 1;
      }
    }
  }

  // case gaada filenya
  if (!name_exist) {
    puts("File doesn't exist\r\n");
    return;
  }

  // case gaada folder target
  if (!target_exist) {
    puts("Target directory not found\r\n");
    return;
  }

  // load sector
  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  if (buffer[target_idx * 16 + 1] != FS_NODE_S_IDX_FOLDER && target_idx != FS_NODE_P_IDX_ROOT) {
    puts("Target is not a folder \r\n");
    return;
  }

  strcpy(parent_dest, target);
  for (i = 0; parent_dest[i] != '\0' ; i++) {
    if (parent_dest[i] == '/') {
      slash = i;
    }
  }

  if (slash == -1) {
    found_parent = 0;
  }
  else {
    while (1) {
      if (parent_dest[slash] == '\0') {
        break;
      }
      parent_dest[slash] = '\0';
    }

    found_parent = 1;
  }

  // Kasus parent tidak ditemukan
  if (!found_parent) {
    parent_idx = FS_NODE_P_IDX_ROOT;
  } else { // Cari idx parent
    for (i = 0; i < 64; i++){
      if (node_fs_buffer.nodes[i].parent_node_index == currDir && strcmp(node_fs_buffer.nodes[i].name, parent_dest)){
        parent_idx = i;
        parent_exist = 1;
        break;
      }
    }

    if (!parent_exist) {
      puts("ERROR\r\n");
      return;
    }
  }

  strcpy(temp, name);

  for (i=0;i < 14 && temp[i] != '\0';i++) {
    buffer[name_idx * 16 + i + 2] = temp[i];
  }

  // null terminator buat sisanya
  for (;i < 14; i++) {
    buffer[name_idx * 16 + i + 2] = '\0';
  }

  buffer[name_idx * 16] = parent_idx;

  interrupt(0x21, 0x3, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x3, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);
}