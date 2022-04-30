#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE "system.img"

typedef unsigned char byte;

extern void insert_file(byte buf[2880][512], char *fname, byte parent_idx);
extern void create_folder(byte buf[2880][512], char *fname, byte parent_idx);

void shell(byte buf[2880][512]);

int main(int argc, char const *argv[])
{

  // Load entire file and save to buffer
  FILE *image = fopen(IMAGE, "rb");

  if (image == NULL)
  {
    fprintf(stderr, "Error : File image <%s> not found\n", IMAGE);
    exit(1);
  }

  byte imagebuffer[2880][512];
  for (int i = 0; i < 2880; i++)
    fread(imagebuffer[i], 512, 1, image);

  // Close file descriptor and overwrite
  fclose(image);
  image = fopen(IMAGE, "wb");

  shell(imagebuffer);

  // Overwrite old file
  for (int i = 0; i < 2880; i++)
    fwrite(imagebuffer[i], 512, 1, image);

  fclose(image);
  return 0;
}

void shell(byte buf[2880][512])
{
  create_folder(buf, "bin", 0xFF);
  insert_file(buf, "shell", 0);
}
