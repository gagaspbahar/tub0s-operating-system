// Kernel header

#include "std_type.h"
#include "std_lib.h"
#include "filesystem.h"
// #include "utils.h"

// Fungsi bawaan
extern void putInMemory(int segment, int address, byte b);
extern int interrupt (int int_number, int AX, int BX, int CX, int DX);
extern void makeInterrupt21();

void handleInterrupt21(int AX, int BX, int CX, int DX);

void fillKernelMap();

// Implementasikan
void printString(char *string);
void readString(char *string);
void clearScreen();
void writeSector(byte *buffer, int sector_number);
void readSector(byte *buffer, int sector_number);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);

void shell();
// int splitParam(char* input, char** param);
int splitParam(char* input, char param[8][64]);

extern void launchProgram(int segment);
void executeProgram(struct file_metadata *metadata, int segment);
void logo();