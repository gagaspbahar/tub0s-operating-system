#include "header/shell.h"
#include "header/std_lib.h"
#include "header/filesystem.h"

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
  
}

void ls(char *param){
  
}

void mv(char *param){
  
}

void mkdir(char *param){
  
}

void cat(char *param){

}

void cp(char *param){

}