#include "header/shell.h"
#include "header/std_lib.h"
#include "header/filesystem.h"
#include "header/kernel.h"

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
      printString("Doesn't have parent directory\r\n");
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
    printString("The system cannot find the path specified.\r\n");
    return_code = FS_R_NODE_NOT_FOUND;
  }
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

void mv(char *param){
// mv dapat memindahkan file dan folder ke root dengan “/<nama tujuan>”
// mv dapat memindahkan file dan folder ke dalam parent folder current working directory dengan “../<nama tujuan>”
// mv dapat memasukkan file dan folder ke folder yang berada pada current working directory. 

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
    if (node_fs_buffer.nodes[i].parent_node_index == data.parent_index && strcmp(node_fs_buffer.nodes[i].name, data.node_name)){
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

  if (return_code == FS_NODE_S_IDX_FOLDER){
    printString("Type is folder\r\n");
  } else if (return_code == FS_R_NODE_NOT_FOUND){
    printString("Node not found\r\n");
  } else {
    printString(data.buffer);
    printString("\r\n");
  }

}

void cp(char *param, byte currDir){
// cp dapat melakukan copy file dari current working directory ke current working directory
// Relative pathing dan peng-copyan rekursif folder tidak diwajibkan
  struct node_filesystem node_fs_buffer;
  struct file_metadata data;
  enum fs_retcode return_code;
  byte buffer[4096];

  // Inisialisasi pemindahan file
  data.parent_index = currDir;
  strcpy(data.node_name, param);

  // Read & Write
  read(&data, &return_code);
  write(&data, &return_code);

  // Error
  if (return_code == FS_W_FILE_ALREADY_EXIST){
    printString("File already exist\r\n");
  }

}
