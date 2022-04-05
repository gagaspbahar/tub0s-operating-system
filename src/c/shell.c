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

void ls(char parentIdx){
// ls dapat memperlihatkan isi pada current working directory 
// ls dapat memperlihatkan isi folder yang berada pada current working directory
  int i = 0, j = 0, idxName, countDir = 0;
  char file[1024];
  interrupt(0x21, 0x2, file, 0x101, 0);
	interrupt(0x21, 0x2, file + 512, 0x102, 0);

  for (i = 0;i < 64; i++){
    if (file[i] == parentIdx){
      idxName = i*16 + 1;
      j = 0;
      if (file[i * 16 + 1] != 0xFF){
        interrupt(0x21, 0x00, "File: ", 0, 0);
      } else {
        interrupt(0x21, 0x00, "Dir: ", 0, 0);
      }

      while (j < 14 && file[idxName + j] != '\0'){
        interrupt(0x10, 0xE00 + file[j + idxName], 0, 0, 0);
				j++;
      }
      countDir++;
      interrupt(0x21, 0x00, "\r\n\0", 0, 0);
    }
  }

  if (countDir == 0){
    interrupt(0x21, 0x00, "Directory kosong\r\n\0", 0, 0);
  }
}

void mv(char *param){
//   mv dapat memindahkan file dan folder ke root dengan “/<nama tujuan>”
// mv dapat memindahkan file dan folder ke dalam parent folder current working directory dengan “../<nama tujuan>”
// mv dapat memasukkan file dan folder ke folder yang berada pada current working directory. 
}

void mkdir(char *param){
  // mkdir dapat membuat folder baru pada current working directory
  byte directory[14];
  char temp[512];
  char currDir;
  char file[1024];
  int i = 0, found = 0, emp, isSuccess;

  // Ambil idx parent & nama file
  interrupt(0x21, 0x02, temp, 512, 0);
  currDir = temp[0];
  while (i < 14){
    directory[i] = temp[i + 1];
    i++;
  }

  for (i = 0; i < 64; i++){
    if (found) break;

    if (file[i * 16] == 0x0 && file[i * 16 + 1] == 0){
      emp = i;
      found = 1;
      break;
    }
  }

  if (found){
    file[emp * 16] = currDir;
    file[emp * 16 + 1] = 0xFF;
    i = 0;
    while (i < 14){
      file[emp * 16 + 2 + i] = directory[i];
      i++;
    }
    interrupt(0x21, 0x3, file, 257, 0);
  }

  interrupt(0x21, 0x3, file, 257, 0);

}

void cat(char *param){
//cat dapat menampilkan isi dari file sebagai text file
  byte fileContent[512 * 16], currDir;
  char name[14], temp[512];
  int i = 0;
  struct file_metadata * data;
  enum fs_retcode *return_code;
  int dump;

  // Ambil nama file dan parentIdx
  interrupt(0x21, 0x02, temp, 512, 0);
  currDir = temp[0];
  while (i < 14){
    name[i] = temp[i+1];
    i++;
  }

  // kosongin buffer
  while (i < 512 * 16){
    fileContent[i] = 0x0;
    i++;
  }

  // Baca file content
  data->buffer = fileContent;
  data->node_name = name;
  data->parent_index = currDir;
  // data.filesize = APA YA BINGUNG
  read(data, return_code);

  if (return_code == 0){
    printString(data->buffer);
    printString("\n");
  } else {
    printString("ERROR\n");
    interrupt(0x21, 0xFF06, "bin/shell", 0x2000, &dump);
  }

}

void cp(char *param){
//   cp dapat melakukan copy file dari current working directory ke current working directory
// Relative pathing dan peng-copyan rekursif folder tidak diwajibkan
}
