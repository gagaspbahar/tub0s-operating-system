#include "header/utils.h"

void get_message(Message *msg){
  readSector(msg, FS_MESSAGE_SECTOR_NUMBER);
}

void set_message(Message *msg){
  writeSector(msg, FS_MESSAGE_SECTOR_NUMBER);
}