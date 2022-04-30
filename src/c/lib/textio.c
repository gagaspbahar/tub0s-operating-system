#include "../header/textio.h"

void puts(char *string) {
    interrupt(0x21, 0x0, string, 0x0, 0x0);
}

int gets(char *string){
    interrupt(0x21, 0x1, string, 0x0, 0x0);
}