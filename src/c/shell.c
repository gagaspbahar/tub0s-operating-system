#include "header/shell.h"
#include "header/std_lib.h"
#include "header/filesystem.h"
#include "header/kernel.h"

void cd(char* param, int* idxDir){
// cd dapat memindah current working directory ke folder tujuan
// cd dapat naik satu tingkat dengan argumen “..”
// cd dapat langsung kembali ke root dengan argumen “/”
// Untuk relative pathing tidak diwajibkan
  byte file[512 * 2];
  char currFlName[128];
  int i = 0, j = 0;
  int found = 0;
  int* foundIdx;
  int temp;

  readSector(file, 0x101);
  readSector(file+512, 0x102);

  
  if (param[i] == '/'){
    *idxDir = 0xFF;
    i++;
  }

  int len = strlen(param);
  while (i < len){
    currFlName[j] = param[i];
    if (currFlName[j] == '/' || currFlName[j] == '\0'){
      currFlName[j] = '\0';

      // Cek nama folder
      if (strcmp(currFlName, "..")){
        if (*idxDir != 0xFF){
          *idxDir = file[(*idxDir)*16];
        }
      } else if (strcmp(currFlName,"")) {
        printString("Error! Invalid Folder Name\n");
      } else {
        temp = *idxDir;
        // Cek apakah file ada atau tidak
        int fileidx = 0, k;
        char tempname[14];
        while (fileidx < 64 && !found){
          if (file[16*fileidx] == *idxDir){
            int m;
            for (m = 0; m <14; m++){
              tempname[i] = file[16 * fileidx + (2+i)];
            }
            tempname[14] = '\0';
            
            if (strcmp(tempname, currFlName)){
              if (file[16*fileidx+1] == 0xFF){
                found = 1;
              }
            }
          }
          fileidx++;
        }
        *foundIdx = --fileidx;

        if (!found){
          *idxDir = temp;
          printString("Error! Folder Not Found\n");
          return;
        } else {
          *idxDir = *foundIdx;
        }
      }
      j = 0;
    } else j++;
    i++;
  }
}

void ls(byte currDir){
// ls dapat memperlihatkan isi pada current working directory 
// ls dapat memperlihatkan isi folder yang berada pada current working directory
  int i = 0, j = 0, idxName, countDir = 0;
  char file[1024];
  int found = 0;
  struct node_filesystem node_fs_buffer;
  byte parentIdx, sectorIdx;

  readSector(&node_fs_buffer, FS_NODE_SECTOR_NUMBER);

  for (i = 0;i < 64; i++){
    parentIdx = node_fs_buffer.nodes[i].parent_node_index;
    sectorIdx = node_fs_buffer.nodes[i].sector_entry_index;
    found = strlen(node_fs_buffer.nodes[i].name != 0) && parentIdx == currDir && sectorIdx;

    if (found){
      printString(node_fs_buffer.nodes[i].name);
      printString("\r\n");
    }

    // TODO : HANDLE KALO BUAT PRINT SEMUA ISI FOLDER DALAM DIRNYA JUGA (harus ganti cwd)

    // if (file[i] == parentIdx){
    //   idxName = i*16 + 1;
    //   j = 0;
    //   if (file[i * 16 + 1] != 0xFF){
    //     interrupt(0x21, 0x00, "File: ", 0, 0);
    //   } else {
    //     interrupt(0x21, 0x00, "Dir: ", 0, 0);
    //   }

    //   while (j < 14 && file[idxName + j] != '\0'){
    //     interrupt(0x10, 0xE00 + file[j + idxName], 0, 0, 0);
		// 		j++;
    //   }
    //   countDir++;
    //   interrupt(0x21, 0x00, "\r\n\0", 0, 0);
    // }
  }

  // if (countDir == 0){
  //   interrupt(0x21, 0x00, "Directory kosong\r\n\0", 0, 0);
  // }
}

void mv(char *param){
//   mv dapat memindahkan file dan folder ke root dengan “/<nama tujuan>”
// mv dapat memindahkan file dan folder ke dalam parent folder current working directory dengan “../<nama tujuan>”
// mv dapat memasukkan file dan folder ke folder yang berada pada current working directory. 
}

void mkdir(char *param, byte idxParent){
  // mkdir dapat membuat folder baru pada current working directory
  struct file_metadata *data;
  int isSuccess;

  data->node_name = param;
  data->parent_index = idxParent;
  data->filesize = 0;// Folder size = 0 
  write(data, &isSuccess);

  if (isSuccess == 0){
    printString("Successfully created directory\n");
  } else {
    printString("Build failed\n");
    printString((char)isSuccess);
    printString("\n");
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
    printString("Type is folder\n");
  } else if (return_code == FS_R_NODE_NOT_FOUND){
    printString("Node not found\n");
  } else {
    printString(data.buffer);
    printString("\n");
  }

}

void cp(char *param){
// cp dapat melakukan copy file dari current working directory ke current working directory
// Relative pathing dan peng-copyan rekursif folder tidak diwajibkan
}
