#include "header/shell.h"
#include "header/std_lib.h"
#include "header/filesystem.h"
#include "header/kernel.h"

void printCWD(path_str, current_dir){

}

char* splitParam(){
  return;
}

void shell(){
  char input_buf[64];
  char path_str[128];
  byte current_dir = FS_NODE_P_IDX_ROOT;

  while (true) {
    printString("OS@IF2230:");
    printCWD(path_str, current_dir);
    printString("$");
    readString(input_buf);
   
    if (strcmp(input_buf, "cd")){
      // Utility cd
    }
    else 
      printString("Unknown command\r\n");
  }
}

void cd(char *param){
// cd dapat memindah current working directory ke folder tujuan
// cd dapat naik satu tingkat dengan argumen “..”
// cd dapat langsung kembali ke root dengan argumen “/”
// Untuk relative pathing tidak diwajibkan
}

void ls(char *param){
// ls dapat memperlihatkan isi pada current working directory 
// ls dapat memperlihatkan isi folder yang berada pada current working directory

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