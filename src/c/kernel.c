// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main()
{
    char *string = "Hai"; // Deklarasikan variabel pada awal scope
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        byte warna = 0xD;
        putInMemory(0xB000, 0x8000 + 2 * i, string[i]);
        putInMemory(0xB000, 0x8001 + 2 * i, warna);
    }
}

// TODO
void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    switch (AX)
    {
    case 0x0:
        printString(BX);
        break;
    case 0x1:
        readString(BX);
        break;
    default:
        printString("Invalid interrupt");
    }
}

void printString(char *string)
{
    int i = 0;
    while(string[i] != '\0'){
        if (string[i] == '\n') interrupt(0x10, 0xe*256+'\r', 0, 0, 0);
        interrupt(0x10, 0xe*256 + string[i], 0, 0, 0);
        i+=1;
    }
}

void readString(char *string)
{
}

void clearScreen()
{
}