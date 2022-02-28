// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"
#include "header/std_lib.h"

int main()
{
    char buf[128];
    clearScreen();
    makeInterrupt21();
    printString("Halo dunia!\r\n");
    readString(buf);
    printString(buf);
    
    while (true);
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
    while (string[i] != '\0')
    {
        if (string[i] == '\n')
        {
            interrupt(0x10, 0xe * 256 + '\r', 0, 0, 0);
        }
        interrupt(0x10, 0xe * 256 + string[i], 0, 0, 0);
        i += 1;
    }
}

void readString(char *string)
{
    int ch;
    int idx = 0;
    while (true)
    {
        ch = interrupt(0x16, 0, 0, 0, 0);
        if (ch == '\r')
        {
            string[idx] = '\0';
            interrupt(0x10, 0x0E00 + '\n', 0, 0, 0); // LF
            interrupt(0x10, 0x0E00 + '\r', 0, 0, 0); // CR
            break;
        }
        else if (ch = '\b')
        {
            if (idx > 0 && mod(idx, 80) != 0)
            {
                interrupt(0x10, 0x0E00 + '\b', 0, 0, 0); // backspace
                interrupt(0x10, 0x0E00 + '\0', 0, 0, 0); // terminator
                interrupt(0x10, 0x0E00 + '\b', 0, 0, 0); // backspace
                idx--;
            }
        }
        else
        {
            string[idx] = ch;
            interrupt(0x10, 0x0E00 + ch, 0, 0, 0);
            idx++;
        }
    }
}

void clearScreen()
{
    interrupt(0x10, 0x3, 0, 0, 0);
    interrupt(0x10, 0x200, 0, 0, 0);
    putInMemory(0xB000, 0x8001, 0xF);
}