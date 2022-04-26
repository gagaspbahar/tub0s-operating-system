#include "../header/shell.h"
#include "../header/std_lib.h"
#include "../header/filesystem.h"
#include "../header/kernel.h"
#include "../header/std_type.h"
#include "../header/string.h"

int main() {
  puts("Halo shell!\r\n");
  while (true);
}

void shell()
{
    int i;
    int j;
    int paramLen;
    char param[8][64];
    char input_buf[64];
    char path_str[128];
    byte current_dir = FS_NODE_P_IDX_ROOT;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 64; j++)
        {
            param[i][j] = 0;
        }
    }
    while (true)
    {
        printString("Tub0S@IF2230:");
        printCWD(path_str, current_dir);
        printString("$");
        readString(input_buf);
        paramLen = splitParam(input_buf, &param);
        if (strcmp(param[0], "cd"))
        {
            // Utility cd
            cd(current_dir, param[1], &current_dir);
        }
        else if (strcmp(param[0], "ls"))
        {
            // util ls
            ls(param[1], current_dir);
        }
        else if (strcmp(param[0], "mv"))
        {
            // util mv
            mv(current_dir, param[1], param[2]);
        }
        else if (strcmp(param[0], "mkdir"))
        {
            // util mkdir
            mkdir(param[1], current_dir);
        }
        else if (strcmp(param[0], "cat"))
        {
            // util cat
            cat(param[1], current_dir);
        }
        else if (strcmp(param[0], "cp"))
        {
            // util cp
            cp(param[1], current_dir, param[2]);
        }
        else
            printString("Unknown command\r\n");
        clear(input_buf, 64);
        clear(param, 512);
    }
}